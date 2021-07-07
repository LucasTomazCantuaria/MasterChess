#include "Serialization.hpp"
#include <iterator>

namespace Arduino
{
    template struct Serializer<bool>;
    template struct Serializer<int8_t>;
    template struct Serializer<uint8_t>;
    template struct Serializer<int16_t>;
    template struct Serializer<uint16_t>;
    template struct Serializer<int32_t>;
    template struct Serializer<uint32_t>;
    template struct Serializer<int64_t>;
    template struct Serializer<uint64_t>;
    template struct Serializer<float>;
    
    using std::enable_if_t;
    using std::is_same_v;
    using std::is_integral_v;

    const string_view Serializer<void, void>::Id = "";

    string Serializer<string>::DeSerialize(const uint8_t* in, size_t size)
    {
        return { (const char*)in, size };
    }

    const string_view Serializer<string>::Id = "s";

    const string_view Serializer<string_view>::Id = "s";

    void Serializer<string_view>::Serialize(vector<uint8_t>& buffer, string_view in)
    {
        buffer.reserve(buffer.size() + in.size() + 1);
        copy(in.begin(), in.end(), back_inserter(buffer));
        buffer.push_back('\0');
    }

}
