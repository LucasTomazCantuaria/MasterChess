#include "MasterChess/Game.hpp"
#include "MasterChess/ChessPlayer.hpp"
#include "MasterChess/ChessPieces.hpp"

#include "Windows/Console.hpp"
#include "Windows/SerialPort.hpp"

#include "Arduino/SimpleRPC.hpp"
#include "Arduino/ArduinoExported.h"
#include "Arduino/ArduinoChessBoard.h"

#include "Math/Area.hpp"

#include <process.hpp>
#include <Windows.h>

#include <unordered_map>
#include <regex>
#include <sstream>
#include <cassert>

namespace MasterChess
{

    string ToFenString(ChessBoard* board, IPlayer* white, IPlayer* currentPlayer)
    {
        static auto peaceCount = 0;
        static auto plays = 0;

        std::stringstream ss;
        King* whiteKing = nullptr;
        King* blackKing = nullptr;
        auto pieces = board->Pieces();
        for (auto piece : pieces) if (auto king = dynamic_cast<King*>(piece))
        {
            if (king->Player() == white)
            {
                whiteKing = king;
                if (blackKing) break;
            }
            else
            {
                blackKing = king;
                if (whiteKing) break;
            }
        }
        for (int j = 8 - 1; j >= 0; --j)
        {
            auto k = 0;
            for (int i = 0; i < 8; ++i) if (auto p = board->At({ i, j }))
            {
                if (k > 0)
                {
                    ss << char('0' + k);
                    k = 0;
                }
                ss << char(p->Player() == white ? std::toupper(p->Id()) : p->Id());
            }
            else ++k;
            if (k > 0) ss << char('0' + k);
            if (j > 0) ss << '/';
        }
        ss << (currentPlayer == white ? " w" : " b");
        {
            string token = " ";
            token.reserve(5);
            if (whiteKing->MovementCount() == 0)
            for (auto&& [v, m] : whiteKing->PossibleMovements()) if (auto ptr = dynamic_cast<King::CastleMovement*>(m.get()))
            {
                auto origin = ptr->Origin();
                auto dst = ptr->Destination();
                auto delta = dst.x - origin.x;
                if (delta < 0)
                    token += 'K';
                else token += 'Q';
            }
            if (blackKing->MovementCount() == 0)
            for (auto&& [v, m] : blackKing->PossibleMovements()) if (auto ptr = dynamic_cast<King::CastleMovement*>(m.get()))
            {
                auto origin = ptr->Origin();
                auto dst = ptr->Destination();
                auto delta = dst.x - origin.x;
                if (delta < 0)
                    token += 'k';
                else token += 'q';
            }
            if (token.size() > 1)
                ss << token;
        }
        Pawn::EnPassantMovement* enPassant;
        for (auto piece : pieces) if (auto pawn = dynamic_cast<Pawn*>(piece))
        {
            for (auto&& [v, movement] : pawn->PossibleMovements()) if (enPassant = dynamic_cast<Pawn::EnPassantMovement*>(movement.get()))
                break;
            if (enPassant) break;
        }
        if (enPassant)
        {
            auto [x, y] = enPassant->Destination();
            ss << ' ';
            ss << char('a' + x);
            ss << char('1' + y);
        }
        else ss << " -";
        ss << ' ';
        ss << std::to_string(peaceCount);
        ss << ' ';
        ss << std::to_string(plays);
        return ss.str();
    }

    struct StockFish
    {
        struct Movement
        {
            Vector2Int StartPosition, FinalPosition, PonderPosition, PonderFinalPosition;
        };

        template<class Path>
        StockFish(Path&& path) : process(path, {}, [=](auto ptr, auto n)
        {
            std::lock_guard l(m);
            ss += std::string_view{ ptr, n };
        }, nullptr, true)
        {

        }

        StockFish() : StockFish("stockfish")
        {

        }

        StockFish(const StockFish&) = delete;
        StockFish(StockFish&&) = delete;

        StockFish& operator=(const StockFish&) = delete;
        StockFish& operator=(StockFish&&) = delete;

        ~StockFish()
        {
            process.kill();
        }

        bool IsRunning()
        {
            int status;
            return !process.try_get_exit_status(status);
        }

        void Command(std::string_view cmd)
        {
            auto str = fmt::format("{}\n", cmd);
            process.write(str);
        }

        std::string Read()
        {
            std::lock_guard l(m);
            return move(ss);
        }

        void Clear()
        {
            std::lock_guard l(m);
            ss.clear();
        }

        Movement Go(int depth)
        {
            Command(fmt::format("go depth {}", depth));
            std::regex r(R"(bestmove\s+([a-e][1-8][a-e][1-8])\s+ponder\s([a-e][1-8][a-e][1-8]))");
            std::smatch m;
            std::string token;
            do
            {
                std::string str;
                do str = Read();
                while (str.empty());
                size_t pos = 0;
                while ((pos = str.find("\r\n")) != std::string::npos)
                {
                    token = str.substr(0, pos);
                    str.erase(0, pos + 2);
                    if (regex_match(token, m, r))
                        goto end;
                }
            }
            while (true);
            end:

            auto first = m[1].str(),
                second = m[2].str();
            Movement mov;
            mov.StartPosition = { first[0] - 'a', first[1] - '1' };
            mov.FinalPosition = { first[2] - 'a', first[3] - '1' };
            mov.PonderPosition = { second[0] - 'a', second[1] - '1' };
            mov.PonderFinalPosition = { second[2] - 'a', second[3] - '1' };
            return mov;
        }

        void GoPonder()
        {
            
        }

    private:
        TinyProcessLib::Process process;
        std::string ss;
        std::mutex m;
    };

    struct StockFishInput : IInput
    {
        explicit StockFishInput() : game(nullptr) {  }
        void OnGameStart(Game* game) override
        {
            this->game = game;
        }
        unique_ptr<IMovement> CreateMovement(IBoard* board, IPlayer* player) override
        {
            assert(dynamic_cast<ChessBoard*>(board));
            auto chessBoard = static_cast<ChessBoard*>(board);
            auto fen = ToFenString(chessBoard, game->Players()[0].get(), player);
            auto [origin, destination, ponderOrigin, ponderDestination] = fish.Go(5);
            auto piece = board->At(origin);
            assert(piece);
            auto moves = piece->PossibleMovements();
            auto it = moves.find(destination);
            assert(it != moves.end());
            return move(it->second);
        }
        unique_ptr<IPiece> SelectPromotion(IPlayer* player) override { return nullptr; }
    private:
        Game* game;
        StockFish fish;
    };

    void CreateArmy(vector<unique_ptr<IPiece>>& v, ChessPlayer* player, ChessBoard* board, const Vector2Int& zero, const Vector2Int& right, const Vector2Int& up)
    {
        using std::make_unique;
        v.reserve(v.size() + 16);
        for (int i = 0; i < 8; ++i)
            board->AddPiece(v.emplace_back(make_unique<Pawn>(player, up, Ray{ zero + up * 7, right, 8 })).get(), zero + up + right * i);

        vector rooks
        {
            v.emplace_back(make_unique<Rook>(player)).get(),
            v.emplace_back(make_unique<Rook>(player)).get(),
        };
        
        board->AddPiece(rooks[0], zero);
        board->AddPiece(rooks[1], zero + right * 7);
        board->AddPiece(v.emplace_back(make_unique<Knight>(player)).get(), zero + right);
        board->AddPiece(v.emplace_back(make_unique<Bishop>(player)).get(), zero + right * 2);
        board->AddPiece(v.emplace_back(make_unique<Queen>(player)).get(), zero + right * 3);
        board->AddPiece(v.emplace_back(make_unique<King>(player, move(rooks), nullptr)).get(), zero + right * 4);
        board->AddPiece(v.emplace_back(make_unique<Bishop>(player)).get(), zero + right * 5);
        board->AddPiece(v.emplace_back(make_unique<Knight>(player)).get(), zero + right * 6);
    }

    vector<unique_ptr<IPiece>> CreateArmy(ChessPlayer* player, ChessBoard* board, const Vector2Int& zero, const Vector2Int& right, const Vector2Int& up)
    {
        vector<unique_ptr<IPiece>> v;
        CreateArmy(v, player, board, zero, right, up);
        return v;
    }

}

namespace Arduino
{    

    struct PlayRequester final : MasterChess::ChessPlayer::TwoStepsInput
    {
        PlayRequester() = default;

        PlayRequester(ArduinoChessBoard* board, IPiece* piece, vector<Vector2Int> positions)
            : board(board),
              piece(piece),
              positions(move(positions)),
              it(&this->positions[0])
        {

        }

        IPiece* SelectPiece(std::function<bool(IPiece*)> filter) override
        {
            if (it == positions.data() + positions.size())
                return board->SelectPiece(move(filter));
            board->RequestMovement(piece, *it);
            return piece;
        }

        Vector2Int SelectPosition(std::function<bool(const Vector2Int&)> filter) override
        {
            if (it == positions.data() + positions.size())
                return board->SelectPosition(move(filter));
            return *it++;
        }

    private:
        ArduinoChessBoard* board;
        IPiece* piece;
        vector<Vector2Int> positions;
        Vector2Int* it;
    };

}

int main(int argc, char* argv[])
{
    using namespace MasterChess;
    using namespace Windows;
    using namespace Arduino;
    try
    {
        auto console = Console(220, 220);

        auto serial = SerialPort("COM3", 115200);
        auto arduino = ArduinoExported(&serial);
        auto board = std::make_unique<ArduinoChessBoard>(&console, &arduino);

        //auto board = std::make_unique<ConsoleChessBoard>(&console);
        //auto fish = StockFishInput();

        auto white = std::make_unique<ChessPlayer>(&*board, 1, 0xFF0000),
             black = std::make_unique<ChessPlayer>(&*board, 2, 0x0000FF);

        board->BeginUpdate();
        auto pieces = CreateArmy(white.get(), &*board, { 0, 0 }, Vector2Int::Right, Vector2Int::Up);
        CreateArmy(pieces, black.get(), &*board, { 0, 7 }, Vector2Int::Right, Vector2Int::Down);
        board->EndUpdate();        

        vector<unique_ptr<IPlayer>> players;
        players.emplace_back(move(white));
        players.emplace_back(move(black));

        Game game{ move(board), move(players), move(pieces) };
        auto play = game.Play();
    }
    catch (std::exception& e)
    {
        fmt::print("{}\n", e.what());
    }
}
