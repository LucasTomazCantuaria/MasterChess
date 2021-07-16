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
        ChessGame();

        ChessGame(unique_ptr<ChessBoard> board, vector<unique_ptr<ChessPlayer>> players, vector<unique_ptr<ChessPiece>> pieces);

        int PeaceCount() const { return peaceCount; }

        bool ValidateMovement(unique_ptr<IMovement>& movement) override;

        unique_ptr<GameResult> IsGameOver(IPlayer* currentPlayer) override;

        ChessPlayer* AddPlayer(unique_ptr<IPlayer> player) override;

        IPiece* AddPiece(unique_ptr<IPiece> piece) override;

        string Fen();

    protected:
        void ExecuteMovement(unique_ptr<IMovement> movement) override;

        void UndoLastMovement() override;

    private:
        IPlayer* whitePlayer;
        IPlayer* blackPlayer;
        King* whiteKing;
        King* blackKing;
        int peaceCount;
    };
}
