#pragma once
#include "Math/Vector2Int.hpp"

#include <string_view>

namespace MasterChess
{
    using Math::Vector2Int;

    struct IConsole
    {
        /// <summary>
        /// Adiciona um valor na stack.
        /// Após o seu uso todos os comandos para desenhar são somados a essa posição até que a PopOffset seja chamada.
        /// </summary>
        /// <param name="offset">Um vetor com o valor a ser adicionado ao offset.</param>
        /// <remarks>
        ///	Offsets subsequentes são adicionados cumultativamente.
        ///	A função PopOffset deve ser chamada logo em seguida que o offset não for mais nescessário.
        /// </remarks>
        virtual void PushOffset(const Vector2Int& offset) = 0;

        /// <summary>
        /// Retira o último offset que foi colocado na pilha efetivamente o descartando.
        /// </summary>
        virtual void PopOffset() = 0;

        /// <summary>
        /// Limpa o buffer da tela.
        /// </summary>
        virtual void Clear() = 0;

        /// <summary>
        /// Passa o conteúdo do buffer para a tela.
        /// </summary>
        virtual void Display() const = 0;

        /// <summary>
        /// Desenha a borda de um quadrado no buffer.
        /// </summary>
        /// <param name="position">Posição superior esquerda do quadrado.</param>
        /// <param name="size">Tamanho do quadrado.</param>
        /// <param name="d">Digito a ser usado no preenchimento</param>
        virtual void DrawSquareBorder(const Vector2Int& position, const Vector2Int& size, char d) = 0;

        /// <summary>
        /// Desenha um quadrado no buffer.
        /// </summary>
        /// <param name="position">Posição superior esquerda do quadrado.</param>
        /// <param name="size">Tamanho do quadrado.</param>
        /// <param name="d">Digito a ser usado no preenchimento</param>
        virtual void DrawSquare(const Vector2Int& position, const Vector2Int& size, char d) = 0;

        /// <summary>
        /// Desenha um "pixel" no buffer.
        /// </summary>
        /// <param name="position">Posição a ser preenchida.</param>
        /// <param name="d">Digito a ser usado.</param>
        virtual void DrawPixel(const Vector2Int& position, char d) = 0;

        /// <summary>
        /// Desenha um texto na tela.
        /// </summary>
        /// <param name="position">Posição do primeiro caractér.</param>
        /// <param name="sv">Texto a ser desenhado.</param>
        virtual void DrawString(const Vector2Int& position, std::string_view sv) = 0;
    };

}
