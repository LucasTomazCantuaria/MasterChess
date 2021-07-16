#include "ArduinoChessBoard.hpp"
#include "ArduinoExported.hpp"

#include "MasterChess/IBoard.hpp"
#include "MasterChess/IPlayer.hpp"
#include "MasterChess/ChessPieces/ChessPiece.hpp"

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

    ArduinoChessBoard::PlayRequester::PlayRequester(ArduinoChessBoard* board, IInput* underlyingInput): board(board), underlyingInput(underlyingInput)
    {

    }

    std::unique_ptr<MasterChess::IMovement> ArduinoChessBoard::PlayRequester::CreateMovement(MasterChess::IBoard* b, MasterChess::IPlayer* player)
    {
        auto movement = underlyingInput->CreateMovement(b, player);
        if (auto mov = dynamic_cast<MasterChess::ChessPiece::Movement*>(movement.get()))
            board->RequestMovement(mov->Piece(), mov->Destination());
        return move(movement);
    }

    std::unique_ptr<IPiece> ArduinoChessBoard::PlayRequester::SelectPromotion(MasterChess::IPlayer* player)
    {
        return underlyingInput->SelectPromotion(player);
    }

    ArduinoChessBoard::ArduinoChessBoard(ArduinoExported* arduino, IInput* piecePromotionInput) : arduino(arduino), selectedPiece(nullptr), piecePromotionInput(piecePromotionInput)
    {
        Lighten(-1, -1);
    }

    IPiece* ArduinoChessBoard::SelectPiece(MasterChess::IPlayer* currentPlayer)
    {
        auto board = game->Board();
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
                } while (delta < 100ms);
                break;
            }
        }
        auto x = v1 ^ v2;
        for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
        {
            if (!x(i, j)) continue;
            auto p = board->At({ i, j });
            if (!p || p->Player() != currentPlayer)
                goto start;
            selectedPiece = p;
            return p;
        }
        throw std::runtime_error("Invalid!");
    }

    Vector2Int ArduinoChessBoard::SelectPosition(Math::Area a)
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
                } while (delta < 100ms);
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
            if (!x(i, j)) continue;
            if (!a.IncludesPosition({ i, j }))
                goto start;
            auto v = Vector2Int(i, j);
            if (selectedPiece->Position() == v)
                PrintBoard();
            selectedPiece = nullptr;
            return v;
        }
        throw std::runtime_error("Invalid!");
    }

    void ArduinoChessBoard::PrintBoard() const
    {
        std::unordered_map<uint32_t, Matrix8x8> map;
        for (auto piece : game->Board()->Pieces())
            map[piece->Player()->Color()](piece->Position(), true);
        uint64_t blank = 0;
        for (auto [color, area] : map)
        {
            Lighten(color, area);
            blank |= area.Value;
        }
        Lighten(-1, ~blank);
    }

    std::unique_ptr<IPiece> ArduinoChessBoard::SelectPromotion(MasterChess::IPlayer* player)
    {
        return piecePromotionInput->SelectPromotion(player);
    }

    void ArduinoChessBoard::RequestMovement(IPiece* piece, const Vector2Int& position) const
    {
        assert(piece->Position() != position && piece->PossibleMovements().MappedArea().IncludesPosition(position));
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
        auto board = game->Board();
        Matrix8x8 ma;
        std::unordered_map<MasterChess::IPlayer*, Matrix8x8> map;
        for (auto piece : board->Pieces())
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
            auto d = m & ~ma;
            Lighten(0, x | d);
            sleep_for(50ms);
            Lighten(-1, d);
            for (auto [pl, mask] : map)
                Lighten(pl->Color(), mask);
        }
    }

    void ArduinoChessBoard::OnGameStart(MasterChess::Game* game)
    {
        assert(dynamic_cast<MasterChess::ChessGame*>(game));
        this->game = static_cast<MasterChess::ChessGame*>(game);
        PrintBoard();
        CheckBoard();
    }

    void ArduinoChessBoard::OnMovementExecution(MasterChess::IMovement* movement)
    {
        PrintBoard();
    }

    MasterChess::ChessGame* ArduinoChessBoard::CurrentGame()
    {
        return game;
    }

    void ArduinoChessBoard::Lighten(uint32_t color, Matrix8x8 mat) const
    {
        arduino->Lighten(color, mat.Value);
    }
}
