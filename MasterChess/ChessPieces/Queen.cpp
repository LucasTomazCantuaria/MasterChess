#include "Queen.hpp"

namespace MasterChess
{

    Queen::Queen(IPlayer* player, int maxDistance) : CannonChessPiece(player,
    {
        Vector2Int::Right,
        Vector2Int::Left,
        Vector2Int::Up,
        Vector2Int::Down,
        Vector2Int::Up + Vector2Int::Right,
        Vector2Int::Up + Vector2Int::Left,
        Vector2Int::Down + Vector2Int::Right,
        Vector2Int::Down + Vector2Int::Left,
    }, maxDistance)
    {

    }

    char Queen::Id() const
    {
        return 'q';
    }

}
