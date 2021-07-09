#include "MasterChess/Game.hpp"
#include "MasterChess/ChessPlayer.hpp"
#include "MasterChess/ChessPieces.hpp"

#include "Windows/Console.hpp"

#include "Arduino/SerialPort.hpp"
#include "Arduino/SimpleRPC.hpp"
#include "Arduino/ArduinoExported.h"
#include "Arduino/ArduinoChessBoard.h"

#include "Unity/UnityGameListener.hpp"

#include "Math/Area.hpp"

#include <process.hpp>
#include <Windows.h>

#include <unordered_map>
#include <regex>
#include <sstream>
#include <cassert>

namespace MasterChess
{
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
            std::regex r(R"(bestmove\s+([a-h][1-8][a-h][1-8])\s+ponder\s([a-h][1-8][a-h][1-8]))");
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
        explicit StockFishInput() :
            game(nullptr), board(nullptr),
            white(nullptr), whiteLeftRook(true),
            whiteRightRook(true), blackLeftRook(true),
            blackRightRook(true)
        {

        }

        void OnGameStart(Game* game) override
        {
            this->game = game;
            assert(dynamic_cast<ChessBoard*>(game->Board()));
            board = static_cast<ChessBoard*>(game->Board());
            white = game->Player(0);
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
        }

        void OnMovementExecution(IMovement* movement) override
        {
            ++peaceCount;
            ++plays;
            if (dynamic_cast<ChessPiece::CaptureMovement*>(movement) || dynamic_cast<Pawn*>(movement->Piece()))
                peaceCount = 0;
            if (movement->Piece() == whiteKing->CastlePiece(0))
                whiteLeftRook = false;
            if (movement->Piece() == whiteKing->CastlePiece(1))
                whiteRightRook = false;
            if (movement->Piece() == whiteKing)
            {
                whiteLeftRook = false;
                whiteRightRook = false;
            }
            if (movement->Piece() == blackKing->CastlePiece(0))
                blackLeftRook = false;
            if (movement->Piece() == blackKing->CastlePiece(1))
                blackRightRook = false;
            if (movement->Piece() == blackKing)
            {
                blackLeftRook = false;
                blackRightRook = false;
            }
        }

        unique_ptr<IMovement> CreateMovement(IBoard*, IPlayer* player) override
        {
            auto fen = ToFenString(player);
            fish.Command(fmt::format("position fen {}", fen));
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
        ChessBoard* board;
        IPlayer* white;
        int peaceCount = 0;
        int plays = 0;
        StockFish fish;
        King* whiteKing = nullptr;
        King* blackKing = nullptr;
        bool whiteLeftRook, whiteRightRook, blackLeftRook, blackRightRook;

        string ToFenString(IPlayer* currentPlayer) const
        {
            std::stringstream ss;
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

                if (whiteRightRook)
                    token += 'K';
                if (whiteLeftRook)
                    token += 'Q';

                if (blackRightRook)
                    token += 'k';
                if (blackLeftRook)
                    token += 'q';

                if (token.size() > 1)
                    ss << token;
            }
            Pawn::EnPassantMovement* enPassant = nullptr;
            for (auto piece : board->Pieces()) if (auto pawn = dynamic_cast<Pawn*>(piece))
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
        board->AddPiece(v.emplace_back(make_unique<King>(player, move(rooks))).get(), zero + right * 4);
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
    struct PlayRequester final : MasterChess::IInput
    {
        PlayRequester(ArduinoChessBoard* board, IInput* underlyingInput) : board(board), underlyingInput(underlyingInput)
        {

        }

        std::unique_ptr<MasterChess::IMovement> CreateMovement(MasterChess::IBoard*, MasterChess::IPlayer* player) override
        {
            auto movement = underlyingInput->CreateMovement(board, player);
            if (auto mov = dynamic_cast<MasterChess::ChessPiece::Movement*>(movement.get()))
                board->RequestMovement(mov->Piece(), mov->Destination());
            return move(movement);
        }

        std::unique_ptr<IPiece> SelectPromotion(MasterChess::IPlayer* player) override
        {
            return underlyingInput->SelectPromotion(player);
        }

        void OnGameStart(MasterChess::Game* game) override
        {
            underlyingInput->OnGameStart(game);
        }

        void OnMovementExecution(MasterChess::IMovement* movement) override
        {
            underlyingInput->OnMovementExecution(movement);
        }

    private:
        ArduinoChessBoard* board;
        IInput* underlyingInput;
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
        
        //auto serial  = SerialPort("COM3", 2000000);
        //auto arduino = ArduinoExported(&serial);
        //auto board   = std::make_unique<ArduinoChessBoard>(&console, &arduino);

        auto board = std::make_unique<ConsoleChessBoard>(&console);

        auto fish = StockFishInput();
        //auto req = PlayRequester(&*board, &fish);

        auto white = std::make_unique<ChessPlayer>(&*board, 1, 0xFF0000),
             black = std::make_unique<ChessPlayer>(&fish, 2, 0x0000FF);

        board->BeginUpdate();
        auto pieces = CreateArmy(white.get(), &*board, { 0, 0 }, Vector2Int::Right, Vector2Int::Up);
        CreateArmy(pieces, black.get(), &*board, { 0, 7 }, Vector2Int::Right, Vector2Int::Down);
        board->EndUpdate();

        vector<unique_ptr<IPlayer>> players;
        players.emplace_back(move(white));
        players.emplace_back(move(black));

        Game game{ move(board), move(players), move(pieces) };

        //auto unityRpc = Unity::UnityGameListener("localhost", 50051);
        //game.AddListener(&unityRpc);

        auto play = game.Play();
    }
    catch (std::exception& e)
    {
        fmt::print("{}\n", e.what());
    }
}
