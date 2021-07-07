#include "ConsoleChessBoard.hpp"

#include "Math/Area.hpp"

#include "MasterChess/ChessPlayer.hpp"
#include "MasterChess/IConsole.hpp"
#include "MasterChess/IPiece.hpp"

#include "MasterChess/ChessPieces/Bishop.hpp"
#include "MasterChess/ChessPieces/Knight.hpp"
#include "MasterChess/ChessPieces/Queen.hpp"
#include "MasterChess/ChessPieces/Rook.hpp"

#include <conio.h>
#include <Windows.h>
#include <fmt/format.h>

#include <cassert>

#undef DrawText

namespace MasterChess
{
    ConsoleChessBoard::ConsoleChessBoard(IConsole* console, const Vector2Int& size) : console(console), size(size), ignoreUpdate(false)
    {

    }

    void ConsoleChessBoard::AddPiece(IPiece* piece, const Vector2Int& position)
    {
        ChessBoard::AddPiece(piece, position);
        Update();
    }

    void ConsoleChessBoard::LightenArea(const Area& area, uint32_t color)
    {
        ChessBoard::LightenArea(area, color);
        Update();
    }

    void ConsoleChessBoard::RemovePiece(IPiece* piece)
    {
        ChessBoard::RemovePiece(piece);
        Update();
    }

    void ConsoleChessBoard::RepositionPiece(IPiece* piece, const Vector2Int& position)
    {
        ChessBoard::RepositionPiece(piece, position);
        Update();
    }

    Vector2Int ConsoleChessBoard::SelectPosition(std::function<bool(const Vector2Int&)> filter)
    {
        do SelectPosition();
        while (!filter(cursor));
        return cursor;
    }

    IPiece* ConsoleChessBoard::SelectPiece(std::function<bool(IPiece*)> filter)
    {
        auto area = BoardArea();
        IPiece* piece;
        do SelectPosition();
        while (!area.IncludesPosition(cursor) || !(piece = At(cursor)) || !filter(piece));
        return piece;
    }

    void ConsoleChessBoard::PrintBoard() const
    {
        console->Clear();

        for (int i = 0; i < 8; ++i)
            console->DrawPixel({0, i * size.y + size.y / 2 + 1}, char('1' + i));

        console->PushOffset({1, 0});

        for (int i = 0; i < 8; ++i)
            console->DrawPixel({i * size.x + size.x / 2, 0}, char('A' + i));

        console->PushOffset({0, 1});

        for (int i = 0; i < 8; ++i)
            console->DrawPixel({8 * size.x, i * size.y + size.y / 2}, char('1' + i));

        for (int i = 0; i < 8; ++i)
            console->DrawPixel({i * size.x + size.x / 2, 8 * size.y}, char('A' + i));

        Area colored;
        for (auto [color, v] : Colors()) if (color != 0 && color != -1)
            colored |= v;

        for (auto [j, check] = std::make_tuple(0, false); j < 8; ++j, check = !check)
        for (int i = 0; i < 8; ++i, check = !check)
        {
            Vector2Int v{i * size.x, j * size.y}, iv{i, 7 - j};
            if (check) console->DrawSquare(v, size, '\xB2');
            if (colored.IncludesPosition(iv)) console->DrawSquareBorder(v, size, '\xFE');
            if (auto piece = At(iv))
            {
                auto id = piece->Id();
                if (piece->Player()->Team() % 2 != 0)
                    id = char(std::toupper(id));
                console->DrawPixel(v + size / 2, id);
            }
        }

        console->DrawSquareBorder({cursor.x * size.x + 1, (7 - cursor.y) * size.y + 1}, size - Vector2Int::One * 2, '*');

        console->PopOffset();
        console->PopOffset();

        console->Display();
    }

    void ConsoleChessBoard::SelectPosition()
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
            Update();
        }break;
        case 0x20:
        case 0x0D:
            return;
        default:
            MessageBoxA(nullptr, "Key Pressed!", fmt::format("{:x}", k).c_str(), MB_OK);
        case 224: break;
        }
    }

    void ConsoleChessBoard::PopColors()
    {
        ChessBoard::PopColors();
        Update();
    }

    void ConsoleChessBoard::BeginUpdate()
    {
        assert(!ignoreUpdate);
        ignoreUpdate = true;
    }

    void ConsoleChessBoard::EndUpdate()
    {
        assert(ignoreUpdate);
        ignoreUpdate = false;
        Update();
    }

    std::unique_ptr<IPiece> ConsoleChessBoard::SelectPromotion(MasterChess::IPlayer* player)
    {
        auto index = 0u;
        auto draw = [&]
        {
            console->Clear();
            console->DrawText({ 0, 0 }, "Select the piece to promote, press Up and Down to select and enter or space to confirm:");
            console->DrawText({ 0, 1 }, fmt::format("{}Promote to Queen.", index == 0 ? '>' : ' '));
            console->DrawText({ 0, 2 }, fmt::format("{}Promote to Knight.", index == 1 ? '>' : ' '));
            console->DrawText({ 0, 3 }, fmt::format("{}Promote to Rook.", index == 2 ? '>' : ' '));
            console->DrawText({ 0, 4 }, fmt::format("{}Promote to Bishop.", index == 3 ? '>' : ' '));
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
            case 0: return std::make_unique<MasterChess::Queen>(player);
            case 1: return std::make_unique<MasterChess::Knight>(player);
            case 2: return std::make_unique<MasterChess::Rook>(player);
            case 3: return std::make_unique<MasterChess::Bishop>(player);
            }
        }
    }

    void ConsoleChessBoard::Update()
    {
        if (ignoreUpdate) return;
        PrintBoard();
    }
}
