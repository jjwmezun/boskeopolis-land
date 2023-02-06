#include "input.hpp"
#include "input_controller.hpp"

namespace BSL
{
    InputController::~InputController() {};

    bool InputController::heldRight()
    {
        return Input::heldRight();
    };

    bool InputController::heldLeft()
    {
        return Input::heldLeft();
    };

    bool InputController::heldJump()
    {
        return Input::heldJump();
    };

    bool InputController::heldRun()
    {
        return Input::heldRun();
    };
}