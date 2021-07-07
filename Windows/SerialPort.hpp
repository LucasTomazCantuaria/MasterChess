#pragma once
#include <memory>
#include <mutex>
#include <string>
#include "Arduino/ISerialPort.hpp"

namespace Windows
{
    struct SerialPort : Arduino::ISerialPort
    {
        SerialPort();
        explicit SerialPort(const std::string& portName, unsigned int baudRate = 115200);
        size_t Read(void* data, size_t bytes) override;
        size_t Write(const void* data, size_t bytes) override;
    private:
        std::unique_ptr<void, void(*)(void*)> hFile;
        std::mutex m;
    };
    
}
