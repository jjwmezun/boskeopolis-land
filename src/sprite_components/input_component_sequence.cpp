#include <cstdio>
#include "input.hpp"
#include "input_component_sequence.hpp"
#include "title_state.hpp"
#include "main.hpp"

InputComponentSequence::InputComponentSequence( std::string local_file, bool trainer )
:
	sequence_of_inputs_ (),
	number_of_frames_ ( 0 ),
	current_frame_ ( 0 ),
	trainer_ ( trainer )
{
	const std::string file_path = getFilePath( local_file );
	FILE* file = fopen( file_path.c_str(), "r" );
	int c;
	int action = 0;
	if ( file == nullptr )
	{

	}
	else
	{
		do
		{
			c = getc( file );
			if ( action >= Input::NUM_O_ACTIONS )
			{
				action = 0;
				++number_of_frames_;
			}
			sequence_of_inputs_[ number_of_frames_ ][ action ] = c == 't';
			++action;
		} while ( c != EOF );
		fclose( file );
	}
};

InputComponentSequence::~InputComponentSequence() {};

void InputComponentSequence::update()
{
	++current_frame_;
	if ( trainer_ && current_frame_ >= number_of_frames_ )
	{
		Main::changeState( std::make_unique<TitleState> () );
	}
};

bool InputComponentSequence::action1() { return testFrame( Input::Action::JUMP         ); };
bool InputComponentSequence::action2() { return testFrame( Input::Action::RUN          ); };
bool InputComponentSequence::action3() { return testFrame( Input::Action::MENU         ); };
bool InputComponentSequence::up()      { return testFrame( Input::Action::MOVE_UP      ); };
bool InputComponentSequence::down()    { return testFrame( Input::Action::MOVE_DOWN    ); };
bool InputComponentSequence::left()    { return testFrame( Input::Action::MOVE_LEFT    ); };
bool InputComponentSequence::right()   { return testFrame( Input::Action::MOVE_RIGHT   ); };
bool InputComponentSequence::cUp()     { return testFrame( Input::Action::CAMERA_UP    ); };
bool InputComponentSequence::cDown()   { return testFrame( Input::Action::CAMERA_DOWN  ); };
bool InputComponentSequence::cLeft()   { return testFrame( Input::Action::CAMERA_LEFT  ); };
bool InputComponentSequence::cRight()  { return testFrame( Input::Action::CAMERA_RIGHT ); };

bool InputComponentSequence::testFrame( Input::Action action ) const
{
	return ( current_frame_ < number_of_frames_ ) ? sequence_of_inputs_[ current_frame_ ][ ( int )( action ) ] : false;
};

std::string InputComponentSequence::getFilePath( const std::string& local )
{
	return Main::resourcePath() + "recordings" + Main::pathDivider() + local + ".txt";
};