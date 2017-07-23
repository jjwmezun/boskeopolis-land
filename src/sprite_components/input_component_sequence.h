


// Name
//===================================
//
// InputComponentSequence
//

#ifndef INPUT_COMPONENT_SEQUENCE_H
#define INPUT_COMPONENT_SEQUENCE_H


// FORWARD DECLARATIONS
//===================================


// DEPENDENCIES
//===================================

    #include <array>
    #include "counter.h"
    #include "input.h"
    #include "input_component.h"
    #include <vector>


// CLASS
//===================================

    class InputComponentSequence : public InputComponent
    {
        public:
            InputComponentSequence( std::vector< std::array<bool, Input::NUM_O_ACTIONS> > sequence );
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

        private:
            const std::vector< std::array<bool, Input::NUM_O_ACTIONS> > sequence_of_inputs_;
            Counter frame_;
    };


#endif // INPUT_COMPONENT_SEQUENCE_H
