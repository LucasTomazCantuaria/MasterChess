#include "Game.hpp"
#include "ChessPlayer.hpp"
#include "IBoard.hpp"
#include "IMovement.hpp"
#include "IPiece.hpp"
#include "IPlayer.hpp"

using namespace std::chrono_literals;
using std::chrono::duration_cast;

namespace MasterChess
{
    Game::Movement::Movement(IPlayer* player, unique_ptr<IMovement> move, seconds time): Player(player),
                                                                                         Move(std::move(move)),
                                                                                         Time(time)
    {

    }

    Game::Game(unique_ptr<IBoard> board, vector<unique_ptr<IPlayer>> players, vector<unique_ptr<IPiece>> pieces) :
        board(move(board)),
        players(move(players)),
        pieces(move(pieces))
    {

    }

    bool Game::ValidateMovement(IMovement* movement)
    {
        movement->Execute();
        return true;
    }

    bool Game::IsGameOver() const
    {
        return false;
    }

    Game::Result Game::Play()
    {
        for (auto&& ptr : Players())
            ptr->Input()->OnGameStart(this);
        Board()->OnGameStart(this);
        auto&& now = std::chrono::high_resolution_clock::now;
        size_t i = 0;
        while (!IsGameOver())
        {
            auto player = players[i].get();
            auto input = player->Input();
            unique_ptr<IMovement> movement;
            auto t = now();
            do movement = input->CreateMovement(board.get(), player);
            while (!ValidateMovement(movement.get()));
            auto delta = now() - t;
            movements.emplace_back(player, move(movement), duration_cast<seconds>(delta));
            ++i %= players.size();
        }
        return {};
    }



}
