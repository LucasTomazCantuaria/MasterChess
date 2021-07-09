#include "SerialPort.hpp"

#include <boost/asio/io_service.hpp>
#include <boost/asio/serial_port.hpp>

using namespace boost::asio;

namespace Arduino
{
    struct SerialPort::Vars
    {
        io_service io;
        serial_port serial;
        Vars(const std::string& path, unsigned baudRate) : serial(io)
        {
            serial.open(path);
            serial.set_option(serial_port_base::baud_rate(baudRate));
        }
    };


    SerialPort::SerialPort(const std::string& path, unsigned baudRate) : v(std::make_unique<Vars>(path, baudRate))
    {

    }

    SerialPort::~SerialPort() = default;

    size_t SerialPort::Read(void* data, size_t bytes)
    {
        return v->serial.read_some(buffer(data, bytes));
    }

    size_t SerialPort::Write(const void* data, size_t bytes)
    {
        return v->serial.write_some(buffer(data, bytes));
    }

    void SerialPort::Clear()
    {
        uint8_t buf[0xFF];
        while (Read(buf, 0xFF) == 0xFF);
    }

    std::vector<uint8_t> SerialPort::ReadAll()
    {
        auto buf = std::vector<uint8_t>(0xFF);
        for (auto i = 0; Read(buf.data() + i, 0xFF) == 0xFF; i += 0xFF)
            buf.resize(buf.size() + 0xFF);
        return buf;
    }
}
