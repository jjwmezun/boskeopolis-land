#pragma once

#include <array>
#include "counter.hpp"
#include "input.hpp"
#include "input_component.hpp"
#include <vector>

class InputComponentSequence : public InputComponent
{
	public:
		InputComponentSequence( std::vector<std::array<bool, Input::NUM_O_ACTIONS>>&& sequence );
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

	private:
		const std::vector< std::array<bool, Input::NUM_O_ACTIONS> > sequence_of_inputs_;
		Counter frame_;
};