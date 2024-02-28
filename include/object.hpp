#ifndef OBJECT_H
#define OBJECT_H

#include <cstdint>

namespace BSL
{
    enum class ObjectType
    {
        __NULL,
        MONEY
    };

    union ObjectData
    {
        struct
        {
            uint_fast16_t amount;
        }
        money;
    };

    struct ObjectTemplate
    {
        ObjectType type;
        ObjectData data;
        struct
        {
            uint_fast8_t x;
            uint_fast8_t y;
            uint_fast8_t animation;
        }
        tile;
    };

    struct Object
    {
        ObjectType type;
        ObjectData data;
    };
}

#endif // OBJECT_H