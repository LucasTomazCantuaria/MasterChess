#include "Ray.hpp"
#include <cassert>

namespace Math
{
    Ray::Ray(const Vector2Int& startPosition, const Vector2Int& direction, int lenght, int startLenght) : StartPosition(startPosition), Direction(direction), Lenght(lenght), StartLenght(startLenght)
    {
        assert(lenght > 0);
    }

    Vector2Int Ray::Position(int n) const
    {
        return StartPosition + Direction * n;
    }

    bool Ray::TracesPosition(const Vector2Int& position) const
    {
        for (int i = StartLenght; i < Lenght; ++i) if (Position(i) == position)
            return true;
        return false;
    }
}
