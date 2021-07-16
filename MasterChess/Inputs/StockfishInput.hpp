#pragma once
#include "MasterChess/IGameListener.hpp"
#include "MasterChess/IInput.hpp"

#include <string>

namespace MasterChess
{
    struct King;
    struct ChessGame;
    struct ChessBoard;

    struct StockFishInput final : IInput, IGameListener
    {
        explicit StockFishInput();

        StockFishInput(const StockFishInput&) = delete;
        StockFishInput(StockFishInput&&) = delete;
        StockFishInput& operator=(const StockFishInput&) = delete;
        StockFishInput& operator=(StockFishInput&&) = delete;

        ~StockFishInput() override;

        void OnGameStart(Game* game) override;

        void OnMovementExecution(IMovement* movement) override;

        unique_ptr<IMovement> CreateMovement(IBoard*, IPlayer* player) override;

        unique_ptr<IPiece> SelectPromotion(IPlayer* player) override;
    private:
        struct StockFish;
        ChessGame* game;
        ChessBoard* board;
        IPlayer* white;
        std::unique_ptr<StockFish> fish;
        King* whiteKing;
        King* blackKing;
        bool whiteLeftRook, whiteRightRook, blackLeftRook, blackRightRook;
        char nextPromotion;
        std::string ToFenString(IPlayer* currentPlayer) const;
    };
}
