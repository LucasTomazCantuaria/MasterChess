#pragma once
#include <memory>

namespace MasterChess
{
    using std::unique_ptr;

    struct IBoard;
    struct IPlayer;
    struct IMovement;
    struct IPiece;

    struct IInput
    {
        virtual ~IInput() = default;
        virtual unique_ptr<IMovement> CreateMovement(IBoard* board, IPlayer* player) = 0;
        virtual unique_ptr<IPiece> SelectPromotion(IPlayer* player) = 0;
    };
}
