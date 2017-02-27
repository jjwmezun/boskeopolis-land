



// Name
//===================================
//
// InputComponentPlayer
//


// DEPENDENCIES
//===================================

    #include "input.h"
    #include "input_component_player.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    void InputComponentPlayer::update() {};
    bool InputComponentPlayer::action1( const Input& input ) { return input.held( Input::Action::JUMP ); };
    bool InputComponentPlayer::action2( const Input& input ) { return input.held( Input::Action::RUN ); };
    bool InputComponentPlayer::action3( const Input& input ) { return input.held( Input::Action::MENU ); };
    bool InputComponentPlayer::up( const Input& input ) { return input.held( Input::Action::MOVE_UP ); };
    bool InputComponentPlayer::down( const Input& input ) { return input.held( Input::Action::MOVE_DOWN ); };
    bool InputComponentPlayer::left( const Input& input ) { return input.held( Input::Action::MOVE_LEFT ); };
    bool InputComponentPlayer::right( const Input& input ) { return input.held( Input::Action::MOVE_RIGHT ); };
    bool InputComponentPlayer::cUp( const Input& input ) { return input.held( Input::Action::CAMERA_UP ); };
    bool InputComponentPlayer::cDown( const Input& input ) { return input.held( Input::Action::CAMERA_DOWN ); };
    bool InputComponentPlayer::cLeft( const Input& input ) { return input.held( Input::Action::CAMERA_LEFT ); };
    bool InputComponentPlayer::cRight( const Input& input ) { return input.held( Input::Action::CAMERA_RIGHT ); };
