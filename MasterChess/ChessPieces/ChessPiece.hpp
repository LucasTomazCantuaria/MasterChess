#pragma once
#include "MasterChess/IMovement.hpp"
#include "MasterChess/IPiece.hpp"
#include "Math/Vector2Int.hpp"

#include <memory>

namespace MasterChess
{
    using std::unique_ptr;
    using Math::Vector2Int;

    struct ChessPiece : IPiece
    {
        struct Movement : IMovement
        {
            Movement(ChessPiece* piece, const Vector2Int& destination);
            ChessPiece* Piece() const override;
            void Execute() override;
            void Undo() override;
            Vector2Int Origin() const;
            Vector2Int Destination() const;
        private:
            ChessPiece* piece;
            Vector2Int origin, destination;
        };
        struct CaptureMovement : Movement
        {
            CaptureMovement(ChessPiece* piece, const Vector2Int& destination, IPiece* captured);
            void Execute() override;
            void Undo() override;
            IPiece* Captured() const;
        private:
            IPiece* captured;
            Vector2Int capturedDestination;
        };        
        friend Movement;

        ChessPiece(IPlayer* player);
        IPlayer* Player() const final;
        IBoard* Board() const final;
        Vector2Int Position() const override;
        Game* CurrentGame() const { return game; }
        int MovementCount() const override;
        void OnAddToBoard(IBoard* board) override;
        void OnRemoveFromBoard() override;
        void OnGameStart(Game* game) override;

        unique_ptr<IMovement> CreateMovement(const Vector2Int& position);

    protected:
        int movementCount;
        bool IsEmpty(const Vector2Int& position) const;
        bool IsCapturable(const Vector2Int& position) const;
        bool IsCapturable(IPiece* piece) const;
    private:
        IPlayer* player;
        IBoard* board;
        Game* game;
    };
}
