#pragma once

#include "Math/Vector2Int.hpp"

namespace MasterChess
{
    struct IPiece;

    struct HitInfo
    {
        Math::Vector2Int Position;
        int Distance;
        IPiece* Piece;
    };
}
