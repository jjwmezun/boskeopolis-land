#ifndef GOAL_H
#define GOAL_H

namespace BSL
{
    enum class GoalType
    {
        KEYCANE,
        COLLECT,
        TIMED,
        LAND_O_COMMUNISM,
        LAND_O_CAPITALISM
    };

    struct Goal
    {
        GoalType type;
        char * message;
        union
        {
            struct
            {
                uint_fast32_t amount;
            } collect;
            struct
            {
                uint_fast16_t time;
            } timed;
        } data;
    };
};

#endif // GOAL_H