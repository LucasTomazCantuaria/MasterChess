#pragma once
#include "MasterChess/IConsole.hpp"
#include "Math/Vector2Int.hpp"

#include <memory>
#include <stack>
#include <string_view>
#include <vector>

namespace Windows
{
    using Math::Vector2Int;

    struct Console : MasterChess::IConsole
    {
        /// <summary>
        /// Linhas do buffer.
        /// </summary>
        const int Rows;

        /// <summary>
        /// Colunas do buffer.
        /// </summary>
        const int Columns;

        Console(int rows, int columns);

        ~Console();

        /// <summary>
        /// Obtem uma referência para um dos digitos do buffer.
        /// </summary>
        /// <param name="pos">Um vetor com a posição no buffer.</param>
        /// <returns>Uma referência do tipo char</returns>
        char& operator[](const Vector2Int& pos);

        void PushOffset(const Vector2Int& offset) override;
        void PopOffset() override;
        void Clear() override;
        void Display() const override;
        void DrawSquareBorder(const Vector2Int& position, const Vector2Int& size, char d) override;
        void DrawSquare(const Vector2Int& position, const Vector2Int& size, char d) override;
        void DrawPixel(const Vector2Int& position, char d) override;
        void DrawString(const Vector2Int& position, std::string_view sv) override;

    private:
        void* stdHandle;
        std::unique_ptr<void, void(*)(void*)> handle;
        std::vector<char> buffer;
        std::stack<Vector2Int> offsets;
    };

}
