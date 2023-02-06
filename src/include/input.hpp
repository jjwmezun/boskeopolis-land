#ifndef INPUT_H
#define INPUT_H

namespace BSL
{
    class Input
    {
        public:
            static void init();
            static bool heldRight();
            static bool heldLeft();
            static bool heldJump();
            static bool heldRun();

            enum class Keys
            {
                RIGHT,
                LEFT,
                JUMP,
                RUN
            };
    };
}

#endif // INPUT_H