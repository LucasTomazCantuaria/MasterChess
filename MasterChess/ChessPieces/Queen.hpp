#pragma once
#include "CannonChessPiece.hpp"

namespace MasterChess
{
    /// <summary>
    /// Classe que será a representação da Dama no jogo.
    /// </summary>
    struct Queen final : CannonChessPiece
    {
        Queen(IPlayer* player, int maxDistance = 8);

        char Id() const override;

    };
}