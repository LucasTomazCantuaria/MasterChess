#include <chrono>
#include <future>
#include <iostream>
#include <string_view>
#include <sstream>
#include <regex>
#include <process.hpp>
#include <fmt/format.h>

#include "Math/Vector2Int.hpp"

//#include <wiringPi.h>
//#include <wiringSerial.h>

using namespace std::chrono_literals;
using namespace TinyProcessLib;

Math::Vector2Int ParsePosition(std::string_view sv)
{
    return { int(sv[0] - 'a'), int(sv[0] - '0') };
}

class Stockfish
{
    Process process;
    std::stringstream ss;
    std::mutex m;
public:
    template<class Path>
    Stockfish(Path&& path) : process(path, "", [=](auto ptr, auto n)
    {
        std::lock_guard l(m);
        ss << std::string_view{ ptr, n };
    }, nullptr, true)
    {
        
    }

    Stockfish() : Stockfish("stockfish")
    {
        
    }

    ~Stockfish()
    {
        process.kill();
    }

    bool IsRunning()
    {
        int status;
        return !process.try_get_exit_status(status);
    }

    void Command(std::string_view cmd)
    {
        auto str = fmt::format("{}\n", cmd);
        process.write(str);
    }

    std::string Read()
    {
        std::lock_guard l(m);
        return { std::istreambuf_iterator(ss), {} };
    }

    void Clear()
    {
        std::lock_guard l(m);
        ss.str({});
        ss.clear();
    }

    void Go(int depth)
    {
        Command(fmt::format("go depth {}", depth));
        std::regex r(R"(bestmove\s+([a-e][1-8][a-e][1-8]))");
        std::string str;
        std::cmatch m;
        do
        {
            //std::lock_guard l(m);
            getline(ss, str);
            //if (regex_match(r, str, m))
        } while (!str.empty());
    }
    
};

int main(int argc, char* argv[])
{
    try
    {
        //wiringPiSetup();
        //auto fd = serialOpen("/dev/ttyUSB0", 115200);
        //if (fd < 0) return fd;
        //
        //char buffer[0x100];
        //ssize_t n = 1;
        //do
        //{
        //	//n = read(fd, buffer, sizeof buffer);
        //	//printf("%.*s", n, buffer);
        //	std::string str;
        //	std::cin >> str;
        //	n = write(fd, str.c_str(), str.size());
        //}
        //while (n != -1);
        
    }
    catch (const std::exception& e)
    {
        fmt::print("{}\n", e.what());
    }
}