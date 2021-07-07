#pragma once
#include <utility>

namespace Math
{
    struct Vector2Int
    {
        static const Vector2Int Zero;
        static const Vector2Int One;
        static const Vector2Int Up;
        static const Vector2Int Down;
        static const Vector2Int Right;
        static const Vector2Int Left;

        int x, y;

        Vector2Int(int x, int y) : x(x), y(y) {  }

        Vector2Int() : Vector2Int(0, 0) {  }

        int& operator[](int i);
        int operator[](int i) const;

        Vector2Int Perpendicular() const { return { y, -x }; }

        static Vector2Int Max(const Vector2Int& lhs, const Vector2Int& rhs);

        static Vector2Int Min(const Vector2Int& lhs, const Vector2Int& rhs);

        static Vector2Int Clamp(const Vector2Int& value, const Vector2Int& lower, const Vector2Int& higher);

        Vector2Int operator-() const;

        Vector2Int& operator+=(const Vector2Int& rhs);

        Vector2Int& operator-=(const Vector2Int& rhs);

        Vector2Int& operator*=(int rhs);

        Vector2Int& operator/=(int rhs);

        friend Vector2Int operator+(Vector2Int lhs, const Vector2Int& rhs);

        friend Vector2Int operator-(Vector2Int lhs, const Vector2Int& rhs);

        friend Vector2Int operator*(Vector2Int lhs, int rhs);

        friend Vector2Int operator/(Vector2Int lhs, int rhs);

        friend Vector2Int operator*(int lhs, Vector2Int rhs);

        friend bool operator==(const Vector2Int& lhs, const Vector2Int& rhs);

        friend bool operator!=(const Vector2Int& lhs, const Vector2Int& rhs);
    };

    inline const Vector2Int Vector2Int::One		{ 1, 1 };
    inline const Vector2Int Vector2Int::Zero	{ 0, 0 };
    inline const Vector2Int Vector2Int::Up		{ 0, 1 };
    inline const Vector2Int Vector2Int::Down	{ 0,-1 };
    inline const Vector2Int Vector2Int::Right	{ 1, 0 };
    inline const Vector2Int Vector2Int::Left	{-1, 0 };
    
}

namespace std
{
    template<>
    struct hash<Math::Vector2Int>
    {
        size_t operator()(const Math::Vector2Int& v) const noexcept
        {
            return reinterpret_cast<const size_t&>(v);
        }
    };
}
