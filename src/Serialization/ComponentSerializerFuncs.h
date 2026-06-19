#pragma once

#include <functional>
#include "SerializedEntity.h"

namespace ComponentSerializerFuncs {
    using SerializeFunc   = std::function<bool(int, SerializedComponent&)>;
    using DeserializeFunc = std::function<bool(int, const SerializedComponent&)>;
}
