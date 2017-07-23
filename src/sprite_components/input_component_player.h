



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
            bool action1();
            bool action2();
            bool action3();
            bool up();
            bool down();
            bool left();
            bool right();
            bool cUp();
            bool cDown();
            bool cLeft();
            bool cRight();
    };


#endif // INPUT_COMPONENT_PLAYER_H
