#pragma once
#include "Math/Area.hpp"
#include "Math/Vector2Int.hpp"

namespace Arduino
{
    struct Matrix8x8
    {
        uint64_t Value;

        Matrix8x8(uint64_t value = 0) : Value(value) {  }
        Matrix8x8(const Math::Area& area) : Matrix8x8()
        {
            for (auto v : area.Positions())
                operator()(v.x, v.y, true);
        }
        bool operator()(int i, int j) const { return Value & (uint64_t)1 << (j + (7 - i) * 8); }
        void operator()(int i, int j, bool v)
        {
            auto bit = (uint64_t)1 << (j + (7 - i) * 8);
            if (v) Value |= bit;
            else Value &= ~bit;
        }

        bool operator()(const Math::Vector2Int& v) const { return operator()(v.x, v.y); }
        void operator()(const Math::Vector2Int& v, bool val) { return operator()(v.x, v.y, val); }

        Matrix8x8 operator^(Matrix8x8 rhs) const { return Value ^ rhs.Value; }
        Matrix8x8 operator&(Matrix8x8 rhs) const { return Value & rhs.Value; }
        Matrix8x8 operator|(Matrix8x8 rhs) const { return Value | rhs.Value; }
        Matrix8x8 operator~() const { return ~Value; }

        friend bool operator==(const Matrix8x8& lhs, const Matrix8x8& rhs) { return lhs.Value == rhs.Value; }
        friend bool operator!=(const Matrix8x8& lhs, const Matrix8x8& rhs) { return !(lhs == rhs); }
    };
}
