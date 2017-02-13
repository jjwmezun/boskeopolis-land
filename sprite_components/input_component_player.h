



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
            bool action1( Input& input );
            bool action2( Input& input );
            bool action3( Input& input );
            bool up( Input& input );
            bool down( Input& input );
            bool left( Input& input );
            bool right( Input& input );
            bool cUp( Input& input );
            bool cDown( Input& input );
            bool cLeft( Input& input );
            bool cRight( Input& input );
    };


#endif // INPUT_COMPONENT_PLAYER_H
