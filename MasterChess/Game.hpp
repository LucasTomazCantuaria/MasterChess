#pragma once
#include "IGameListener.hpp"

#include <vector>
#include <chrono>
#include <memory>
#include <string>

namespace MasterChess
{
    using std::string;
    using std::vector;
    using std::unique_ptr;
    using std::chrono::seconds;
    struct IMovement;
    struct ChessPlayer;
    struct IPlayer;
    struct IPiece;
    struct IBoard;

    struct Game
    {
        struct Player
        {
            IPlayer* PlayerReference;
            IPlayer* operator->() const { return PlayerReference; }
        };

        struct Movement
        {
            IPlayer* Player;
            unique_ptr<IMovement> Move;
            seconds Time;
            Movement(IPlayer* player, unique_ptr<IMovement> move, seconds time);
        };

        struct Result
        {
            vector<IPlayer*> Winners;
            vector<IPlayer*> Losers;
            IBoard* Board;
            vector<Movement> Movements;
            string Message;
            seconds Time;
        };

        Game(unique_ptr<IBoard> board, vector<unique_ptr<IPlayer>> players, vector<unique_ptr<IPiece>> pieces);

        virtual bool ValidateMovement(IMovement* movement);

        virtual bool IsGameOver() const;

        Result Play();

        IBoard* Board() const { return board.get(); }

        const vector<unique_ptr<IPlayer>>& Players() const { return players; }

        const vector<unique_ptr<IPiece>>& Pieces() const { return pieces; }

        Movement* LastMovement() { return !movements.empty() ? &movements.back() : nullptr; }

        void AddListener(IGameListener* listener);

        IPlayer* Player(int i) const { return players[i].get(); }

    private:
        unique_ptr<IBoard> board;
        vector<unique_ptr<IPlayer>> players;
        vector<unique_ptr<IPiece>> pieces;
        vector<Movement> movements;
        GameBroadcastListener listener;
    };
    
}
