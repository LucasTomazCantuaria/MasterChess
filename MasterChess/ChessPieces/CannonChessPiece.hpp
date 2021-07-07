#pragma once
#include "ChessPiece.hpp"

namespace MasterChess
{
    using std::vector;

    struct CannonChessPiece : ChessPiece
    {
        CannonChessPiece(IPlayer* player, vector<Vector2Int> directions, int maxDistance);
        MovementMap PossibleMovements() override;
    private:
        vector<Vector2Int> directions;
        int maxDistance;
    };
}
