#pragma once
#include "Matrix8x8.hpp"
#include "MasterChess/ChessGame.hpp"
#include "MasterChess/ChessPlayer.hpp"
#include "MasterChess/IGameListener.hpp"
#include "MasterChess/IInput.hpp"
#include "MasterChess/Inputs/TwoStepsInput.hpp"

namespace Arduino
{
    using std::unique_ptr;
    using Math::Vector2Int;
    using MasterChess::IPiece;

    struct ArduinoExported;

    struct ArduinoChessBoard final : MasterChess::IGameListener, MasterChess::TwoStepsInput
    {
        struct PlayRequester final : IInput
        {
            PlayRequester(ArduinoChessBoard* board, IInput* underlyingInput);
            unique_ptr<MasterChess::IMovement> CreateMovement(MasterChess::IBoard*, MasterChess::IPlayer* player) override;
            unique_ptr<IPiece> SelectPromotion(MasterChess::IPlayer* player) override;
        private:
            ArduinoChessBoard* board;
            IInput* underlyingInput;
        };

        ArduinoChessBoard(ArduinoExported* arduino, IInput* piecePromotionInput);

        IPiece* SelectPiece(MasterChess::IPlayer* currentPlayer) override;

        Vector2Int SelectPosition(Math::Area area) override;

        unique_ptr<IPiece> SelectPromotion(MasterChess::IPlayer* player) override;
        
        void RequestMovement(IPiece* piece, const Vector2Int& position) const;
        
        void CheckBoard() const;
        
        void OnGameStart(MasterChess::Game* game) override;

        void OnMovementExecution(MasterChess::IMovement* movement) override;

        MasterChess::ChessGame* CurrentGame() override;
        
    private:
        ArduinoExported* arduino;
        MasterChess::ChessGame* game;
        IPiece* selectedPiece;
        IInput* piecePromotionInput;
        void Lighten(uint32_t color, Matrix8x8 mat) const;
        void PrintBoard() const;        
    };
}
