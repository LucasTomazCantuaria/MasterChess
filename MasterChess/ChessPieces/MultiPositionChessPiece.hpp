#pragma once
#include "ChessPiece.hpp"

namespace MasterChess
{
    using std::vector;

    struct MultiPositionChessPiece : ChessPiece
    {
        MultiPositionChessPiece(IPlayer* player, vector<Vector2Int> deltas);

        MovementMap PossibleMovements() override;

    private:
        vector<Vector2Int> deltas;
    };

}
