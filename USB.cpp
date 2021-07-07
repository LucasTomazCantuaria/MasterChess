#include "Arduino/SimpleRPC.hpp"
#include "Windows/SerialPort.hpp"

#include <fmt/format.h>

#include <thread>
#include <array>

#include "Arduino/Matrix8x8.h"

constexpr auto Count = 17 * 16;

static std::array<int, 2> GetSquareIndexes(int i, int j)
{
    j = 7 - j;
    static constexpr uint16_t RowCount = 34, LedCount = 2;
    uint16_t n1 = RowCount * j + LedCount * i, n2 = RowCount * (j + 1) - (LedCount * i + 1);
    return { n1 + 1, n2 - 1 };
}

int main(int argc, char* argv[])
{
    using namespace std::chrono;
    using namespace std::chrono_literals;
    using std::this_thread::sleep_for;
    using Windows::SerialPort;
    using Arduino::Matrix8x8;

    bool quit = false;
    Matrix8x8 value = 0;
    std::thread thread([&]
    {
        while (!quit)
        {
            system("cls");
            for (int i = 0; i < 8; ++i)
            {
                for (int j = 0; j < 8; ++j)
                {
                    putchar(value(7 - i, j) ? '1' : '0');
                    putchar(' ');
                }
                putchar('\n');
            }
            putchar('\n');
            sleep_for(1ms);
        }
    });
    try
    {
        auto serial = SerialPort("COM3");
        auto rpc = Arduino::SimpleRPC(&serial);
        auto read = rpc.CreateRemoteProcessCall<uint64_t>(0);
        auto setBrightness = rpc.CreateRemoteProcessCall<uint8_t, uint8_t>(1);
        auto lighten = rpc.CreateRemoteProcessCall<uint8_t, uint32_t, uint64_t>(2);

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
    quit = true;
    thread.join();
}
