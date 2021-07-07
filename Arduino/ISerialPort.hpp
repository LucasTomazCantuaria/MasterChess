#pragma once
#include <string>
#include <string_view>
#include <vector>

namespace Arduino
{
    struct ISerialPort
    {
        virtual size_t Read(void* data, size_t bytes) = 0;
        virtual size_t Write(const void* data, size_t bytes) = 0;
        std::vector<uint8_t> ReadAll()
        {
            auto buf = std::vector<uint8_t>(0xFF);
            for (auto i = 0; Read(buf.data() + i, 0xFF) == 0xFF; i += 0xFF)
                buf.resize(buf.size() + 0xFF);
            return buf;
        }
        size_t Write(std::string_view sv) { return Write(sv.data(), (int)sv.length()); }
        size_t WriteChar(char d) { return Write(&d, sizeof d); }
        template<class T>
        T ReadTrivialType()
        {
            T result;
            Read(&result, sizeof T);
            return result;
        }
        char ReadChar() { return ReadTrivialType<char>(); }

    };
}
