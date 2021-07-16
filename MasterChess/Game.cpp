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
    Game::Game() : currentPlayer(nullptr), isSimulating(false)
    {

    }

    Game::Game(unique_ptr<IBoard> board, vector<unique_ptr<IPlayer>> players, vector<unique_ptr<IPiece>> pieces) : currentPlayer(nullptr)
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

    unique_ptr<GameResult> Game::IsGameOver(IPlayer* currentPlayer)
    {
        return nullptr;
    }

    GameResult Game::Play()
    {
        listener.OnGameStart(this);
        size_t i = 0;
        while (true)
        {
            currentPlayer = players[i].get();
            if (auto result = IsGameOver(currentPlayer))
            {
                listener.OnGameOver(result.get());
                return *result;
            }
            unique_ptr<IMovement> movement;
            do movement = currentPlayer->Input()->CreateMovement(board.get(), currentPlayer);
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
        if (players.empty())
            currentPlayer = player.get();
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

    IPiece* Game::RequestPromotion(IPiece* piece)
    {
        assert(currentPlayer == piece->Player());
        if (isSimulating)
            return nullptr;
        auto ptr = pieces.emplace_back(currentPlayer->Input()->SelectPromotion(currentPlayer)).get();
        ptr->OnGameStart(this);
        return ptr;
    }

    void Game::ExecuteMovement(unique_ptr<IMovement> movement)
    {
        movement->Execute();
        movements.emplace_back(move(movement));
        listener.OnMovementExecution(LastMovement());
    }

    void Game::UndoLastMovement()
    {
        assert(!movements.empty());
        LastMovement()->Undo();
        auto movement = move(movements.back());
        movements.resize(movements.size() - 1);
        listener.OnMovementUndo(movement.get());
    }

    void Game::SimulateMovementExecution(unique_ptr<IMovement>& movement)
    {
        isSimulating = true;
        movement->Execute();
        movements.emplace_back(move(movement));
        isSimulating = false;
    }

    void Game::SimulateMovementUndo(unique_ptr<IMovement>& movement)
    {
        assert(!movements.empty());
        isSimulating = true;
        LastMovement()->Undo();
        movement = move(movements.back());
        movements.resize(movements.size() - 1);
        isSimulating = false;
    }

    void Game::SetBoard(unique_ptr<IBoard> board)
    {
        assert(!this->board && board);
        AddListener(board.get());
        this->board = move(board);
    }

    template<class Range, class Fn, class... Args>
    static void pfor(Range&& rng, Fn fn, Args&&... args)
    {
        for_each(std::execution::par_unseq, std::begin(rng), std::end(rng), [=](IGameListener* listener) { (listener->*fn)(args...); });
    }

    void Game::GameBroadcastListener::OnGameStart(Game* game)
    {
        pfor(listeners, &IGameListener::OnGameStart, game);
    }

    void Game::GameBroadcastListener::OnMovementExecution(IMovement* movement)
    {
        pfor(listeners, &IGameListener::OnMovementExecution, movement);
    }

    void Game::GameBroadcastListener::OnMovementUndo(IMovement* movement)
    {
        pfor(listeners, &IGameListener::OnMovementUndo, movement);
    }

    void Game::GameBroadcastListener::OnGameOver(GameResult* result)
    {
        pfor(listeners, &IGameListener::OnGameOver, result);
    }

    void Game::GameBroadcastListener::AddListener(IGameListener* listener)
    {
        listeners.emplace_back(listener);
    }
}
