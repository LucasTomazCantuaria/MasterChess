#include "Rook.hpp"

namespace MasterChess
{

    Rook::Rook(IPlayer* player, int maxDistance) : CannonChessPiece(player,
    {
        Vector2Int::Right,
        Vector2Int::Left,
        Vector2Int::Up,
        Vector2Int::Down,
    }, maxDistance)
    {

    }

    char Rook::Id() const
    {
        return 'r';
    }

}