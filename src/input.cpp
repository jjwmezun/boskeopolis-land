#include "input.hpp"
#include "nasringine/nasr_input.h"

namespace BSL
{
    void Input::init()
    {
        NasrInput inputs[] =
        {
            { static_cast<unsigned int>( Keys::RIGHT ), NASR_KEY_RIGHT },
            { static_cast<unsigned int>( Keys::LEFT ), NASR_KEY_LEFT },
            { static_cast<unsigned int>( Keys::JUMP ), NASR_KEY_Z },
            { static_cast<unsigned int>( Keys::RUN ), NASR_KEY_X }
        };
        NasrRegisterInputs( inputs, 4 );
    };

    bool Input::heldRight()
    {
        return NasrHeld( static_cast<unsigned int>( Keys::RIGHT ) );
    };

    bool Input::heldLeft()
    {
        return NasrHeld( static_cast<unsigned int>( Keys::LEFT ) );
    };

    bool Input::heldJump()
    {
        return NasrHeld( static_cast<unsigned int>( Keys::JUMP ) );
    };

    bool Input::heldRun()
    {
        return NasrHeld( static_cast<unsigned int>( Keys::RUN ) );
    };
}