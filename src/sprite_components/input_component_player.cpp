



// Name
//===================================
//
// InputComponentPlayer
//


// DEPENDENCIES
//===================================

    #include "input.hpp"
    #include "input_component_player.hpp"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    void InputComponentPlayer::update() {};
    bool InputComponentPlayer::action1() { return Input::held( Input::Action::JUMP ); };
    bool InputComponentPlayer::action2() { return Input::held( Input::Action::RUN ); };
    bool InputComponentPlayer::action3() { return Input::held( Input::Action::MENU ); };
    bool InputComponentPlayer::up() { return Input::held( Input::Action::MOVE_UP ); };
    bool InputComponentPlayer::down() { return Input::held( Input::Action::MOVE_DOWN ); };
    bool InputComponentPlayer::left() { return Input::held( Input::Action::MOVE_LEFT ); };
    bool InputComponentPlayer::right() { return Input::held( Input::Action::MOVE_RIGHT ); };
    bool InputComponentPlayer::cUp() { return Input::held( Input::Action::CAMERA_UP ); };
    bool InputComponentPlayer::cDown() { return Input::held( Input::Action::CAMERA_DOWN ); };
    bool InputComponentPlayer::cLeft() { return Input::held( Input::Action::CAMERA_LEFT ); };
    bool InputComponentPlayer::cRight() { return Input::held( Input::Action::CAMERA_RIGHT ); };
