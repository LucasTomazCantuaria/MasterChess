#include "Area.hpp"

namespace Math
{
    void Area::AddPosition(const Vector2Int& position)
    {
        if (!IncludesPosition(position))
            positions.emplace(position);
    }

    void Area::RemovePosition(const Vector2Int& position)
    {
        positions.erase(positions.find(position));
    }

    void Area::AddRay(const Ray& ray)
    {
        for (int i = ray.StartLenght; i < ray.Lenght; ++i)
            AddPosition(ray.Position(i));
    }

    bool Area::IncludesPosition(const Vector2Int& position) const
    {
        return positions.find(position) != positions.end();
    }

    vector<Vector2Int> Area::Positions() const
    {
        return { positions.begin(), positions.end() };
    }

    Area& Area::operator|=(const Area& rhs)
    {
        for (auto && position : rhs.positions)
            AddPosition(position);
        return *this;
    }

    Area& Area::operator&=(const Area& rhs)
    {
        for (auto && v : rhs.positions)
            positions.erase(v);
        return *this;
    }

    Area& Area::operator|=(const Vector2Int& rhs)
    {
        AddPosition(rhs);
        return *this;
    }

    Area& Area::operator&=(const Vector2Int& rhs)
    {
        RemovePosition(rhs);
        return *this;
    }

    Area& Area::operator|=(const Ray& rhs)
    {
        AddRay(rhs);
        return *this;
    }

    Area& Area::operator&=(const Ray& rhs)
    {
        for (auto i = rhs.StartLenght; i < rhs.Lenght; ++i)
            RemovePosition(rhs.Position(i));
        return *this;
    }
}
