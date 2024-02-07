#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include "gfx.hpp"
#include "controls.hpp"
#include "log.hpp"
#include <cstdint>
#include <cstdlib>
#include <cstring>

namespace BSL::Controls
{
    #define KEY_UNKNOWN 0
    #define KEY_SPACE 1
    #define KEY_APOSTROPHE 2
    #define KEY_COMMA 3
    #define KEY_MINUS 4
    #define KEY_PERIOD 5
    #define KEY_SLASH 6
    #define KEY_0 7
    #define KEY_1 8
    #define KEY_2 9
    #define KEY_3 10
    #define KEY_4 11
    #define KEY_5 12
    #define KEY_6 13
    #define KEY_7 14
    #define KEY_8 15
    #define KEY_9 16
    #define KEY_SEMICOLON 17
    #define KEY_EQUAL 18
    #define KEY_A 19
    #define KEY_B 20
    #define KEY_C 21
    #define KEY_D 22
    #define KEY_E 23
    #define KEY_F 24
    #define KEY_G 25
    #define KEY_H 26
    #define KEY_I 27
    #define KEY_J 28
    #define KEY_K 29
    #define KEY_L 30
    #define KEY_M 31
    #define KEY_N 32
    #define KEY_O 33
    #define KEY_P 34
    #define KEY_Q 35
    #define KEY_R 36
    #define KEY_S 37
    #define KEY_T 38
    #define KEY_U 39
    #define KEY_V 40
    #define KEY_W 41
    #define KEY_X 42
    #define KEY_Y 43
    #define KEY_Z 44
    #define KEY_LEFT_BRACKET 45
    #define KEY_BACKSLASH 46
    #define KEY_RIGHT_BRACKET 47
    #define KEY_GRAVE_ACCENT 48
    #define KEY_WORLD_1 49
    #define KEY_WORLD_2 50
    #define KEY_ESCAPE 51
    #define KEY_ENTER 52
    #define KEY_TAB 53
    #define KEY_BACKSPACE 54
    #define KEY_INSERT 55
    #define KEY_DELETE 56
    #define KEY_RIGHT 57
    #define KEY_LEFT 58
    #define KEY_DOWN 59
    #define KEY_UP 60
    #define KEY_PAGE_UP 61
    #define KEY_PAGE_DOWN 62
    #define KEY_HOME 63
    #define KEY_END 64
    #define KEY_CAPS_LOCK 65
    #define KEY_SCROLL_LOCK 66
    #define KEY_NUM_LOCK 67
    #define KEY_PRINT_SCREEN 68
    #define KEY_PAUSE 69
    #define KEY_F1 70
    #define KEY_F2 71
    #define KEY_F3 72
    #define KEY_F4 73
    #define KEY_F5 74
    #define KEY_F6 75
    #define KEY_F7 76
    #define KEY_F8 77
    #define KEY_F9 78
    #define KEY_F10 79
    #define KEY_F11 80
    #define KEY_F12 81
    #define KEY_F13 82
    #define KEY_F14 83
    #define KEY_F15 84
    #define KEY_F16 85
    #define KEY_F17 86
    #define KEY_F18 87
    #define KEY_F19 88
    #define KEY_F20 89
    #define KEY_F21 90
    #define KEY_F22 91
    #define KEY_F23 92
    #define KEY_F24 93
    #define KEY_F25 94
    #define KEY_KP_0 95
    #define KEY_KP_1 96
    #define KEY_KP_2 97
    #define KEY_KP_3 98
    #define KEY_KP_4 99

    struct KeyPair
    {
        unsigned int id;
        unsigned int key;
    };

    // Static Data
    static int max_inputs;
    static int inputs_per_key;
    static int keys_per_input;
    static int key_input_size;
    static int input_key_size;
    static int input_keys_start;
    static int pressed_keys_start;
    static int held_keys_start;
    static int pressed_start;
    static int pressed_size;
    static int held_start;
    static int * keydata;
    static int key_to_glfw_map[] =
    {
        GLFW_KEY_UNKNOWN, // KEY_UNKNOWN
        GLFW_KEY_SPACE, // KEY_SPACE
        GLFW_KEY_APOSTROPHE, // KEY_APOSTROPHE
        GLFW_KEY_COMMA, // KEY_COMMA
        GLFW_KEY_MINUS, // KEY_MINUS
        GLFW_KEY_PERIOD, // KEY_PERIOD
        GLFW_KEY_SLASH, // KEY_SLASH
        GLFW_KEY_0, // KEY_0
        GLFW_KEY_1, // KEY_1
        GLFW_KEY_2, // KEY_2
        GLFW_KEY_3, // KEY_3
        GLFW_KEY_4, // KEY_4
        GLFW_KEY_5, // KEY_5
        GLFW_KEY_6, // KEY_6
        GLFW_KEY_7, // KEY_7
        GLFW_KEY_8, // KEY_8
        GLFW_KEY_9, // KEY_9
        GLFW_KEY_SEMICOLON, // KEY_SEMICOLON
        GLFW_KEY_EQUAL, // KEY_EQUAL
        GLFW_KEY_A, // KEY_A
        GLFW_KEY_B, // KEY_B
        GLFW_KEY_C, // KEY_C
        GLFW_KEY_D, // KEY_D
        GLFW_KEY_E, // KEY_E
        GLFW_KEY_F, // KEY_F
        GLFW_KEY_G, // KEY_G
        GLFW_KEY_H, // KEY_H
        GLFW_KEY_I, // KEY_I
        GLFW_KEY_J, // KEY_J
        GLFW_KEY_K, // KEY_K
        GLFW_KEY_L, // KEY_L
        GLFW_KEY_M, // KEY_M
        GLFW_KEY_N, // KEY_N
        GLFW_KEY_O, // KEY_O
        GLFW_KEY_P, // KEY_P
        GLFW_KEY_Q, // KEY_Q
        GLFW_KEY_R, // KEY_R
        GLFW_KEY_S, // KEY_S
        GLFW_KEY_T, // KEY_T
        GLFW_KEY_U, // KEY_U
        GLFW_KEY_V, // KEY_V
        GLFW_KEY_W, // KEY_W
        GLFW_KEY_X, // KEY_X
        GLFW_KEY_Y, // KEY_Y
        GLFW_KEY_Z, // KEY_Z
        GLFW_KEY_LEFT_BRACKET, // KEY_LEFT_BRACKET
        GLFW_KEY_BACKSLASH, // KEY_BACKSLASH
        GLFW_KEY_RIGHT_BRACKET, // KEY_RIGHT_BRACKET
        GLFW_KEY_GRAVE_ACCENT, // KEY_GRAVE_ACCENT
        GLFW_KEY_WORLD_1, // KEY_WORLD_1
        GLFW_KEY_WORLD_2, // KEY_WORLD_2
        GLFW_KEY_ESCAPE, // KEY_ESCAPE
        GLFW_KEY_ENTER, // KEY_ENTER
        GLFW_KEY_TAB, // KEY_TAB
        GLFW_KEY_BACKSPACE, // KEY_BACKSPACE
        GLFW_KEY_INSERT, // KEY_INSERT
        GLFW_KEY_DELETE, // KEY_DELETE
        GLFW_KEY_RIGHT, // KEY_RIGHT
        GLFW_KEY_LEFT, // KEY_LEFT
        GLFW_KEY_DOWN, // KEY_DOWN
        GLFW_KEY_UP, // KEY_UP
        GLFW_KEY_PAGE_UP, // KEY_PAGE_UP
        GLFW_KEY_PAGE_DOWN, // KEY_PAGE_DOWN
        GLFW_KEY_HOME, // KEY_HOME
        GLFW_KEY_END, // KEY_END
        GLFW_KEY_CAPS_LOCK, // KEY_CAPS_LOCK
        GLFW_KEY_SCROLL_LOCK, // KEY_SCROLL_LOCK
        GLFW_KEY_NUM_LOCK, // KEY_NUM_LOCK
        GLFW_KEY_PRINT_SCREEN, // KEY_PRINT_SCREEN
        GLFW_KEY_PAUSE, // KEY_PAUSE
        GLFW_KEY_F1, // KEY_F1
        GLFW_KEY_F2, // KEY_F2
        GLFW_KEY_F3, // KEY_F3
        GLFW_KEY_F4, // KEY_F4
        GLFW_KEY_F5, // KEY_F5
        GLFW_KEY_F6, // KEY_F6
        GLFW_KEY_F7, // KEY_F7
        GLFW_KEY_F8, // KEY_F8
        GLFW_KEY_F9, // KEY_F9
        GLFW_KEY_F10, // KEY_F10
        GLFW_KEY_F11, // KEY_F11
        GLFW_KEY_F12, // KEY_F12
        GLFW_KEY_F13, // KEY_F13
        GLFW_KEY_F14, // KEY_F14
        GLFW_KEY_F15, // KEY_F15
        GLFW_KEY_F16, // KEY_F16
        GLFW_KEY_F17, // KEY_F17
        GLFW_KEY_F18, // KEY_F18
        GLFW_KEY_F19, // KEY_F19
        GLFW_KEY_F20, // KEY_F20
        GLFW_KEY_F21, // KEY_F21
        GLFW_KEY_F22, // KEY_F22
        GLFW_KEY_F23, // KEY_F23
        GLFW_KEY_F24, // KEY_F24
        GLFW_KEY_F25, // KEY_F25
        GLFW_KEY_KP_0, // KEY_KP_0
        GLFW_KEY_KP_1, // KEY_KP_1
        GLFW_KEY_KP_2, // KEY_KP_2
        GLFW_KEY_KP_3, // KEY_KP_3
        GLFW_KEY_KP_4, // KEY_KP_4
        GLFW_KEY_KP_5, // KEY_KP_5
        GLFW_KEY_KP_6, // KEY_KP_6
        GLFW_KEY_KP_7, // KEY_KP_7
        GLFW_KEY_KP_8, // KEY_KP_8
        GLFW_KEY_KP_9, // KEY_KP_9
        GLFW_KEY_KP_DECIMAL, // KEY_KP_DECIMAL
        GLFW_KEY_KP_DIVIDE, // KEY_KP_DIVIDE
        GLFW_KEY_KP_MULTIPLY, // KEY_KP_MULTIPLY
        GLFW_KEY_KP_SUBTRACT, // KEY_KP_SUBTRACT
        GLFW_KEY_KP_ADD, // KEY_KP_ADD
        GLFW_KEY_KP_ENTER, // KEY_KP_ENTER
        GLFW_KEY_KP_EQUAL, // KEY_KP_EQUAL
        GLFW_KEY_LEFT_SHIFT, // KEY_LEFT_SHIFT
        GLFW_KEY_LEFT_CONTROL, // KEY_LEFT_CONTROL
        GLFW_KEY_LEFT_ALT, // KEY_LEFT_ALT
        GLFW_KEY_LEFT_SUPER, // KEY_LEFT_SUPER
        GLFW_KEY_RIGHT_SHIFT, // KEY_RIGHT_SHIFT
        GLFW_KEY_RIGHT_CONTROL, // KEY_RIGHT_CONTROL
        GLFW_KEY_RIGHT_ALT, // KEY_RIGHT_ALT
        GLFW_KEY_RIGHT_SUPER, // KEY_RIGHT_SUPER
        GLFW_KEY_MENU, // KEY_MENU
        GLFW_KEY_LAST  // KEY_LAST
    };



    // Static Functions
    static int held( int id );
    static int pressed( int id );
    static int * GetHeld( int id );
    static int * GetPressed( int id );
    static int * GetHeldKeys( int input );
    static int * GetPressedKeys( int input );
    static int * GetInputKeys( int input );
    static int * GetKeyInputs( int key );
    static void HandleInput( void * window, int key, int scancode, int action, int mods );



    // Public Functions
    void update()
    {
        memset( &keydata[ pressed_keys_start ], 0, pressed_size );
    };

    int pressedConfirm()
    {
        return pressed( 4 );
    };

    int heldConfirm()
    {
        return held( 4 );
    };

    int heldCancel()
    {
        return held( 5 );
    };

    int heldUp()
    {
        return held( 0 );
    };

    int heldRight()
    {
        return held( 1 );
    };

    int heldDown()
    {
        return held( 2 );
    };

    int heldLeft()
    {
        return held( 3 );
    };

    void init()
    {
        KeyPair inputs[] =
        {
            { 0, KEY_UP },
            { 1, KEY_RIGHT },
            { 2, KEY_DOWN },
            { 3, KEY_LEFT },
            { 4, KEY_Z },
            { 5, KEY_X }
        };
        unsigned int num_o_inputs = 6;

        BSL::GFX::registerInputHandler( HandleInput );

        // These several loops are necessary to find the max inputs per key & max keys per input.
        int keys_max_inputs[ GLFW_KEY_LAST ] = { 0 };
        const int mapsize = sizeof( key_to_glfw_map ) / sizeof( int );
        for ( int i = 0; i < num_o_inputs; ++i )
        {
            if ( inputs[ i ].key >= mapsize )
            {
                BSL::log( "Controls init Error: Input #%d has invalid key “%d”.", i, inputs[ i ].key );
                max_inputs = 0;
                return;
            }
            if ( inputs[ i ].id + 1 > max_inputs )
            {
                max_inputs = inputs[ i ].id + 1;
            }
            ++keys_max_inputs[ key_to_glfw_map[ inputs[ i ].key ] ];
        }
        int * inputs_max_keys = ( int * )( calloc( max_inputs, sizeof( int ) ) );
        for ( int i = 0; i < GLFW_KEY_LAST; ++i )
        {
            if ( keys_max_inputs[ i ] > inputs_per_key )
            {
                inputs_per_key = keys_max_inputs[ i ];
            }
        }
        for ( int i = 0; i < num_o_inputs; ++i )
        {
            ++inputs_max_keys[ inputs[ i ].id ];
        }
        for ( int i = 0; i < max_inputs; ++i )
        {
            if ( inputs_max_keys[ i ] > keys_per_input )
            {
                keys_per_input = inputs_max_keys[ i ];
            }
        }
        free( inputs_max_keys );

        // Calculate size o’ chunks.
        // 4 chunks are:
        // * List o’ inputs for each key ( list o’ lists )
        //       -> MAX_KEYS * ( 1 int for inner list count + inner list length )
        // * List o’ keys for each input ( list o’ lists )
        //       -> MAX_INPUTS * ( 1 int for inner list count + inner list length )
        // * List o’ keys held for each input ( list o’ lists )
        //       -> MAX_INPUTS * inner list length.
        //          We don’t need counts, as these are only e’er referenced from inputs & ne’er looped thru.
        // * List o’ keys pressed for each input ( list o’ lists )
        //       -> MAX_INPUTS * inner list length.
        // * List o’ inputs pressed ( 1D list )
        //       -> MAX_INPUTS
        //          This is needed for inputs pressed, regardless o’ efficiency, as we need to test if that particular
        //          input has already been pressed before.
        // * List o’ inputs held ( 1D list )
        //       -> MAX_INPUTS
        //          Tho this value can be found thru the previous held_keys list, it’s mo’ efficient to only loop thru &
        //          check all keys held during press or release & set 1 simple value to be checked during the main game
        //          loop, 60 times per second.
        key_input_size = 1 + inputs_per_key;
        const int key_inputs_size = key_input_size * GLFW_KEY_LAST;
        input_keys_start = key_inputs_size;
        input_key_size = 1 + keys_per_input;
        const int input_keys_size = input_key_size * max_inputs;
        held_keys_start = input_keys_start + input_keys_size;
        const int pressed_keys_count = keys_per_input * max_inputs;
        pressed_keys_start = held_keys_start + pressed_keys_count;
        pressed_start = pressed_keys_start + pressed_keys_count;
        const int pressed_count = max_inputs;
        pressed_size = ( pressed_keys_count + pressed_count ) * sizeof( int );
        held_start = pressed_start + pressed_count;
        const int total_size = key_inputs_size + input_keys_size + input_keys_size + pressed_keys_count + pressed_keys_count + pressed_count;
        keydata = ( int * )( calloc( total_size, sizeof( int ) ) );

        // Loop thru given list o’ key/input pairs & set KeyInputs & InputKeys,
        // incrementing their counts as we go.
        for ( int i = 0; i < num_o_inputs; ++i )
        {
            int * keyinputs = GetKeyInputs( key_to_glfw_map[ inputs[ i ].key ] );
            int * keycount = &keyinputs[ 0 ];
            keyinputs[ 1 + keycount[ 0 ] ] = inputs[ i ].id;
            ++*keycount;

            int * inputkeys = GetInputKeys( inputs[ i ].id );
            int * inputcount = &inputkeys[ 0 ];
            inputkeys[ 1 + inputcount[ 0 ] ] = key_to_glfw_map[ inputs[ i ].key ];
            ++*inputcount;
        }
    };

    void close()
    {
        free( keydata );
    };



    // Static Functions
    static int held( int id )
    {
        if ( id >= max_inputs )
        {
            BSL::log( "Held Error: invalid id #%d.", id );
            return 0;
        }
        return GetHeld( id )[ 0 ];
    };

    static int pressed( int id )
    {
        if ( id >= max_inputs )
        {
            BSL::log( "pressed Error: invalid id #%d.", id );
            return 0;
        }
        return GetPressed( id )[ 0 ];
    };

    static int * GetHeld( int id )
    {
        return &keydata[ held_start + id ];
    };

    static int * GetPressed( int id )
    {
        return &keydata[ pressed_start + id ];
    };

    static int * GetHeldKeys( int input )
    {
        return &keydata[ held_keys_start + ( keys_per_input * input ) ];
    };

    static int * GetPressedKeys( int input )
    {
        return &keydata[ pressed_keys_start + ( keys_per_input * input ) ];
    };

    static int * GetInputKeys( int input )
    {
        return &keydata[ input_keys_start + ( input_key_size * input ) ];
    };

    static int * GetKeyInputs( int key )
    {
        return &keydata[ key_input_size * key ];
    };

    static void HandleInput( void * window, int key, int scancode, int action, int mods )
    {
        if ( !keydata )
        {
            BSL::log( "HandleInput Error: key data not initialized." );
            return;
        }
        switch ( action )
        {
            case ( GLFW_PRESS ):
            {
                // Loop thru this key’s inputs so we can set them as held.
                int * inputs = GetKeyInputs( key );
                const int count = inputs[ 0 ];
                for ( int i = 0; i < count; ++i )
                {
                    const int input = inputs[ 1 + i ];
                    GetHeld( input )[ 0 ] = 1;
                    GetPressed( input )[ 0 ] = 1;
                    const int * input_keys = GetInputKeys( input );
                    const int input_keys_count = input_keys[ 0 ];
                    // While we can settle for just setting the 1D held list to 1, since if any key for an input is
                    // pressed, the input is considered pressed, we need to also register the specific key held in
                    // a list o’ keys for the input so we can check it later for release.
                    for ( int j = 0; j < input_keys_count; ++j )
                    {
                        if ( input_keys[ j + 1 ] == key )
                        {
                            GetPressedKeys( input )[ j ] = 1;
                            GetHeldKeys( input )[ j ] = 1;
                        }
                    }
                }
            }
            break;
            case ( GLFW_RELEASE ):
            {
                int * inputs = GetKeyInputs( key );
                const int count = inputs[ 0 ];
                for ( int i = 0; i < count; ++i )
                {
                    const int input = inputs[ 1 + i ];
                    // Unlike the press code, we need this for later, so keep pointer.
                    int * held = GetHeld( input );
                    int * pressed = GetPressed( input );
                    *held = *pressed = 0;
                    const int * input_keys = GetInputKeys( input );
                    const int input_keys_count = input_keys[ 0 ];
                    for ( int j = 0; j < input_keys_count; ++j )
                    {
                        // Unlike the press code, we need this for later, so keep pointer.
                        int * heldkeys = &GetHeldKeys( input )[ j ];
                        int * pressedkeys = &GetPressedKeys( input )[ j ];
                        if ( input_keys[ j + 1 ] == key )
                        {
                            *heldkeys = *pressedkeys = 0;
                        }
                        // This is a simple & efficient way to say that if any o’ the keys for this input are held, still
                        // consider it held.
                        held[ 0 ] = held[ 0 ] || *heldkeys;
                        pressed[ 0 ] = pressed[ 0 ] || *pressedkeys;
                    }
                }
            }
            break;
        }
    };
}