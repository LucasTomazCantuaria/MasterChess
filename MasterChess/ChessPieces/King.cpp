#include "King.hpp"
#include "MasterChess/IBoard.hpp"
#include "MasterChess/ChessPlayer.hpp"
#include "MasterChess/HitInfo.hpp"

#include <cassert>

#include "MasterChess/Game.hpp"

namespace MasterChess
{
    King::CastleMovement::CastleMovement(King* king, const Vector2Int& direction, IPiece* castlePiece) :
        Movement(king, king->Position() + direction * 2), castlePiece(castlePiece),
        castlePieceOrigin(castlePiece->Position()), castlePieceDestination(king->Position() + direction * 1)
    {
        assert(king->MovementCount() == 0 && king->Board() == castlePiece->Board());
    }

    King* King::CastleMovement::Piece() const
    {
        return (King*)Movement::Piece();
    }

    void King::CastleMovement::Execute()
    {
        auto board = castlePiece->Board();
        board->RepositionPiece(castlePiece, castlePieceDestination);
        Movement::Execute();
    }

    void King::CastleMovement::Undo()
    {
        Movement::Undo();
        auto board = castlePiece->Board();
        board->RepositionPiece(castlePiece, castlePieceOrigin);
    }

    IPiece* King::CastleMovement::CastlePiece() const
    {
        return castlePiece;
    }

    King::King(IPlayer* player, vector<IPiece*> rookPieces) :
        MultiPositionChessPiece(player,
        {
            Vector2Int::Left,
            Vector2Int::Up + Vector2Int::Left,
            Vector2Int::Up,
            Vector2Int::Up + Vector2Int::Right,
            Vector2Int::Right,
            Vector2Int::Down + Vector2Int::Right,
            Vector2Int::Down,
            Vector2Int::Down + Vector2Int::Left,
        }),
        castlePieces(move(rookPieces))
    {

    }

    char King::Id() const
    {
        return 'k';
    }

    MovementMap King::PossibleMovements()
    {
        auto map = MultiPositionChessPiece::PossibleMovements();        
        if (MovementCount() != 0) return map;
        auto board = Board();
        auto position = Position();
        for (auto piece : castlePieces) if (piece->MovementCount() == 0)
        {
            auto dir = piece->Position() - position;
            auto& [x, y] = dir;
            if (x != y && !x && !y || IsAttacked(position)) continue;
            if (x) x = x > 0 ? 1 : -1;
            if (y) y = y > 0 ? 1 : -1;
            Ray ray{ position, dir, 5, 1 };
            auto positions = Area{ ray }.Positions();
            auto it = find_if(positions.begin(), positions.end(), [=](auto& v) { return IsAttacked(v); });
            if (it != positions.end()) continue;
            HitInfo hit;
            if (!board->CastRay(ray, &hit) || hit.Piece != piece) continue;
            auto ptr = std::make_unique<CastleMovement>(this, dir, piece);
            map.emplace(ptr->Destination(), move(ptr));
        }
        return map;
    }

    bool King::IsAttacked() const
    {
        return IsAttacked(Position());
    }

    bool King::IsAttacked(const Vector2Int& position) const
    {
        auto board = Board();
        auto team = Player()->Team();
        for (auto piece : board->Pieces())
        {
            if (piece->Id() != Id())
            {
                if (piece->Player()->Team() != team && piece->PossibleMovements().MappedArea().IncludesPosition(position))
                    return true;
            }
            else if (piece != this)
            {
                auto delta = piece->Position() - Position();
                if (std::abs(delta.x) == 1 && std::abs(delta.y) == 1)
                    return true;
            }
        }
        return false;
    }
}
