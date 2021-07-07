#pragma once
#include "CannonChessPiece.hpp"

namespace MasterChess
{
    /// <summary>
    /// Classe que será a representação do Bispo no jogo.
    /// </summary>
    struct Bishop final : CannonChessPiece
    {
        Bishop(IPlayer* player, int maxDistance = 8);

        char Id() const override;

    };
}