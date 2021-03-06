#include "Pawn.hpp"

#include <cassert>

#include "Queen.hpp"
#include "MasterChess/Game.hpp"
#include "MasterChess/IBoard.hpp"
#include "MasterChess/IInput.hpp"
#include "MasterChess/IPlayer.hpp"

namespace MasterChess
{
    Pawn::DoubleMovement::DoubleMovement(Pawn* piece, const Vector2Int& destination) : Movement(piece, destination)
    {

    }

    Pawn* Pawn::DoubleMovement::Piece() const
    {
        return (Pawn*)Movement::Piece();
    }

    Pawn::EnPassantMovement::EnPassantMovement(Pawn* pawn, const Vector2Int& destination, Pawn* capturedPawn) : CaptureMovement(pawn, destination, capturedPawn)
    {

    }

    Pawn* Pawn::EnPassantMovement::Piece() const
    {
        return (Pawn*)CaptureMovement::Piece();
    }

    Pawn::PromotionMovement::PromotionMovement(ChessPiece* piece, const Vector2Int& destination, unique_ptr<IMovement> innerMovement) :
        Movement(piece, destination), innerMovement(move(innerMovement)), promotedPiece(nullptr), board(piece->Board())
    {

    }

    void Pawn::PromotionMovement::Execute()
    {
        if (!promotedPiece)
        {
            auto piece = Piece();
            auto game = piece->CurrentGame();
            promotedPiece = game->RequestPromotion(piece);
        }
        innerMovement->Execute();
        board->RemovePiece(Piece());
        if (promotedPiece)
            board->AddPiece(promotedPiece, Destination());
    }

    void Pawn::PromotionMovement::Undo()
    {
        assert(innerMovement);
        auto piece = Piece();
        if (promotedPiece)
            board->RemovePiece(promotedPiece);
        board->AddPiece(piece, Destination());
        innerMovement->Undo();
    }

    Pawn::Pawn(IPlayer* player, const Vector2Int& direction, Area promotionArea, bool disableDoubleMovement) :
        ChessPiece(player), direction(direction),
        promotionArea(std::move(promotionArea)),
        disableDoubleMovement(disableDoubleMovement)
    {
        
    }

    char Pawn::Id() const
    {
        return 'p';
    }

    MovementMap Pawn::PossibleMovements()
    {
        MovementMap map;
        auto position = Position();
        if (auto forw = position + direction; IsEmpty(forw))
        {
            map.emplace(forw, std::make_unique<Movement>(this, forw));
            forw += direction;
            if (!disableDoubleMovement && MovementCount() == 0 && IsEmpty(forw))
                map.emplace(forw, std::make_unique<DoubleMovement>(this, forw));
        }
        auto area = Board()->BoardArea();
        if (auto left = position + direction - direction.Perpendicular(); area.IncludesPosition(left) && IsCapturable(left))
            map.emplace(left, CreateMovement(left));
        if (auto right = position + direction + direction.Perpendicular(); area.IncludesPosition(right) && IsCapturable(right))
            map.emplace(right, CreateMovement(right));
        if (auto last = CurrentGame()->LastMovement())
        if (auto mov = dynamic_cast<DoubleMovement*>(last))
        if (auto pawn = mov->Piece(); IsCapturable(pawn))
        if (auto pos = pawn->Position(); pos == position + direction.Perpendicular() || pos == position - direction.Perpendicular())
        {
            pos += direction;
            map.emplace(pos, std::make_unique<EnPassantMovement>(this, pos, pawn));
        }
        for (auto& [v, m] : map) if (promotionArea.IncludesPosition(v))
            map[v] = std::make_unique<PromotionMovement>(this, v, move(m));
        return map;
    }

}
