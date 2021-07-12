#pragma once
#include "MasterChess/IInput.hpp"
#include "Math/Area.hpp"
#include "Math/Vector2Int.hpp"

namespace MasterChess
{
    using Math::Vector2Int;

    struct IPiece;
    struct IBoard;
    struct IPlayer;
    struct Game;

    struct TwoStepsInput : IInput
    {
        virtual Game* CurrentGame() = 0;
        virtual IPiece* SelectPiece(IPlayer* currentPlayer) = 0;
        virtual Vector2Int SelectPosition(Math::Area area) = 0;
        unique_ptr<IMovement> CreateMovement(IBoard* board, IPlayer* player) final;
    };
}
