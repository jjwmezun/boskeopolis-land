#pragma once

#include "input_component.hpp"

class InputComponentPlayer : public InputComponent
{
	public:
		~InputComponentPlayer();
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
};