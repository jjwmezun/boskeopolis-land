#include "input.hpp"
#include "input_component_flashlight_player.hpp"

InputComponentFlashlightPlayer::~InputComponentFlashlightPlayer() {};

void InputComponentFlashlightPlayer::update() {};
bool InputComponentFlashlightPlayer::action1() { return Input::held( Input::Action::JUMP ); };
bool InputComponentFlashlightPlayer::action2() { return Input::held( Input::Action::RUN ); };
bool InputComponentFlashlightPlayer::action3() { return Input::held( Input::Action::MENU ); };
bool InputComponentFlashlightPlayer::up() { return false; };
bool InputComponentFlashlightPlayer::down() { return false; };
bool InputComponentFlashlightPlayer::left() { return Input::held( Input::Action::MOVE_LEFT ); };
bool InputComponentFlashlightPlayer::right() { return Input::held( Input::Action::MOVE_RIGHT ); };
bool InputComponentFlashlightPlayer::cUp() { return false; };
bool InputComponentFlashlightPlayer::cDown() { return false; };
bool InputComponentFlashlightPlayer::cLeft() { return false; };
bool InputComponentFlashlightPlayer::cRight() { return false; };
