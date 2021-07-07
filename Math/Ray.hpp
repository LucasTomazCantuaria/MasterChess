#pragma once
#include "Vector2Int.hpp"

namespace Math
{
    struct Ray
    {
        /// <summary>
        /// Posição inicial do raio.
        /// </summary>
        Vector2Int StartPosition;

        /// <summary>
        /// Direção do raio.
        /// </summary>
        Vector2Int Direction;

        /// <summary>
        /// Distância total do raio.
        /// </summary>
        int Lenght;

        /// <summary>
        /// Distância inicial que deve ser ignorada no começo do raio.
        /// </summary>
        int StartLenght;

        Ray(const Vector2Int& startPosition, const Vector2Int& direction, int lenght, int startLenght = 0);

        /// <summary>
        /// Checa se um raio traca uma posição.
        /// </summary>
        /// <param name="position">Posição a ser testada.</param>
        /// <returns>Verdadeiro caso trace e falso caso contrário.</returns>
        bool TracesPosition(const Vector2Int& position) const;

        /// <summary>
        /// Função para obter o valor de uma posição em uma distância específica do raio.
        /// </summary>
        /// <param name="n">Distância desejada.</param>
        /// <returns>Um vetor com as coordenadas da posição naquela distância.</returns>
        Vector2Int Position(int n) const;

    };
}