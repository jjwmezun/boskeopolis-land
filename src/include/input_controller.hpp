#ifndef INPUT_CONTROLLER_H
#define INPUT_CONTROLLER_H

#include "controller.hpp"

namespace BSL
{
    class InputController : public Controller
    {
        public:
            ~InputController();
            bool heldRight() override;
            bool heldLeft() override;
            bool heldJump() override;
            bool heldRun() override;
    };
}

#endif // INPUT_CONTROLLER_H