#include "SimpleRPC.hpp"

namespace Arduino
{
#pragma pack(push, 1)
    struct SimpleRPC::Header
    {
        char Identifier[sizeof "simpleRPC"];
        char Version[3];
        char Endianness;
        char VectorSize;
    };
#pragma pack(pop)

    static std::vector<std::string_view> Split(std::string_view sv, char delim)
    {
        std::vector<std::string_view> v;
        for (auto n = sv.find(delim); n != std::string_view::npos; n = sv.find(delim))
        {
            v.emplace_back(sv.data(), n);
            sv = { sv.data() + n + 1, sv.size() - n };
        }
        return v;
    }
    
    SimpleRPC::SimpleRPC(ISerialPort* serialPort) : serialPort(serialPort)
    {
        signatures.reserve(0xFF);
        UpdateSignatures();
    }

    void SimpleRPC::UpdateSignatures()
    {
        signatures.clear();
        char buffer[0x100];
        size_t readBytes;
        do
        {
            serialPort->Write("\xFF");
            readBytes = serialPort->Read(buffer, std::size(buffer) * sizeof(char));
        } while (readBytes == 0);
        binarySignature = { buffer, buffer + readBytes };
        header = (Header*)binarySignature.c_str();
        auto sv = std::string_view{ binarySignature.data() + sizeof(Header) + 1, binarySignature.length() - sizeof(Header) };
        for (auto s : Split(sv, '\0')) if (!s.empty())
            signatures.emplace_back(s);
    }
}
