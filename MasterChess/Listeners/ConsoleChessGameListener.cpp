#include "ConsoleChessGameListener.hpp"

#include "MasterChess/ChessGame.hpp"
#include "MasterChess/ChessPlayer.hpp"
#include "MasterChess/Game.hpp"
#include "MasterChess/IConsole.hpp"
#include "MasterChess/IPiece.hpp"
#include "MasterChess/Boards/ChessBoard.hpp"
#include "MasterChess/ChessPieces/Bishop.hpp"
#include "MasterChess/ChessPieces/Knight.hpp"
#include "MasterChess/ChessPieces/Queen.hpp"
#include "MasterChess/ChessPieces/Rook.hpp"
#include "Math/Area.hpp"

#include <conio.h>
#include <Windows.h>
#include <fmt/format.h>

#include <cassert>

namespace MasterChess
{
    ConsoleChessGameListener::ConsoleChessGameListener(IConsole* console, const Vector2Int& size) : console(console), size(size), game(nullptr), board(nullptr)
    {

    }

    void ConsoleChessGameListener::OnGameStart(Game* game)
    {
        assert(dynamic_cast<ChessGame*>(game));
        this->game = static_cast<ChessGame*>(game);
        assert(dynamic_cast<ChessBoard*>(game->Board()));
        board = static_cast<ChessBoard*>(game->Board());
        PrintBoard();
    }

    void ConsoleChessGameListener::OnMovementExecution(IMovement* movement)
    {
        PrintBoard();
    }

    Game* ConsoleChessGameListener::CurrentGame()
    {
        return game;
    }

    IPiece* ConsoleChessGameListener::SelectPiece(IPlayer* currentPlayer)
    {
        PrintBoard();
        auto area = board->BoardArea();
        IPiece* piece;
        do SelectPosition();
        while (!area.IncludesPosition(cursor) || !(piece = board->At(cursor)) || piece->Player() != currentPlayer);
        return piece;
    }

    Vector2Int ConsoleChessGameListener::SelectPosition(Area area)
    {
        colored = area;
        PrintBoard();
        do SelectPosition();
        while (!area.IncludesPosition(cursor));
        colored = {};
        PrintBoard();
        return cursor;
    }

    void ConsoleChessGameListener::PrintBoard() const
    {
        console->Clear();

        for (int i = 0; i < 8; ++i)
            console->DrawPixel({ 0, i * size.y + size.y / 2 + 1 }, char('8' - i));

        console->PushOffset({ 1, 0 });

        for (int i = 0; i < 8; ++i)
            console->DrawPixel({ i * size.x + size.x / 2, 0 }, char('A' + i));

        console->PushOffset({ 0, 1 });

        for (int i = 0; i < 8; ++i)
            console->DrawPixel({ 8 * size.x, i * size.y + size.y / 2 }, char('8' - i));

        for (int i = 0; i < 8; ++i)
            console->DrawPixel({ i * size.x + size.x / 2, 8 * size.y }, char('A' + i));

        std::unordered_map<uint32_t, Vector2Int> map;
        for (auto piece : board->Pieces())
            map[piece->Player()->Color()] = piece->Position();

        for (auto [j, check] = std::make_tuple(0, false); j < 8; ++j, check = !check)
        for (int i = 0; i < 8; ++i, check = !check)
        {
            Vector2Int v{ i * size.x, j * size.y }, iv{ i, 7 - j };
            if (check) console->DrawSquare(v, size, '\xB2');
            if (colored.IncludesPosition(iv)) console->DrawSquareBorder(v, size, '\xFE');
            if (auto piece = board->At(iv))
            {
                auto id = piece->Id();
                if (piece->Player()->Team() % 2 != 0)
                    id = char(std::toupper(id));
                console->DrawPixel(v + size / 2, id);
            }
        }

        console->DrawSquareBorder({ cursor.x * size.x + 1, (7 - cursor.y) * size.y + 1 }, size - Vector2Int::One * 2, '*');

        console->PopOffset();
        console->PopOffset();

        auto players = game->Players();
        auto it = std::find(players.begin(), players.end(), game->CurrentPlayer());
        console->DrawString({ 100, 27 }, fmt::format("{} turn", it == players.begin() ? "White" : "Black"));
        console->DrawString({ 100, 28 }, fmt::format("Fen: {}", game->Fen()));

        console->Display();
    }

    void ConsoleChessGameListener::SelectPosition()
    {
        while (true) switch (auto k = _getch())
        {
        case 72:
            cursor += Vector2Int::Up;
            goto clamp;
        case 80:
            cursor += Vector2Int::Down;
            goto clamp;
        case 75:
            cursor += Vector2Int::Left;
            goto clamp;
        case 77:
            cursor += Vector2Int::Right;
        clamp:
            {
                cursor = Vector2Int::Clamp(cursor, Vector2Int::Zero, Vector2Int::One * 7);
                PrintBoard();
            }break;
        case 0x20:
        case 0x0D:
            return;
        default:
            MessageBoxA(nullptr, "Key Pressed!", fmt::format("{:x}", k).c_str(), MB_OK);
        case 224: break;
        }
    }

    std::unique_ptr<IPiece> ConsoleChessGameListener::SelectPromotion(IPlayer* player)
    {
        auto index = 0u;
        auto draw = [&]
        {
            console->Clear();
            console->DrawString({ 0, 0 }, "Select the piece to promote, press Up and Down to select and enter or space to confirm:");
            console->DrawString({ 0, 1 }, fmt::format("{}Promote to Queen.", index == 0 ? '>' : ' '));
            console->DrawString({ 0, 2 }, fmt::format("{}Promote to Knight.", index == 1 ? '>' : ' '));
            console->DrawString({ 0, 3 }, fmt::format("{}Promote to Rook.", index == 2 ? '>' : ' '));
            console->DrawString({ 0, 4 }, fmt::format("{}Promote to Bishop.", index == 3 ? '>' : ' '));
            console->Display();
        };
        draw();
        while (true) switch (_getch())
        {
        case 72:
            --index %= 4;
            goto label;
        case 80:
            ++index %= 4;
        label:
            draw();
            break;
        case 0x20:
        case 0x0D:
            switch (index)
            {
            case 0: return std::make_unique<Queen>(player);
            case 1: return std::make_unique<Knight>(player);
            case 2: return std::make_unique<Rook>(player);
            case 3: return std::make_unique<Bishop>(player);
            }
        }
    }

}
