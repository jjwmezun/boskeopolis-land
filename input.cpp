

// Name
//===================================
//
// Input
//


// DEPENDENCIES
//===================================

    #include "input.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    void Input::reset()
    {
        keys_pressed_.clear();
        keys_released_.clear();
        keys_held_.clear();
    };

    void Input::update()
    {
        keys_pressed_.clear();
        keys_released_.clear();
    };

    bool Input::pressed( Action action )
    {
        return keys_pressed_[ buttons_[ action ] ];
    };

    bool Input::released( Action action )
    {
        return keys_released_[ buttons_[ action ] ];
    };

    bool Input::held( Action action )
    {
        return keys_held_[ buttons_[ action ] ];
    };

    void Input::keyPress( SDL_Keycode key )
    {
        if ( !keys_pressed_before_released_[ key ] )
        {
            keys_pressed_[ key ] = true;
            keys_pressed_before_released_[ key ] = true;
        }
    };

    void Input::keyRelease( SDL_Keycode key )
    {
        keys_released_[ key ] = true;
        keys_pressed_[ key ] = false;
        keys_held_[ key ] = false;
        keys_pressed_before_released_[ key ] = false;
    };

    void Input::keyHold( SDL_Keycode key )
    {
        keys_held_[ key ] = true;
    };
