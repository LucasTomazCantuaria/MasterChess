#pragma once
#include "Vector2Int.hpp"
#include "Ray.hpp"

#include <unordered_set>
#include <vector>

namespace Math
{
    using std::unordered_set;
    using std::vector;

    /// <summary>
    /// Representa uma área no tabuleiro.
    /// </summary>
    struct Area
    {
        template<class... Args>
        Area(Args&&... args)
        {
            ((*this |= args), ...);
        }

        Area(Area&&) noexcept = default;
        Area(const Area&) = default;

        /// <summary>
        /// Adiciona uma posição na área.
        /// </summary>
        /// <param name="position">Posição a ser incluída.</param>
        void AddPosition(const Vector2Int& position);

        /// <summary>
        /// Remove uma posição da área.
        /// </summary>
        /// <param name="position">Posição a ser excluída.</</param>
        void RemovePosition(const Vector2Int& position);

        /// <summary>
        /// Adiciona um raio na área.
        /// </summary>
        /// <param name="ray">Raio a ser incluído.</param>
        void AddRay(const Ray& ray);

        /// <summary>
        /// Checa se uma posição está dentro da área.
        /// </summary>
        /// <param name="position">Posição a ser averiguada.</param>
        /// <returns>Verdadeiro caso a posição esteja dentro da área e falso caso fora.</returns>
        bool IncludesPosition(const Vector2Int& position) const;

        /// <summary>
        /// Concede o valor das posições de uma área.
        /// </summary>
        /// <returns></returns>
        vector<Vector2Int> Positions() const;

        Area& operator|=(const Area& rhs);

        Area& operator&=(const Area& rhs);

        Area& operator|=(const Vector2Int& rhs);

        Area& operator&=(const Vector2Int& rhs);

        Area& operator|=(const Ray& rhs);

        Area& operator&=(const Ray& rhs);

        template<class Rhs>
        friend Area operator|(Area lhs, Rhs&& rhs)
        {
            return lhs |= rhs;
        }

        template<class Rhs>
        friend Area operator&(Area lhs, Rhs&& rhs)
        {
            return lhs &= rhs;
        }

    private:
        struct Hasher { size_t operator()(const Vector2Int& v) const { return reinterpret_cast<const size_t&>(v); } };
        unordered_set<Vector2Int, Hasher> positions;
    };

}
