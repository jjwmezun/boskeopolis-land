



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
    bool InputComponentPlayer::action1( Input& input ) { return input.held( Input::Action::JUMP ); };
    bool InputComponentPlayer::action2( Input& input ) { return input.held( Input::Action::RUN ); };
    bool InputComponentPlayer::action3( Input& input ) { return input.held( Input::Action::MENU ); };
    bool InputComponentPlayer::up( Input& input ) { return input.held( Input::Action::MOVE_UP ); };
    bool InputComponentPlayer::down( Input& input ) { return input.held( Input::Action::MOVE_DOWN ); };
    bool InputComponentPlayer::left( Input& input ) { return input.held( Input::Action::MOVE_LEFT ); };
    bool InputComponentPlayer::right( Input& input ) { return input.held( Input::Action::MOVE_RIGHT ); };
    bool InputComponentPlayer::cUp( Input& input ) { return input.held( Input::Action::CAMERA_UP ); };
    bool InputComponentPlayer::cDown( Input& input ) { return input.held( Input::Action::CAMERA_DOWN ); };
    bool InputComponentPlayer::cLeft( Input& input ) { return input.held( Input::Action::CAMERA_LEFT ); };
    bool InputComponentPlayer::cRight( Input& input ) { return input.held( Input::Action::CAMERA_RIGHT ); };
