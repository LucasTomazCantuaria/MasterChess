#pragma once 
#include "ChessPiece.hpp"

namespace MasterChess
{
    /// <summary>
    /// Classe que será a representação do Peão no jogo.
    /// </summary>
    struct Pawn final : ChessPiece
    {
        struct DoubleMovement : Movement
        {
            DoubleMovement(Pawn* piece, const Vector2Int& destination);
            Pawn* Piece() const override;
        };

        struct EnPassantMovement : CaptureMovement
        {
            EnPassantMovement(Pawn* pawn, const Vector2Int& destination, Pawn* capturedPawn);
            Pawn* Piece() const override;
        };

        struct PromotionMovement : Movement
        {
            PromotionMovement(ChessPiece* piece, const Vector2Int& destination, unique_ptr<IMovement> innerMovement);
            void Execute() override;
            void Undo() override;
            IMovement* InnerMovement() const { return innerMovement.get(); }
            IPiece* PromotedPiece() const { return promotedPiece; }
        private:
            unique_ptr<IMovement> innerMovement;
            IPiece* promotedPiece;
            IBoard* board;
        };

        Pawn(IPlayer* player, const Vector2Int& direction, Area promotionArea, bool disableDoubleMovement = false);

        char Id() const override;

        MovementMap PossibleMovements() override;

        const Vector2Int& Direction() const { return direction; }

    private:
        Vector2Int direction;
        Area promotionArea;
        bool disableDoubleMovement;
    };
    
}
