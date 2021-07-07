#pragma once
#include <cstdint>

namespace MasterChess
{
    struct IInput;

    struct IPlayer
    {
        virtual IInput* Input() const = 0;
        virtual int Team() const = 0;
        virtual uint32_t Color() const = 0;
    };
}
