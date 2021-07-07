#pragma once
#include "ISerialPort.hpp"
#include "Serialization.hpp"

#include <string_view>
#include <vector>

namespace Arduino
{
    struct SimpleRPC
    {
        SimpleRPC(ISerialPort* serialPort);

        void UpdateSignatures();

        template<class Result, class... Args>
        auto Call(uint8_t index, Args... args)
        {
            auto sig = signatures[index];
            if (sig.find(MangledSignature<Result(*)(Args...)>::Id) != 0) throw std::runtime_error("Incompatible signatures.");
            std::vector<uint8_t> buffer;
            buffer.emplace_back(index);

            ((Serializer<Args>::Serialize(buffer, args)), ...);

            serialPort->Write(buffer.data(), (int)buffer.size());
            if constexpr (!std::is_same_v<void, Result>)
            {
                auto sRes = serialPort->ReadAll();
                return Serializer<Result>::DeSerialize(sRes.data(), sRes.size());
            }
        }

        template<class Result, class... Args>
        auto CreateRemoteProcessCall(uint8_t index)
        {
            return [=](Args... args) { return Call<Result, Args...>(index, args...); };
        }

        template<class Fn>
        auto CreateFunction(uint8_t index)
        {
            return CreateFunctionImpl(index, Fn{});
        }

    private:
        struct Header;
        ISerialPort* serialPort;
        std::string binarySignature;
        std::vector<std::string_view> signatures;
        Header* header;

        template<class Result, class... Args>
        auto CreateFunctionImpl(uint8_t index, Result(*)(Args...))
        {
            return CreateRemoteProcessCall<Result, Args...>(index);
        }

    };
    
}

