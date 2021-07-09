#include "ArduinoChessBoard.h"

#include <chrono>
#include <thread>

#include <cassert>

using namespace std::chrono_literals;
using namespace std::chrono;
using std::this_thread::sleep_for;

namespace Arduino
{
    namespace
    {
        auto now() { return high_resolution_clock::now(); }
    }

    ArduinoChessBoard::ArduinoChessBoard(MasterChess::IConsole* console, ArduinoExported* arduino) :
        ConsoleChessBoard(console), arduino(arduino),
        selectedPiece(nullptr), ignoreLighten(false)
    {
        Lighten(-1, -1);
    }

    void ArduinoChessBoard::PrintBoard() const
    {
        ConsoleChessBoard::PrintBoard();
        for (auto [color, area] : Colors())
        {
            if (color)
                Lighten(color, Matrix8x8(area).Value);
            else Lighten(-1, Matrix8x8(area).Value);
        }
    }

    IPiece* ArduinoChessBoard::SelectPiece(std::function<bool(IPiece*)> filter)
    {
    start:
        CheckBoard();
        auto v1 = arduino->ReadMatrix();
        Matrix8x8 v2;
        while (true)
        {
            sleep_for(1ms);
            v2 = arduino->ReadMatrix();
            if (v1 != v2)
            {
                auto t = now();
                auto delta = 0ms;
                do
                {
                    sleep_for(1ms);
                    if (v2 != arduino->ReadMatrix())
                        goto start;
                    delta = duration_cast<milliseconds>(now() - t);
                }
                while (delta < 100ms);
                break;
            }
        }
        auto x = v1 ^ v2;
        for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
        {
            if (x(i, j))
            {
                auto p = At({i, j});
                if (!p || !filter(p)) goto start;
                selectedPiece = p;
                return p;
            }
        }
        throw std::runtime_error("Invalid!");
    }

    Vector2Int ArduinoChessBoard::SelectPosition(std::function<bool(const Vector2Int&)> filter)
    {
        auto area = Matrix8x8(selectedPiece->PossibleMovements().MappedArea() | selectedPiece->Position());
        auto v1 = arduino->ReadMatrix();
        Matrix8x8 v2;
    start:
        while (true)
        {
            sleep_for(1ms);
            v2 = arduino->ReadMatrix();
            if (v1 != v2)
            {
                auto t = now();
                auto delta = 0ms;
                do
                {
                    sleep_for(1ms);
                    if (v2 != arduino->ReadMatrix())
                        goto start;
                    delta = duration_cast<milliseconds>(now() - t);
                }
                while (delta < 100ms);
                break;
            }
            Lighten(0, area);
            sleep_for(50ms);
            Lighten(selectedPiece->Player()->Color(), area);
        }
        auto x = v1 ^ v2;
        for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
        {
            if (x(i, j))
            {
                if (!filter({i, j}))
                    goto start;
                selectedPiece = nullptr;
                return {i, j};
            }
        }
        throw std::runtime_error("Invalid!");
    }

    std::unique_ptr<MasterChess::IMovement> ArduinoChessBoard::CreateMovement(IBoard* board, MasterChess::IPlayer* player)
    {
        while (true)
        {
            auto piece = SelectPiece([=](auto p) { return p->Player() == player; });
            auto start = piece->Position();
            auto moves = piece->PossibleMovements();
            auto movArea = moves.MappedArea() | start;
            board->PushColors();
            auto end = SelectPosition([&](auto& v) { return movArea.IncludesPosition(v); });
            board->PopColors();
            if (start != end)
                return move(moves[end]);
        }
    }

    void ArduinoChessBoard::AddPiece(IPiece* piece, const Vector2Int& position)
    {
        ignoreLighten = true;
        ConsoleChessBoard::AddPiece(piece, position);
        ignoreLighten = false;
        LightenArea(position, piece->Player()->Color());
    }

    void ArduinoChessBoard::RepositionPiece(IPiece* piece, const Vector2Int& position)
    {
        ignoreLighten = true;
        auto p = piece->Position();
        ConsoleChessBoard::RepositionPiece(piece, position);
        ignoreLighten = false;
        LightenArea(p, 0);
        LightenArea(position, piece->Player()->Color());
    }

    void ArduinoChessBoard::RemovePiece(IPiece* piece)
    {
        ignoreLighten = true;
        auto p = piece->Position();
        ConsoleChessBoard::RemovePiece(piece);
        ignoreLighten = false;
        LightenArea(p, 0);
    }

    void ArduinoChessBoard::RequestMovement(IPiece* piece, const Vector2Int& position) const
    {
        assert(piece->Position() != position && !ignoreLighten && piece->PossibleMovements().MappedArea().IncludesPosition(position));
        auto color = piece->Player()->Color();
        Matrix8x8 m1 = Math::Area(piece->Position());
        Matrix8x8 m2 = Math::Area(position);

        auto mr = arduino->ReadMatrix();
        auto me = mr;
        me(piece->Position(), false);
        me(position, true);
        while (true)
        {
            mr = arduino->ReadMatrix();
            if (mr == me)
                break;
            Lighten(color, m1.Value);
            Lighten(-1, m2.Value);            
            sleep_for(50ms);
            Lighten(-1, m1.Value);
            Lighten(color, m2.Value);
            sleep_for(50ms);
            Lighten(-1, m2.Value);
            sleep_for(50ms);
            Lighten(color, m2.Value);
            sleep_for(50ms);
        }
    }

    void ArduinoChessBoard::CheckBoard() const
    {
        assert(!ignoreLighten);
        Matrix8x8 ma;
        std::unordered_map<MasterChess::IPlayer*, Matrix8x8> map;
        for (auto piece : Pieces())
        {
            ma(piece->Position(), true);
            map[piece->Player()](piece->Position(), true);
        }
        while (true)
        {
            auto m = arduino->ReadMatrix();
            if (m == ma)
                break;
            auto x = m ^ ma;
            auto d = ~ma & m;
            Lighten(0, x | d);
            sleep_for(50ms);
            Lighten(-1, d);
            for (auto [pl, mask] : map)
                Lighten(pl->Color(), mask);
        }
    }

    void ArduinoChessBoard::OnGameStart(MasterChess::Game* game)
    {
        ConsoleChessBoard::OnGameStart(game);
        CheckBoard();
    }

    void ArduinoChessBoard::Lighten(uint32_t color, Matrix8x8 mat) const
    {
        if (ignoreLighten) return;
        arduino->Lighten(color, mat.Value);
    }
}
