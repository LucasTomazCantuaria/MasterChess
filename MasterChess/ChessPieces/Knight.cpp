#include "Knight.hpp"

namespace MasterChess
{

    Knight::Knight(IPlayer* player) : MultiPositionChessPiece(player,
    {
        Vector2Int::Left  + 2 * Vector2Int::Up,
        Vector2Int::Left  + 2 * Vector2Int::Down,
        Vector2Int::Up    + 2 * Vector2Int::Left,
        Vector2Int::Up    + 2 * Vector2Int::Right,
        Vector2Int::Right + 2 * Vector2Int::Up,
        Vector2Int::Right + 2 * Vector2Int::Down,
        Vector2Int::Down  + 2 * Vector2Int::Left,
        Vector2Int::Down  + 2 * Vector2Int::Right,
    })
    {

    }

    char Knight::Id() const
    {
        return 'n';
    }

}
