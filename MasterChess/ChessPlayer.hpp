#pragma once
#include "IBoard.hpp"
#include "IPlayer.hpp"
#include "Math/Vector2Int.hpp"
#include "IInput.hpp"

#include <memory>
#include <functional>

namespace MasterChess
{
    struct IBoard;

    struct ChessPlayer final : IPlayer
    {
        struct TwoStepsInput : IInput
        {
            virtual IPiece* SelectPiece(std::function<bool(IPiece*)> filter) = 0;
            virtual Vector2Int SelectPosition(std::function<bool(const Vector2Int&)> filter) = 0;
            unique_ptr<IMovement> CreateMovement(IBoard* board, IPlayer* player) override;
            void OnGameStart(Game* game) override {  }
        };
        ChessPlayer(IInput* input, int team, uint32_t color);
        IInput* Input() const override { return input; }
        int Team() const override { return team; }
        uint32_t Color() const override { return color; }
    private:
        IInput* input;
        int team;
        uint32_t color;
    };
}
