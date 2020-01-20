#pragma once

struct OWObject
{
    enum class Type
    {
        LEVEL
    };

    union Value
    {
        int level;
    };

    Type type;
    Value value;

    static OWObject createLevel( int level )
    {
        return { Type::LEVEL, level };
    };
};