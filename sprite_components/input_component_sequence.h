


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

        private:
            const std::vector< std::array<bool, Input::NUM_O_ACTIONS> > sequence_of_inputs_;
            Counter frame_;
    };


#endif // INPUT_COMPONENT_SEQUENCE_H
