#pragma once
#include "IPlayer.hpp"

namespace MasterChess
{
    struct IBoard;

    struct ChessPlayer final : IPlayer
    {
        ChessPlayer(IInput* input, int team, uint32_t color) : input(input), team(team), color(color) {  }
        IInput* Input() const override { return input; }
        int Team() const override { return team; }
        uint32_t Color() const override { return color; }
    private:
        IInput* input;
        int team;
        uint32_t color;
    };
}
