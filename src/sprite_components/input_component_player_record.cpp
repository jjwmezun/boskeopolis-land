#include <array>
#include "input_component_player_record.hpp"
#include <fstream>
#include "main.hpp"
#include <vector>

#include <iostream>

InputComponentPlayerRecord::InputComponentPlayerRecord()
:
    counter_ ( 0 )
{
    for ( int i = 0; i < InputComponentSequence::MAX_INPUTS; ++i )
    {
        for ( int action = 0; action < Input::NUM_O_ACTIONS; ++action )
        {
            recording_[ i ][ action ] = false;
        }
    }
};

InputComponentPlayerRecord::~InputComponentPlayerRecord()
{
    std::string text = "";
    for ( int i = 0; i < counter_; ++i )
    {
        for ( int action = 0; action < Input::NUM_O_ACTIONS; ++action )
        {
            text += ( recording_[ i ][ action ] ) ? "t" : "f";
        }
    }


    const std::string file_path = InputComponentSequence::getFilePath( "trainer" );
    std::ofstream out( file_path );
    out << text;
    out.close();
};

void InputComponentPlayerRecord::update() { if ( counter_ < InputComponentSequence::MAX_INPUTS - 1 ) { ++counter_; } };
bool InputComponentPlayerRecord::action1() { return act( Input::Action::JUMP ); };
bool InputComponentPlayerRecord::action2() { return act( Input::Action::RUN ); };
bool InputComponentPlayerRecord::action3() { return act( Input::Action::MENU ); };
bool InputComponentPlayerRecord::up() { return act( Input::Action::MOVE_UP ); };
bool InputComponentPlayerRecord::down() { return act( Input::Action::MOVE_DOWN ); };
bool InputComponentPlayerRecord::left() { return act( Input::Action::MOVE_LEFT ); };
bool InputComponentPlayerRecord::right() { return act( Input::Action::MOVE_RIGHT ); };
bool InputComponentPlayerRecord::cUp() { return act( Input::Action::CAMERA_UP ); };
bool InputComponentPlayerRecord::cDown() { return act( Input::Action::CAMERA_DOWN ); };
bool InputComponentPlayerRecord::cLeft() { return act( Input::Action::CAMERA_LEFT ); };
bool InputComponentPlayerRecord::cRight() { return act( Input::Action::CAMERA_RIGHT ); };

bool InputComponentPlayerRecord::act( Input::Action action )
{
    return recording_[ counter_ ][ ( int )( action ) ] = Input::held( action );
};