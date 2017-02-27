


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
            virtual bool action1( const Input& input ) = 0;
            virtual bool action2( const Input& input ) = 0;
            virtual bool action3( const Input& input ) = 0;
            virtual bool up( const Input& input ) = 0;
            virtual bool down( const Input& input ) = 0;
            virtual bool left( const Input& input ) = 0;
            virtual bool right( const Input& input ) = 0;
            virtual bool cUp( const Input& input ) = 0;
            virtual bool cDown( const Input& input ) = 0;
            virtual bool cLeft( const Input& input ) = 0;
            virtual bool cRight( const Input& input ) = 0;
    };


#endif // INPUT_COMPONENT_H
