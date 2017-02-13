


// Name
//===================================
//
// InputComponent
//

#ifndef INPUT_COMPONENT_H
#define INPUT_COMPONENT_H


// FORWARD DECLARATIONS
//===================================

    class Input;


// DEPENDENCIES
//===================================


// CLASS
//===================================

    class InputComponent
    {
        public:
            virtual void update() = 0;
            virtual bool action1( Input& input ) = 0;
            virtual bool action2( Input& input ) = 0;
            virtual bool action3( Input& input ) = 0;
            virtual bool up( Input& input ) = 0;
            virtual bool down( Input& input ) = 0;
            virtual bool left( Input& input ) = 0;
            virtual bool right( Input& input ) = 0;
            virtual bool cUp( Input& input ) = 0;
            virtual bool cDown( Input& input ) = 0;
            virtual bool cLeft( Input& input ) = 0;
            virtual bool cRight( Input& input ) = 0;
    };


#endif // INPUT_COMPONENT_H
