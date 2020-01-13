#pragma once

#include "input.hpp"
#include "input_component.hpp"
#include "input_component_sequence.hpp"

class InputComponentPlayerRecord : public InputComponent
{
	public:
        InputComponentPlayerRecord();
		~InputComponentPlayerRecord();
		void update() override;
		bool action1() override;
		bool action2() override;
		bool action3() override;
		bool up() override;
		bool down() override;
		bool left() override;
		bool right() override;
		bool cUp() override;
		bool cDown() override;
		bool cLeft() override;
		bool cRight() override;
    
    private:
        bool act( Input::Action action );
        bool recording_[ InputComponentSequence::MAX_INPUTS ][ Input::NUM_O_ACTIONS ] = { false };
        int counter_;
};