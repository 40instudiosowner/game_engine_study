#include "ComponentSerializer.h"

ComponentSerializer::ComponentSerializer(World &world): _world(world)
{
    for (auto& fn : LateRegistrators())
    {
        fn(*this);
    }
}

bool ComponentSerializer::Serialize(const int entity, const std::type_index &type, SerializedComponent &out)
{
    return _serializer[type](entity, out);
}

bool ComponentSerializer::Deserialize(const int entity, const SerializedComponent &in)
{
    return _deserializer[in.Type](entity, in);
}
