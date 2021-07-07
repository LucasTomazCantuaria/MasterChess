#include "CannonChessPiece.hpp"
#include "MasterChess/HitInfo.hpp"
#include "MasterChess/IBoard.hpp"

namespace MasterChess
{
    CannonChessPiece::CannonChessPiece(IPlayer* player, vector<Vector2Int> directions, int maxDistance) : ChessPiece(player), directions(move(directions)), maxDistance(maxDistance)
    {

    }

    MovementMap CannonChessPiece::PossibleMovements()
    {
        MovementMap map;
        HitInfo hitInfo;
        auto position = Position();
        for (auto& v : directions)
        {
            Ray ray = { position, v, maxDistance, 1 };
            if (Board()->CastRay(ray, &hitInfo) && IsCapturable(hitInfo.Piece))
                map.emplace(hitInfo.Position, CreateMovement(hitInfo.Position));
            for (int i = 1; i < hitInfo.Distance; ++i)
            {
                auto pos = ray.Position(i);
                map.emplace(pos, std::make_unique<Movement>(this, pos));
            }
        }
        return map;
    }

}
