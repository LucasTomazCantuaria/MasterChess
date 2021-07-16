#include "StockfishInput.hpp"

#include "MasterChess/ChessGame.hpp"
#include "MasterChess/Boards/ChessBoard.hpp"
#include "MasterChess/ChessPieces/Bishop.hpp"
#include "MasterChess/ChessPieces/King.hpp"
#include "MasterChess/ChessPieces/Knight.hpp"
#include "MasterChess/ChessPieces/Pawn.hpp"
#include "MasterChess/ChessPieces/Queen.hpp"
#include "MasterChess/ChessPieces/Rook.hpp"

#include <fmt/format.h>

#include <cassert>
#include <mutex>
#include <process.hpp>
#include <regex>
#include <sstream>
#include <optional>

namespace MasterChess
{
    struct StockFishInput::StockFish
    {
        struct Ponder
        {
            Vector2Int StartPosition, FinalPosition;
            std::optional<char> Promotion;
        };

        struct Movement
        {
            Vector2Int StartPosition, FinalPosition;
            std::optional<char> Promotion;
            std::optional<Ponder> Ponder;
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
            std::regex r(R"(bestmove\s+([a-h][1-8][a-h][1-8][QqRrBbNn]?)(?:\s+ponder\s([a-h][1-8][a-h][1-8][QqRrBbNn]?))?)");
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
            } while (true);
        end:
            auto first = m[1].str();
            Movement mov;
            mov.StartPosition = { first[0] - 'a', first[1] - '1' };
            mov.FinalPosition = { first[2] - 'a', first[3] - '1' };
            if (first.length() > 4)
                mov.Promotion = first[4];
            if (m[2].matched)
            {
                auto second = m[2].str();
                auto& [start, pos, promo] = mov.Ponder.emplace();
                start = { second[0] - 'a', second[1] - '1' };
                pos = { second[2] - 'a', second[3] - '1' };
                if (second.length() > 4)
                    promo = second[4];
            }
            return mov;
        }

        void GoPonder()
        {
            process.write("go ponder\n");
        }

    private:
        TinyProcessLib::Process process;
        std::string ss;
        std::mutex m;
    };

    StockFishInput::StockFishInput():
        game(nullptr), board(nullptr), white(nullptr),
        fish(std::make_unique<StockFish>()),
        whiteKing(nullptr), blackKing(nullptr),
        whiteLeftRook(false), whiteRightRook(false),
        blackLeftRook(false), blackRightRook(false),
        nextPromotion('Q')
    {

    }

    StockFishInput::~StockFishInput() = default;

    void StockFishInput::OnGameStart(Game* game)
    {
        this->game = static_cast<ChessGame*>(game);
        assert(dynamic_cast<ChessBoard*>(game->Board()) && "Board must be a ChessBoard!");
        board = static_cast<ChessBoard*>(game->Board());
        white = game->Player(0);
        auto pieces = board->Pieces();
        for (auto piece : pieces)
        if (auto king = dynamic_cast<King*>(piece))
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
        assert(whiteKing && "White king not found!");
        assert(blackKing && "Black king not found!");
        pieces = whiteKing->CastlePieces();
        whiteLeftRook = !pieces.empty();
        whiteRightRook = pieces.size() > 1;
        pieces = blackKing->CastlePieces();
        blackLeftRook = !pieces.empty();
        blackRightRook = pieces.size() > 1;
    }

    void StockFishInput::OnMovementExecution(IMovement* movement)
    {
        if (whiteLeftRook && (movement->Piece() == whiteKing->CastlePiece(0) || movement->Piece() == whiteKing))
            whiteLeftRook = false;
        if (whiteRightRook && (movement->Piece() == whiteKing->CastlePiece(1) || movement->Piece() == whiteKing))
            whiteRightRook = false;

        if (blackLeftRook && (movement->Piece() == blackKing->CastlePiece(0) || movement->Piece() == blackKing))
            blackLeftRook = false;
        if (blackRightRook && (movement->Piece() == blackKing->CastlePiece(1) || movement->Piece() == blackKing))
            blackRightRook = false;
    }

    unique_ptr<IMovement> StockFishInput::CreateMovement(IBoard*, IPlayer* player)
    {
        auto fen = game->Fen();
        fish->Command(fmt::format("position fen {}", fen));
        auto [origin, destination, promo, ponder] = fish->Go(5);
        auto piece = board->At(origin);
        assert(piece);
        auto moves = piece->PossibleMovements();
        auto it = moves.find(destination);
        assert(it != moves.end());
        if (promo)
            nextPromotion = *promo;
        return move(it->second);
    }

    unique_ptr<IPiece> StockFishInput::SelectPromotion(IPlayer* player)
    {
        switch (nextPromotion)
        {
        case 'Q':
        case 'q': return std::make_unique<Queen>(player);
        case 'R':
        case 'r': return std::make_unique<Rook>(player);
        case 'B':
        case 'b': return std::make_unique<Bishop>(player);
        case 'N':
        case 'n': return std::make_unique<Knight>(player);
        }
#ifdef _MSC_VER
        __assume(false);
#else
        __builtin_unreachable();
#endif
    }

    std::string StockFishInput::ToFenString(IPlayer* currentPlayer) const
    {        
        assert(board && "Use AddListener!");
        std::stringstream ss;
        for (int j = 8 - 1; j >= 0; --j)
        {
            auto k = 0;
            for (int i = 0; i < 8; ++i)
            if (auto p = board->At({i, j}))
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
        if (auto doubleMovement = dynamic_cast<Pawn::DoubleMovement*>(game->LastMovement()))
        {
            auto [x, y] = doubleMovement->Destination() - doubleMovement->Piece()->Direction();
            ss << ' ';
            ss << char('a' + x);
            ss << char('1' + y);
        }
        else ss << " -";
        ss << ' ';
        ss << std::to_string(game->PeaceCount());
        ss << ' ';
        ss << std::to_string(std::max(game->PlayCount(), 1));
        return ss.str();
    }
}
