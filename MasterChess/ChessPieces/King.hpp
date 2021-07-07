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

        struct IListener
        {
            virtual void OnExecuteCastle(CastleMovement* castle) = 0;
            virtual void OnUndoCastle(CastleMovement* castle) = 0;
        };

        King(IPlayer* player, vector<IPiece*> rookPieces, IListener* listener);

        char Id() const override;

        MovementMap PossibleMovements() override;

        IListener* Listener() const;

    private:
        vector<IPiece*> castlePieces;
        IListener* listener;
        bool IsAttacked(const Vector2Int& position) const;
    };
}
