#include "ChessGame.hpp"

#include "Boards/ChessBoard.hpp"
#include "ChessPieces/ChessPiece.hpp"
#include "ChessPieces/King.hpp"

#include <fmt/format.h>

#include <cassert>

namespace MasterChess
{
    ChessGame::ChessGame(unique_ptr<ChessBoard> board, vector<unique_ptr<ChessPlayer>> players, vector<unique_ptr<ChessPiece>> pieces)
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

    unique_ptr<Game::Result> ChessGame::IsGameOver(IPlayer* currentPlayer)
    {
        auto king = currentPlayer == whitePlayer ? whiteKing : blackKing;
        if (!king->IsAttacked())
            return nullptr;
        for (auto piece : Board()->Pieces()) if (piece->Player() == currentPlayer)
        for (auto& [pos, mov] : piece->PossibleMovements())
        {
            SimulateMovementExecution(mov);
            if (king->Board() && !king->IsAttacked())
            {
                SimulateMovementUndo(mov);
                return nullptr;
            }
            SimulateMovementUndo(mov);
        }
        auto result = std::make_unique<Result>();
        result->Winners = { currentPlayer != whitePlayer ? whitePlayer : blackPlayer };
        result->Losers = { currentPlayer };
        result->Message = fmt::format("{} wins by checkmate!", currentPlayer == whitePlayer ? "White" : "Black");
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
}
