#include "input_component_mirror.hpp"

InputComponentMirror::InputComponentMirror( const std::vector<std::array<bool, Input::NUM_O_ACTIONS>>* list )
:
    current_ ( -1 ),
    list_ ( list )
{};

InputComponentMirror::~InputComponentMirror() {};

void InputComponentMirror::update()
{
    ++current_;
};

void InputComponentMirror::reset()
{
    current_ = 0;
};

bool InputComponentMirror::action1() { return testFrame( Input::Action::JUMP         ); };
bool InputComponentMirror::action2() { return testFrame( Input::Action::RUN          ); };
bool InputComponentMirror::action3() { return testFrame( Input::Action::MENU         ); };
bool InputComponentMirror::up()      { return testFrame( Input::Action::MOVE_UP      ); };
bool InputComponentMirror::down()    { return testFrame( Input::Action::MOVE_DOWN    ); };
bool InputComponentMirror::left()    { return testFrame( Input::Action::MOVE_LEFT    ); };
bool InputComponentMirror::right()   { return testFrame( Input::Action::MOVE_RIGHT   ); };
bool InputComponentMirror::cUp()     { return testFrame( Input::Action::CAMERA_UP    ); };
bool InputComponentMirror::cDown()   { return testFrame( Input::Action::CAMERA_DOWN  ); };
bool InputComponentMirror::cLeft()   { return testFrame( Input::Action::CAMERA_LEFT  ); };
bool InputComponentMirror::cRight()  { return testFrame( Input::Action::CAMERA_RIGHT ); };

bool InputComponentMirror::testFrame( Input::Action action ) const
{
	return ( current_ >= 0 ) ? list_->at( current_ )[ ( int )( action ) ] : false;
};
