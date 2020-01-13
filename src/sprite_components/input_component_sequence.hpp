#pragma once

#include <array>
#include "counter.hpp"
#include "input.hpp"
#include "input_component.hpp"
#include <vector>

class InputComponentSequence : public InputComponent
{
	public:
        static constexpr int MAX_INPUTS = 60 * 60 * 60 * 10;

		InputComponentSequence( std::string local_file, bool trainer = false );
		~InputComponentSequence();
		void update();
		bool action1();
		bool action2();
		bool action3();
		bool up();
		bool down();
		bool left();
		bool right();
		bool cUp();
		bool cDown();
		bool cLeft();
		bool cRight();

		static std::string getFilePath( const std::string& local );

	private:
		bool testFrame( Input::Action action ) const;

		bool sequence_of_inputs_[ MAX_INPUTS ][ Input::NUM_O_ACTIONS ];
		int number_of_frames_;
		int current_frame_;
		bool trainer_;
};