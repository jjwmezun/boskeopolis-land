#include "input.hpp"
#include "input_controller.hpp"

namespace BSL
{
    InputController::~InputController() {};

    bool InputController::heldRight() const
    {
        return Input::heldRight();
    };

    bool InputController::heldLeft() const
    {
        return Input::heldLeft();
    };

    bool InputController::heldJump() const
    {
        return Input::heldJump();
    };

    bool InputController::heldRun() const
    {
        return Input::heldRun();
    };

    bool InputController::heldMenu() const
    {
        return Input::heldMenu();
    };
}