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
            static bool heldMenu();

            enum class Keys
            {
                RIGHT,
                LEFT,
                JUMP,
                RUN,
                MENU
            };
    };
}

#endif // INPUT_H