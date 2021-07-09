#pragma once

namespace MasterChess
{
    struct IPiece;
    /// <summary>
    /// Abstração de um movimento do jogo.
    /// </summary>
    struct IMovement
    {
        /// <summary>
        /// Peça que criou a movimentação.
        /// </summary>
        /// <returns></returns>
        virtual IPiece* Piece() const = 0;
        /// <summary>
        /// Executa a movimentação.
        /// </summary>
        virtual void Execute() = 0;
        /// <summary>
        /// Desfaz a movimentação.
        /// </summary>
        virtual void Undo() = 0;
    };
}
