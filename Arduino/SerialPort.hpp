#pragma once
#include <string>
#include <string_view>
#include <vector>
#include <memory>

namespace Arduino
{
    struct SerialPort
    {
        explicit SerialPort(const std::string& path, unsigned baudRate = 115200);
        SerialPort(const SerialPort&) = delete;
        SerialPort(SerialPort&&) = delete;
        SerialPort& operator=(const SerialPort&) = delete;
        SerialPort& operator=(SerialPort&&) = delete;
        ~SerialPort();

        size_t Read(void* data, size_t bytes);
        size_t Write(const void* data, size_t bytes);
        void Clear();

        std::vector<uint8_t> ReadAll();

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
    private:
        struct Vars;
        std::unique_ptr<Vars> v;
    };
}
