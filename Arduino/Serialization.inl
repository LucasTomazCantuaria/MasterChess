#pragma once

namespace Arduino
{
    template<class T, class Alloc>
    const string Serializer<std::vector<T, Alloc>>::Id = fmt::format("[{}]", Serializer<T>::Id);

    template<class... Args>
    const string Serializer<tuple<Args...>>::Id = format("[{}]", fmt::join(vector<string_view>{ Serializer<Args>::Id... }, ""));

    template <class T>
    const string_view Serializer<T, std::enable_if_t<std::is_integral_v<T> || std::is_same_v<T, float>>>::Id = []() -> string_view
    {
        using std::is_same_v;
        if constexpr (is_same_v<T, bool>)
            return "?";
        else if constexpr (is_same_v<T, char>)
            return "c";
        else if constexpr (is_same_v<T, uint8_t>)
            return "B";
        else if constexpr (is_same_v<T, int16_t>)
            return "h";
        else if constexpr (is_same_v<T, uint16_t>)
            return "H";
        else if constexpr (is_same_v<T, int32_t>)
            return "l";
        else if constexpr (is_same_v<T, uint32_t>)
            return "L";
        else if constexpr (is_same_v<T, int64_t>)
            return "q";
        else if constexpr (is_same_v<T, uint64_t>)
            return "Q";
        else if constexpr (is_same_v<T, float>)
            return "f";
        return {};
    }();

    template <class T>
    void Serializer<T, std::enable_if_t<std::is_integral_v<T> || std::is_same_v<T, float>>>::Serialize(vector<uint8_t>& buffer, T in)
    {
        auto i = buffer.size();
        buffer.resize(i + sizeof(T));
        memcpy(buffer.data() + i, &in, sizeof(T));
    }

    template <class T>
    T Serializer<T, std::enable_if_t<std::is_integral_v<T> || std::is_same_v<T, float>>>::DeSerialize(const uint8_t* in, size_t size)
    {
        T out;
        memcpy(&out, in, sizeof(T));
        return out;
    }
    
}
