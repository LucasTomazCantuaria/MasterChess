#pragma once
#include "MultiPositionChessPiece.hpp"

namespace MasterChess
{
    /// <summary>
    /// Classe que será a representação do Rei no jogo.
    /// </summary>
    struct King final : MultiPositionChessPiece
    {
        struct CastleMovement : Movement
        {
            CastleMovement(King* king, const Vector2Int& direction, IPiece* castlePiece);
            King* Piece() const override;
            void Execute() override;
            void Undo() override;
            IPiece* CastlePiece() const;
        private:
            IPiece* castlePiece;
            Vector2Int castlePieceOrigin, castlePieceDestination;
        };

        King(IPlayer* player, vector<IPiece*> rookPieces);

        char Id() const override;

        MovementMap PossibleMovements() override;

        vector<IPiece*> CastlePieces() const { return castlePieces; }

        IPiece* CastlePiece(int i) const { return castlePieces[i]; }

        bool IsAttacked() const;

    private:
        vector<IPiece*> castlePieces;
        bool IsAttacked(const Vector2Int& position) const;
    };
}
