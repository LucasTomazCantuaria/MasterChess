#pragma once
#include "IGameListener.hpp"

#include <vector>
#include <memory>
#include <string>

namespace MasterChess
{
    using std::string;
    using std::vector;
    using std::unique_ptr;

    struct IMovement;
    struct ChessPlayer;
    struct IPlayer;
    struct IPiece;
    struct IBoard;

    struct Game
    {
        struct Result
        {
            vector<IPlayer*> Winners;
            vector<IPlayer*> Losers;
            IBoard* Board;
            vector<IMovement*> Movements;
            string Message;
        };

        Game(unique_ptr<IBoard> board, vector<unique_ptr<IPlayer>> players, vector<unique_ptr<IPiece>> pieces);

        Game() = default;

        virtual bool ValidateMovement(unique_ptr<IMovement>& movement);

        virtual unique_ptr<Result> IsGameOver(IPlayer* currentPlayer);

        Result Play();

        IBoard* Board() const { return board.get(); }

        virtual void SetBoard(unique_ptr<IBoard> board);

        IMovement* LastMovement() { return !movements.empty() ? movements.back().get() : nullptr; }

        void AddListener(IGameListener* listener);

        vector<IPlayer*> Players() const;

        IPlayer* Player(int i) const { return players[i].get(); }

        virtual IPlayer* AddPlayer(unique_ptr<IPlayer> player);

        vector<IPiece*> Pieces() const;

        virtual IPiece* AddPiece(unique_ptr<IPiece> piece);

    protected:

        void ExecuteMovement(unique_ptr<IMovement> movement);

        void UndoLastMovement();

        void SimulateMovementExecution(unique_ptr<IMovement>& movement);

        void SimulateMovementUndo(unique_ptr<IMovement>& movement);

    private:
        unique_ptr<IBoard> board;
        vector<unique_ptr<IPlayer>> players;
        vector<unique_ptr<IPiece>> pieces;
        vector<unique_ptr<IMovement>> movements;
        struct GameBroadcastListener : IGameListener
        {
            void OnGameStart(Game* game) override;
            void OnMovementExecution(IMovement* movement) override;
            void AddListener(IGameListener* listener);
        private:
            std::vector<IGameListener*> listeners;
        }listener;
    };
    
}
