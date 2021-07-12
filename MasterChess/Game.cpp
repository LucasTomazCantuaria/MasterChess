#include "Game.hpp"

#include "IBoard.hpp"
#include "IMovement.hpp"
#include "IPiece.hpp"
#include "IPlayer.hpp"
#include "IInput.hpp"

#include <future>
#include <execution>

#include <cassert>
#include <iterator>

using namespace std::chrono_literals;

namespace MasterChess
{
    Game::Game(unique_ptr<IBoard> board, vector<unique_ptr<IPlayer>> players, vector<unique_ptr<IPiece>> pieces)
    {
        SetBoard(move(board));
        for (auto& player : players)
            AddPlayer(move(player));
        for (auto& piece : pieces)
            AddPiece(move(piece));
    }

    bool Game::ValidateMovement(unique_ptr<IMovement>& movement)
    {
        return true;
    }

    unique_ptr<Game::Result> Game::IsGameOver(IPlayer* currentPlayer)
    {
        return nullptr;
    }

    Game::Result Game::Play()
    {
        listener.OnGameStart(this);
        size_t i = 0;
        while (true)
        {
            if (auto result = IsGameOver(players[i].get()))
                return *result;
            auto player = players[i].get();
            auto input = player->Input();
            unique_ptr<IMovement> movement;
            do movement = input->CreateMovement(board.get(), player);
            while (!ValidateMovement(movement));
            ExecuteMovement(move(movement));
            ++i %= players.size();
        }
    }

    void Game::AddListener(IGameListener* listener)
    {
        this->listener.AddListener(listener);
    }

    vector<IPlayer*> Game::Players() const
    {
        vector<IPlayer*> vec;
        transform(players.begin(), players.end(), back_inserter(vec), [](auto& ptr) { return ptr.get(); });
        return vec;
    }

    IPlayer* Game::AddPlayer(unique_ptr<IPlayer> player)
    {
        return players.emplace_back(move(player)).get();
    }

    vector<IPiece*> Game::Pieces() const
    {
        vector<IPiece*> vec;
        transform(pieces.begin(), pieces.end(), back_inserter(vec), [](auto& ptr) { return ptr.get(); });
        return vec;
    }

    IPiece* Game::AddPiece(unique_ptr<IPiece> piece)
    {
        return pieces.emplace_back(move(piece)).get();
    }

    void Game::ExecuteMovement(unique_ptr<IMovement> movement)
    {
        SimulateMovementExecution(movement);
        listener.OnMovementExecution(LastMovement());
    }

    void Game::UndoLastMovement()
    {
        unique_ptr<IMovement> movement;
        SimulateMovementUndo(movement);
        //listener->OnMovementUndo(movement.get());
    }

    void Game::SimulateMovementExecution(unique_ptr<IMovement>& movement)
    {
        movement->Execute();
        movements.emplace_back(move(movement));
    }

    void Game::SimulateMovementUndo(unique_ptr<IMovement>& movement)
    {
        assert(!movements.empty());
        LastMovement()->Undo();
        movement = move(movements.back());
        movements.resize(movements.size() - 1);
    }

    void Game::SetBoard(unique_ptr<IBoard> board)
    {
        assert(!this->board && board);
        AddListener(board.get());
        this->board = move(board);
    }

    void Game::GameBroadcastListener::OnGameStart(Game* game)
    {
        for_each(std::execution::par_unseq, listeners.begin(), listeners.end(), [=](IGameListener* listener) { listener->OnGameStart(game); });
    }

    void Game::GameBroadcastListener::OnMovementExecution(IMovement* movement)
    {
        for_each(std::execution::par_unseq, listeners.begin(), listeners.end(), [=](IGameListener* listener) { listener->OnMovementExecution(movement); });
    }

    void Game::GameBroadcastListener::AddListener(IGameListener* listener)
    {
        listeners.emplace_back(listener);
    }
}
