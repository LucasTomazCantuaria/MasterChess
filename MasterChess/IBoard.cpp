#include "IBoard.hpp"
#include "HitInfo.hpp"
#include "Math/Area.hpp"
#include "Math/Ray.hpp"

namespace MasterChess
{
    bool IBoard::CastRay(const Ray& ray, HitInfo* hitInfo) const
    {
        auto& area = BoardArea();
        auto pos = ray.Position(ray.StartLenght);
        auto i = ray.StartLenght;
        for (; i < ray.Lenght && area.IncludesPosition(pos); pos += ray.Direction, ++i) if (auto piece = At(pos))
        {
            if (hitInfo) *hitInfo = { pos, i, piece };
            return true;
        }

        if (hitInfo) *hitInfo = { pos, i, nullptr };
        return false;
    }
}
