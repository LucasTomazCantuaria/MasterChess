#include "TwoStepsInput.hpp"

#include "MasterChess/IBoard.hpp"
#include "MasterChess/Game.hpp"

namespace MasterChess
{
    unique_ptr<IMovement> TwoStepsInput::CreateMovement(IBoard* board, IPlayer* player)
    {
        auto game = CurrentGame();
        while (true)
        {
            auto piece = SelectPiece(player);
            auto start = piece->Position();
            auto moves = piece->PossibleMovements();
            for (auto it = moves.begin(); it != moves.end();)
            {
                auto& [pos, mov] = *it;
                if (!game->ValidateMovement(mov))
                    it = moves.erase(it);
                else ++it;
            }
            auto end = SelectPosition(moves.MappedArea() | start);
            if (start != end)
                return std::move(moves[end]);
        }
    }
}
