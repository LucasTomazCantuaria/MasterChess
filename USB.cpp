#include "Arduino/SimpleRPC.hpp"
#include "Arduino/SerialPort.hpp"

#include <fmt/format.h>

#include <thread>
#include <array>
#include <cassert>

#include "Arduino/Matrix8x8.h"
#include "Windows/Console.hpp"

using namespace std::chrono;
using namespace std::chrono_literals;
using namespace std::string_view_literals;
using std::this_thread::sleep_for;

void Test1()
{
    auto times = 1000;
    auto baudRate = 2000000;
    auto serial = Arduino::SerialPort("COM3", baudRate);
    
    serial.Clear();
    auto t = high_resolution_clock::now();
    for (int i = 0; i < times; ++i)
    {
        //uint8_t index = 0;
        //serial.write_some(buffer(&index, sizeof index));
        uint64_t v;
        //serial.read_some(buffer(&v, sizeof v));
        std::vector<uint8_t> buffer;
        buffer.emplace_back(1);
        auto str = "Hello World!Hello World!Hello World!Hello World!Hello World!Hello World!Hello World!"sv;
        auto len = (uint8_t)str.length();
        auto bytes = len + sizeof(uint8_t);
        buffer.reserve(buffer.size() + bytes);
        buffer.emplace_back(len);
        copy(str.begin(), str.end(), back_inserter(buffer));
        serial.Write(buffer.data(), buffer.size());
        v = serial.ReadTrivialType<uint64_t>();
    }
    fmt::print("{}\n", (duration_cast<milliseconds>(high_resolution_clock::now() - t) / times).count());
}

void Test2()
{
    auto times = 50;
    auto baudRate = 2000000;
    auto serial = Arduino::SerialPort("COM3", baudRate);
    auto rpc = Arduino::SimpleRPC(&serial);
    auto test2 = rpc.CreateRemoteProcessCall<uint64_t, std::string_view>(4);
    auto t = high_resolution_clock::now();
    for (int i = 0; i < times; ++i)
        (void)test2("Hello World!!!!");
    fmt::print("{}\n", (duration_cast<milliseconds>(high_resolution_clock::now() - t) / times).count());
}

void Test4()
{
    auto times = 20;
    auto baudRate = 2000000;
    auto serial = Arduino::SerialPort("COM3", baudRate);
    auto rpc = Arduino::SimpleRPC(&serial);
    auto read = rpc.CreateRemoteProcessCall<uint64_t>(0);
    auto t = high_resolution_clock::now();
    for (int i = 0; i < times; ++i)
        (void)read();
    fmt::print("{}\n", (duration_cast<milliseconds>(high_resolution_clock::now() - t) / times).count());
}

int main(int argc, char* argv[])
{
    using namespace std::chrono;
    using namespace std::chrono_literals;
    using Arduino::SerialPort;
    using Arduino::Matrix8x8;

    Matrix8x8 value = 0;
    try
    {
        auto serial = SerialPort("COM3", 2000000);
        auto rpc = Arduino::SimpleRPC(&serial);
        auto read = rpc.CreateRemoteProcessCall<uint64_t>(0);
        auto setBrightness = rpc.CreateRemoteProcessCall<uint8_t, uint8_t>(1);
        auto lighten = rpc.CreateRemoteProcessCall<uint8_t, uint32_t, uint64_t>(2);

        fmt::print("{}\n", read());

        setBrightness(0xFF);
        lighten(-1, -1);

        while (true)
        {
            serial.WriteChar('r');
            auto v = Matrix8x8(read());

            auto x = value ^ v;
            value = v;

            Matrix8x8 m1, m2;
            for (auto i = 0; i < 8; ++i)
            for (auto j = 0; j < 8; ++j)
            {
                if (!x(i, j)) continue;
                if (v(i, j))
                    m1(i, j, true);
                else m2(i, j, true);
            }
            if (m1.Value)
                lighten(0xFF0000, m1.Value);
            if (m2.Value)
                lighten(0xFFFFFF, m2.Value);
        }
    }
    catch (std::exception& e)
    {
        fmt::print("{}\n", e.what());
    }
}
