#pragma once
#include "Game.hpp"
#include "ChessPlayer.hpp"

namespace MasterChess
{
    struct ChessBoard;
    struct ChessPiece;
    struct King;

    struct ChessGame final : Game
    {
        ChessGame() = default;

        ChessGame(unique_ptr<IBoard> board, vector<unique_ptr<IPlayer>> players, vector<unique_ptr<IPiece>> pieces) = delete;

        ChessGame(unique_ptr<ChessBoard> board, vector<unique_ptr<ChessPlayer>> players, vector<unique_ptr<ChessPiece>> pieces);

        bool ValidateMovement(unique_ptr<IMovement>& movement) override;

        unique_ptr<Result> IsGameOver(IPlayer* currentPlayer) override;

        ChessPlayer* AddPlayer(unique_ptr<IPlayer> player) override;

        IPiece* AddPiece(unique_ptr<IPiece> piece) override;

    private:
        IPlayer* whitePlayer;
        IPlayer* blackPlayer;
        King* whiteKing;
        King* blackKing;
    };
}
