#include "MultiPositionChessPiece.hpp"
#include "MasterChess/IBoard.hpp"

namespace MasterChess
{
    MultiPositionChessPiece::MultiPositionChessPiece(IPlayer* player, vector<Vector2Int> deltas) : ChessPiece(player), deltas(move(deltas))
    {

    }

    MovementMap MultiPositionChessPiece::PossibleMovements()
    {
        MovementMap map;
        auto area = Board()->BoardArea();
        auto position = Position();
        for (auto&& delta : deltas)
        {
            auto dst = position + delta;
            if (area.IncludesPosition(dst) && (IsEmpty(dst) || IsCapturable(dst)))
                map.emplace(dst, CreateMovement(dst));
        }
        return map;
    }

}
