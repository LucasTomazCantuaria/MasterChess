#pragma once
#include "MasterChess/ChessPlayer.hpp"
#include "MasterChess/IPiece.hpp"
#include "MasterChess/Inputs/TwoStepsInput.hpp"

namespace MasterChess
{
    using MasterChess::IPiece;
    using Math::Area;
    using Math::Vector2Int;

    struct ChessBoard;
    struct IConsole;

    struct ConsoleChessGameListener final : IGameListener, TwoStepsInput
    {
        explicit ConsoleChessGameListener(IConsole* console, const Vector2Int& size = {12, 7});

        void OnGameStart(Game* game) override;

        void OnMovementExecution(IMovement* movement) override;

        Game* CurrentGame() override { return game; }

        IPiece* SelectPiece(IPlayer* currentPlayer) override;

        Vector2Int SelectPosition(Area area) override;

        void PrintBoard() const;

        void SelectPosition();

        std::unique_ptr<IPiece> SelectPromotion(IPlayer* player) override;

    private:
        IConsole* console;
        Vector2Int size;
        Vector2Int cursor;
        Game* game;
        ChessBoard* board;
        Area colored;
    };

}
