#include "input.hpp"
#include "input_component_sequence.hpp"

InputComponentSequence::InputComponentSequence( std::vector<std::array<bool, Input::NUM_O_ACTIONS>>&& sequence )
:
	sequence_of_inputs_ ( std::move( sequence ) ),
	frame_ ( { 0, ( int )( sequence_of_inputs_.size() ) - 1 } )
{};

InputComponentSequence::~InputComponentSequence() {};

void InputComponentSequence::update()
{
	++frame_;
};

bool InputComponentSequence::action1() { return ( frame_.value() < ( int )( sequence_of_inputs_.size() ) ) ? sequence_of_inputs_[ frame_.value() ][ (int)Input::Action::JUMP ] : false; };
bool InputComponentSequence::action2() { return ( frame_.value() < ( int )( sequence_of_inputs_.size() ) ) ? sequence_of_inputs_[ frame_.value() ][ (int)Input::Action::RUN ] : false; };
bool InputComponentSequence::action3() { return ( frame_.value() < ( int )( sequence_of_inputs_.size() ) ) ? sequence_of_inputs_[ frame_.value() ][ (int)Input::Action::MENU ] : false; };
bool InputComponentSequence::up() { return ( frame_.value() < ( int )( sequence_of_inputs_.size() ) ) ? sequence_of_inputs_[ frame_.value() ][ (int)Input::Action::MOVE_UP ] : false; };
bool InputComponentSequence::down() { return ( frame_.value() < ( int )( sequence_of_inputs_.size() ) ) ? sequence_of_inputs_[ frame_.value() ][ (int)Input::Action::MOVE_DOWN ] : false; };
bool InputComponentSequence::left() { return ( frame_.value() < ( int )( sequence_of_inputs_.size() ) ) ? sequence_of_inputs_[ frame_.value() ][ (int)Input::Action::MOVE_LEFT ] : false; };
bool InputComponentSequence::right() { return ( frame_.value() < ( int )( sequence_of_inputs_.size() ) ) ? sequence_of_inputs_[ frame_.value() ][ (int)Input::Action::MOVE_RIGHT ] : false; };
bool InputComponentSequence::cUp() { return ( frame_.value() < ( int )( sequence_of_inputs_.size() ) ) ? sequence_of_inputs_[ frame_.value() ][ (int)Input::Action::CAMERA_UP ] : false; };
bool InputComponentSequence::cDown() { return ( frame_.value() < ( int )( sequence_of_inputs_.size() ) ) ? sequence_of_inputs_[ frame_.value() ][ (int)Input::Action::CAMERA_DOWN ] : false; };
bool InputComponentSequence::cLeft() { return ( frame_.value() < ( int )( sequence_of_inputs_.size() ) ) ? sequence_of_inputs_[ frame_.value() ][ (int)Input::Action::CAMERA_LEFT ] : false; };
bool InputComponentSequence::cRight() { return ( frame_.value() < ( int )( sequence_of_inputs_.size() ) ) ? sequence_of_inputs_[ frame_.value() ][ (int)Input::Action::CAMERA_RIGHT ] : false; };