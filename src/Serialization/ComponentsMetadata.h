#pragma once

#include <tuple>

#include "IComponentSerializer.h"
#include "ComponentSerializer.h"

template<typename T, typename Struct>
struct FieldInfo
{
    const char* name;
    T Struct::* member;
};

#define FIELD(name) FieldInfo<decltype(TypeAlias::name), TypeAlias>{#name, &TypeAlias::name}

template<typename T>
struct StructTraits;

#define COMPONENT(Type, ...) \
    template<> \
    struct StructTraits<Type> { \
        using TypeAlias = Type; \
        static constexpr auto fields = std::make_tuple(__VA_ARGS__); \
    }; \
    static struct Type##Registrator { \
        Type##Registrator() { \
            if (!once) { \
                once = true; \
                IComponentSerializer::AddLateRegistration([](ComponentSerializer& r) { r.Register<Type>(); }); \
            } \
        } \
        static inline bool once; \
    } Type##RegistratorInstance;

template<typename T, typename Func>
void ForEachField(Func&& func)
{
    std::apply([&](auto&&... field) {
        (func(field), ...);
    }, StructTraits<T>::fields);
}
