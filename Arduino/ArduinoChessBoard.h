#pragma once
#include "ArduinoExported.h"
#include "MasterChess/Boards/ConsoleChessBoard.hpp"

namespace Arduino
{
    using Math::Vector2Int;
    using MasterChess::IPiece;

    struct ArduinoChessBoard final : MasterChess::ConsoleChessBoard
    {
        ArduinoChessBoard(MasterChess::IConsole* console, ArduinoExported* arduino);

        void PrintBoard() const override;

        IPiece* SelectPiece(std::function<bool(IPiece*)> filter) override;

        Vector2Int SelectPosition(std::function<bool(const Vector2Int&)> filter) override;

        std::unique_ptr<MasterChess::IMovement> CreateMovement(IBoard* board, MasterChess::IPlayer* player) override;

        void AddPiece(IPiece* piece, const Vector2Int& position) override;

        void RepositionPiece(IPiece* piece, const Vector2Int& position) override;

        void RemovePiece(IPiece* piece) override;

        void RequestMovement(IPiece* piece, const Vector2Int& position) const;

        void CheckBoard() const;

    private:
        ArduinoExported* arduino;
        IPiece* selectedPiece;
        bool ignoreLighten;
        void Lighten(uint32_t color, Matrix8x8 mat) const;
    };
}
