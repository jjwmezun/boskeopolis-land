#ifndef INPUT_CONTROLLER_H
#define INPUT_CONTROLLER_H

#include "controller.hpp"

namespace BSL
{
    class InputController : public Controller
    {
        public:
            ~InputController();
            bool heldRight() const override;
            bool heldLeft() const override;
            bool heldJump() const override;
            bool heldRun() const override;
            bool heldMenu() const override;
    };
}

#endif // INPUT_CONTROLLER_H