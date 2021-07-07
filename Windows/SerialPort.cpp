#include "SerialPort.hpp"
#include <stdexcept>
#include <Windows.h>

namespace Windows
{
    struct LastErrorException final : std::exception
    {
        LastErrorException() : exception(
            (FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                nullptr, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                buffer, sizeof buffer, nullptr), buffer))
        {

        }

    private:
        char buffer[0xFF];
    };

    static void CloseHandle(void* handle)
    {
        ::CloseHandle(handle);
    }
    
    SerialPort::SerialPort() : hFile(nullptr, &CloseHandle)
    {
        
    }

    SerialPort::SerialPort(const std::string& portName, unsigned baudRate) :
        hFile(CreateFileA(portName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr), &CloseHandle)
    {
        
        if (hFile.get() == INVALID_HANDLE_VALUE) throw LastErrorException();
        DCB dcb;
        dcb.DCBlength = sizeof dcb;
        if (!GetCommState(hFile.get(), &dcb)) throw LastErrorException();
        dcb.BaudRate = baudRate;
        dcb.ByteSize = 8;
        dcb.StopBits = ONESTOPBIT;
        dcb.Parity = PARITY_NONE;
        if (!SetCommState(hFile.get(), &dcb)) throw LastErrorException();
        COMMTIMEOUTS timeouts{ 50, 10, 50, 10, 50 };
        if (!SetCommTimeouts(hFile.get(), &timeouts)) throw LastErrorException();
    }

    size_t SerialPort::Read(void* data, size_t bytes)
    {
        std::lock_guard lock(m);
        DWORD bytesRead;
        if (!ReadFile(hFile.get(), data, (DWORD)bytes, &bytesRead, nullptr)) throw LastErrorException();
        return size_t(bytesRead);
    }

    size_t SerialPort::Write(const void* data, size_t bytes)
    {
        std::lock_guard lock(m);
        DWORD result;
        if (!WriteFile(hFile.get(), data, (DWORD)bytes, &result, nullptr)) throw LastErrorException();
        return size_t(result);
    }

}
