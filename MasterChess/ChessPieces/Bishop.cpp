#include "Bishop.hpp"

namespace MasterChess
{

    Bishop::Bishop(IPlayer* player, int maxDistance) : CannonChessPiece(player,
    {
        Vector2Int::Up + Vector2Int::Left,
        Vector2Int::Up + Vector2Int::Right,
        Vector2Int::Down + Vector2Int::Left,
        Vector2Int::Down + Vector2Int::Right,
    }, maxDistance)
    {
        
    }

    char Bishop::Id() const
    {
        return 'b';
    }

}