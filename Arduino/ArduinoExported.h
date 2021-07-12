#pragma once
#include "SerialPort.hpp"
#include "Matrix8x8.h"
#include "SimpleRPC.hpp"

#include <cstdint>
#include <functional>

namespace Arduino
{
    using std::function;

    struct ArduinoExported
    {
        explicit ArduinoExported(SerialPort* serial) : rpc(serial)
        {
            read = rpc.CreateRemoteProcessCall<uint64_t>(0);
            setBrightness = rpc.CreateRemoteProcessCall<uint8_t, uint8_t>(1);
            lighten = rpc.CreateRemoteProcessCall<uint8_t, uint32_t, uint64_t>(2);
        }
        uint64_t Read() const { return read(); }
        void SetBrightness(uint8_t v) const { return setBrightness(v); }
        void Lighten(uint32_t color, uint64_t area) const { return lighten(color, area); }
        Matrix8x8 ReadMatrix() const { return Read(); }
    private:
        SerialPort* serial;
        SimpleRPC rpc;
        function<uint64_t()> read;
        function<void(uint8_t)> setBrightness;
        function<void(uint32_t, uint64_t)> lighten;
    };

}
