



// Name
//===================================
//
// InputComponentPlayer
//

#ifndef INPUT_COMPONENT_PLAYER_H
#define INPUT_COMPONENT_PLAYER_H


// FORWARD DECLARATIONS
//===================================


// DEPENDENCIES
//===================================

    #include "input_component.h"


// CLASS
//===================================

    class InputComponentPlayer : public InputComponent
    {
        public:
            void update();
            bool action1( const Input& input );
            bool action2( const Input& input );
            bool action3( const Input& input );
            bool up( const Input& input );
            bool down( const Input& input );
            bool left( const Input& input );
            bool right( const Input& input );
            bool cUp( const Input& input );
            bool cDown( const Input& input );
            bool cLeft( const Input& input );
            bool cRight( const Input& input );
    };


#endif // INPUT_COMPONENT_PLAYER_H
