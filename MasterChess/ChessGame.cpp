#include "ChessGame.hpp"

#include "Boards/ChessBoard.hpp"
#include "ChessPieces/ChessPiece.hpp"
#include "ChessPieces/King.hpp"
#include "ChessPieces/Pawn.hpp"

#include <fmt/format.h>

#include <sstream>

#include <cassert>

namespace MasterChess
{
    ChessGame::ChessGame() :
        whitePlayer(nullptr), blackPlayer(nullptr),
        whiteKing(nullptr), blackKing(nullptr),
        peaceCount(0)
    {

    }

    ChessGame::ChessGame(unique_ptr<ChessBoard> board, vector<unique_ptr<ChessPlayer>> players, vector<unique_ptr<ChessPiece>> pieces) : peaceCount(0)
    {
        assert(players.size() <= 2);
        SetBoard(move(board));
        for (auto& player : players)
            AddPlayer(move(player));
        for (auto& piece : pieces)
            AddPiece(move(piece));
    }

    bool ChessGame::ValidateMovement(unique_ptr<IMovement>& movement)
    {
        auto king = movement->Piece()->Player() == whitePlayer ? whiteKing : blackKing;
        SimulateMovementExecution(movement);
        for (auto piece : Board()->Pieces())
        {
            auto moves = piece->PossibleMovements();
            for (auto& [pos, mov] : moves) if (pos == king->Position())
            {
                SimulateMovementUndo(movement);
                return false;
            }
        }
        SimulateMovementUndo(movement);
        return true;
    }

    unique_ptr<GameResult> ChessGame::IsGameOver(IPlayer* currentPlayer)
    {
        auto king = currentPlayer == whitePlayer ? whiteKing : blackKing;
        auto pieces = Board()->Pieces();
        if (pieces.size() == 2)
        {
            auto result = std::make_unique<GameResult>();
            result->Losers = { whitePlayer, blackPlayer };
            result->Message = fmt::format("Draw by insufficient material.");
            return result;
        }
        auto hasLegalMove = [&]
        {
            for (auto piece : pieces) if (piece->Player() == currentPlayer)
            for (auto& [pos, mov] : piece->PossibleMovements())
            {
                SimulateMovementExecution(mov);
                if (king->Board() && !king->IsAttacked())
                {
                    SimulateMovementUndo(mov);
                    return true;
                }
                SimulateMovementUndo(mov);
            }
            return false;
        }();
        if (hasLegalMove)
            return nullptr;
        auto result = std::make_unique<GameResult>();
        if (king->IsAttacked())
        {
            result->Winners = { currentPlayer != whitePlayer ? whitePlayer : blackPlayer };
            result->Losers = { currentPlayer };
            result->Message = fmt::format("{} wins by checkmate.", currentPlayer == whitePlayer ? "White" : "Black");
        }
        else
        {
            result->Losers = { whitePlayer, blackPlayer };
            result->Message = fmt::format("Draw by stalemate.");
        }
        return result;
    }

    ChessPlayer* ChessGame::AddPlayer(unique_ptr<IPlayer> player)
    {
        assert(dynamic_cast<ChessPlayer*>(player.get()) && Players().size() < 2);
        (Players().empty() ? whitePlayer : blackPlayer) = player.get();
        return static_cast<ChessPlayer*>(Game::AddPlayer(move(player)));
    }

    IPiece* ChessGame::AddPiece(unique_ptr<IPiece> piece)
    {
        assert(dynamic_cast<ChessPiece*>(piece.get()));
        if (auto king = dynamic_cast<King*>(piece.get()))
            (king->Player() == whitePlayer ? whiteKing : blackKing) = king;
        return Game::AddPiece(move(piece));
    }

    string ChessGame::Fen()
    {
        auto board = Board();
        assert(board && "Use AddListener!");
        std::stringstream ss;
        for (int j = 8 - 1; j >= 0; --j)
        {
            auto k = 0;
            for (int i = 0; i < 8; ++i)
            if (auto p = board->At({ i, j }))
            {
                if (k > 0)
                {
                    ss << char('0' + k);
                    k = 0;
                }
                ss << char(p->Player() == whitePlayer ? std::toupper(p->Id()) : p->Id());
            }
            else ++k;
            if (k > 0) ss << char('0' + k);
            if (j > 0) ss << '/';
        }
        ss << (CurrentPlayer() == whitePlayer ? " w" : " b");
        {
            string token = " ";
            token.reserve(5);
            if (whiteKing->MovementCount() == 0)
            if (auto pieces = whiteKing->CastlePieces(); !pieces.empty())
            {
                token += 'K';
                if (pieces.size() > 1)
                    token += 'Q';
            }
            if (blackKing->MovementCount() == 0)
            if (auto pieces = blackKing->CastlePieces(); !pieces.empty())
            {
                token += 'k';
                if (pieces.size() > 1)
                    token += 'q';
            }
            if (token.size() > 1)
                ss << token;
        }
        if (auto doubleMovement = dynamic_cast<Pawn::DoubleMovement*>(LastMovement()))
        {
            auto [x, y] = doubleMovement->Destination() - doubleMovement->Piece()->Direction();
            ss << ' ';
            ss << char('a' + x);
            ss << char('1' + y);
        }
        else ss << " -";
        ss << ' ';
        ss << std::to_string(PeaceCount());
        ss << ' ';
        ss << std::to_string(std::max(PlayCount(), 1));
        return ss.str();
    }

    void ChessGame::ExecuteMovement(unique_ptr<IMovement> movement)
    {
        ++peaceCount;
        if (dynamic_cast<Pawn*>(movement->Piece()) || dynamic_cast<ChessPiece::CaptureMovement*>(movement.get()))
            peaceCount = 0;
        Game::ExecuteMovement(move(movement));
    }

    void ChessGame::UndoLastMovement()
    {
        auto movement = LastMovement();
        if (!dynamic_cast<Pawn*>(movement->Piece()) && !dynamic_cast<ChessPiece::CaptureMovement*>(movement))
            --peaceCount;
        Game::UndoLastMovement();
    }
}
