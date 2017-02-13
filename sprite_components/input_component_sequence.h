


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

        private:
            const std::vector< std::array<bool, Input::NUM_O_ACTIONS> > sequence_of_inputs_;
            Counter frame_;
    };


#endif // INPUT_COMPONENT_SEQUENCE_H
