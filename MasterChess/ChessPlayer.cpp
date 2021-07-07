#include "ChessPlayer.hpp"
#include "IPiece.hpp"

namespace MasterChess
{
    unique_ptr<IMovement> ChessPlayer::TwoStepsInput::CreateMovement(IBoard* board, IPlayer* player)
    {
        while (true)
        {
            auto piece = SelectPiece([=](auto p) { return p->Player() == player; });
            auto start = piece->Position();
            auto moves = piece->PossibleMovements();
            auto movArea = moves.MappedArea() | start;
            board->PushColors();
            board->LightenArea(movArea, player->Color());
            auto end = SelectPosition([&](auto& v) { return movArea.IncludesPosition(v); });
            board->PopColors();
            if (start != end)
                return move(moves[end]);
        }
    }

    ChessPlayer::ChessPlayer(IInput* input, int team, uint32_t color) : input(input), team(team), color(color)
    {

    }
    
}
