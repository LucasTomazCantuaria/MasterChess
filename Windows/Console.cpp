#include "Console.hpp"
#include <Windows.h>

#undef DrawText

namespace Windows
{
    Console::Console(int rows, int columns) : Rows(rows), Columns(columns),
        handle(CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, nullptr, CONSOLE_TEXTMODE_BUFFER, nullptr), [](void* handle) { CloseHandle(handle); }),
        buffer((size_t)rows * columns)
    {
        offsets.emplace(Vector2Int::Zero);
        SetConsoleActiveScreenBuffer(handle.get());
        ShowWindow(GetConsoleWindow(), SW_SHOWMAXIMIZED);
    }

    char& Console::operator[](const Math::Vector2Int& pos)
    {
        return buffer[(size_t)pos.y * Columns + pos.x];
    }

    void Console::PushOffset(const Vector2Int& offset)
    {
        offsets.emplace(offsets.top() + offset);
    }

    void Console::PopOffset()
    {
        offsets.pop();
    }

    void Console::Clear()
    {
        fill(buffer.begin(), buffer.end(), '\0');
    }

    void Console::Display() const
    {
        DWORD written;
        for (SHORT i = 0; i < Columns; ++i)
            WriteConsoleOutputCharacterA(handle.get(), buffer.data() + (intptr_t)Columns * i, Columns, COORD{ 0, i }, &written);
    }

    void Console::DrawSquareBorder(const Vector2Int& position, const Vector2Int& size, char d)
    {
        for (int i = 0; i < size.x; ++i)
        {
            DrawPixel(position + Vector2Int::Right * i, d);
            DrawPixel(position + Vector2Int::Up * (size.y - 1) + Vector2Int::Right * i, d);
        }

        for (int i = 1; i + 1 < size.y; ++i)
        {
            DrawPixel(position + Vector2Int::Up * i, d);
            DrawPixel(position + Vector2Int::Right * (size.x - 1) + Vector2Int::Up * i, d);
        }
    }

    void Console::DrawSquare(const Vector2Int& position, const Vector2Int& size, char d)
    {
        for (int i = 0; i < size.x; ++i)
        for (int j = 0; j < size.y; ++j)
            DrawPixel(position + Vector2Int{ i, j }, d);
    }

    void Console::DrawPixel(const Vector2Int& position, char d)
    {
        operator[](position + offsets.top()) = d;
    }

    void Console::DrawText(const Vector2Int& position, std::string_view sv)
    {
        auto p = position;
        for (auto d : sv)
        {
            DrawPixel(p, d);
            p += Vector2Int::Right;
        }
    }
}
