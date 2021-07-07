#include "Vector2Int.hpp"
#include <algorithm>

using std::min;
using std::max;

namespace Math
{
    int& Vector2Int::operator[](int i)
    {
        return i == 0 ? x : y;
    }

    int Vector2Int::operator[](int i) const
    {
        return i == 0 ? x : y;
    }

    Vector2Int Vector2Int::Max(const Vector2Int& lhs, const Vector2Int& rhs)
    {
        return {max(lhs.x, rhs.x), max(lhs.y, rhs.y)};
    }

    Vector2Int Vector2Int::Min(const Vector2Int& lhs, const Vector2Int& rhs)
    {
        return {min(lhs.x, rhs.x), min(lhs.y, rhs.y)};
    }

    Vector2Int Vector2Int::Clamp(const Vector2Int& value, const Vector2Int& lower, const Vector2Int& higher)
    {
        return Max(lower, Min(value, higher));
    }

    Vector2Int Vector2Int::operator-() const
    {
        return { -x, -y };
    }

    Vector2Int& Vector2Int::operator+=(const Vector2Int& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    Vector2Int& Vector2Int::operator-=(const Vector2Int& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    Vector2Int& Vector2Int::operator*=(int rhs)
    {
        x *= rhs;
        y *= rhs;
        return *this;
    }

    Vector2Int& Vector2Int::operator/=(int rhs)
    {
        x /= rhs;
        y /= rhs;
        return *this;
    }

    Vector2Int operator+(Vector2Int lhs, const Vector2Int& rhs)
    {
        return lhs += rhs;
    }

    Vector2Int operator-(Vector2Int lhs, const Vector2Int& rhs)
    {
        return lhs -= rhs;
    }

    Vector2Int operator*(Vector2Int lhs, int rhs)
    {
        return lhs *= rhs;
    }

    Vector2Int operator/(Vector2Int lhs, int rhs)
    {
        return lhs /= rhs;
    }

    Vector2Int operator*(int lhs, Vector2Int rhs)
    {
        return rhs *= lhs;
    }

    bool operator==(const Vector2Int& lhs, const Vector2Int& rhs)
    {
        return lhs.x == rhs.x && lhs.y == rhs.y;
    }

    bool operator!=(const Vector2Int& lhs, const Vector2Int& rhs)
    {
        return !(lhs == rhs);
    }

}
