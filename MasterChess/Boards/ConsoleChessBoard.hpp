#pragma once
#include "ChessBoard.hpp"
#include "MasterChess/ChessPlayer.hpp"

namespace MasterChess
{
    using MasterChess::IPiece;
    using Math::Area;
    using Math::Vector2Int;

    struct IConsole;

    struct ConsoleChessBoard : ChessBoard, ChessPlayer::TwoStepsInput
    {
        explicit ConsoleChessBoard(IConsole* console, const Vector2Int& size = {12, 7});

        void AddPiece(IPiece* piece, const Vector2Int& position) override;

        void RemovePiece(IPiece* piece) override;

        void RepositionPiece(IPiece* piece, const Vector2Int& position) override;

        void LightenArea(const Area& area, uint32_t color) override;

        Vector2Int SelectPosition(std::function<bool(const Vector2Int&)> filter) override;

        IPiece* SelectPiece(std::function<bool(IPiece*)> filter) override;

        virtual void PrintBoard() const;

        void SelectPosition();

        void PopColors() override;

        void BeginUpdate() override;

        void EndUpdate() override;

        std::unique_ptr<IPiece> SelectPromotion(IPlayer* player) override;

        void OnGameStart(Game* game) override { ChessBoard::OnGameStart(game); }

    private:
        IConsole* console;
        Vector2Int size;
        Vector2Int cursor;
        bool ignoreUpdate;
        void Update();
    };

}
