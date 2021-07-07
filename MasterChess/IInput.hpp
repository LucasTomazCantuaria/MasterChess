#pragma once
#include "IGameListener.hpp"
#include "IMovement.hpp"

#include <memory>

namespace MasterChess
{
    using std::unique_ptr;

    struct IBoard;

    struct IInput : IGameListener
    {
        virtual ~IInput() = default;
        virtual unique_ptr<IMovement> CreateMovement(IBoard* board, IPlayer* player) = 0;
        virtual unique_ptr<IPiece> SelectPromotion(IPlayer* player) = 0;
    };
}
