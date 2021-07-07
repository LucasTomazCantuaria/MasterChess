#include "SerialPort.hpp"
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>

namespace Linux
{
    void CloseFile(void* file)
    {
        close((int)file);
    }

    
    SerialPort::SerialPort(const char* portName) : port((void*)open(portName, O_RDWR), CloseFile)
    {
        auto serial_port = (int)port.get();
        if (serial_port < 0) throw "asd";
        termios tty;
        if (tcgetattr(serial_port, &tty)) throw "asd";
        tty.c_cflag &= ~PARENB;
        tty.c_cflag &= ~CSTOPB;
        tty.c_cflag |= CS8;
        tty.c_cflag &= ~CRTSCTS;
        tty.c_cflag |= CREAD | CLOCAL;
        tty.c_lflag &= ~ICANON;
        tty.c_lflag &= ~ECHO;
        tty.c_lflag &= ~ECHOE;
        tty.c_lflag &= ~ECHONL;
        tty.c_lflag &= ~ISIG;
        tty.c_iflag &= ~(IXON | IXOFF | IXANY);
        tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);
        tty.c_oflag &= ~OPOST;
        tty.c_oflag &= ~ONLCR;
        tty.c_cc[VTIME] = 0;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
        tty.c_cc[VMIN] = 0;
        cfsetispeed(&tty, B115200);
        cfsetospeed(&tty, B115200);
        if (tcsetattr(serial_port, TCSANOW, &tty)) throw "asd";
        
    }
    
    int SerialPort::Write(const void* data, int bytes)
    {
        return (int)write((int)port.get(), data, (size_t)bytes);
    }

    std::string SerialPort::Read()
    {
        char buffer[0xFF];
        auto n = read((int)port.get(), &buffer, sizeof(buffer));
        return { buffer, (size_t)n };
    }
    
}
