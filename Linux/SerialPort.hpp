#pragma once
#include "Arduino/ISerialPort.hpp"

#include <memory>
#include <string>


namespace Linux
{
    using std::string_view;
    
    struct SerialPort : Arduino::ISerialPort
    {
        SerialPort(const char* portName);
        
        int Write(const void* data, int bytes) override;
        
        std::string Read() override;
        
    private:
        std::unique_ptr<void, void(*)(void*)> port;
    };
    
}
