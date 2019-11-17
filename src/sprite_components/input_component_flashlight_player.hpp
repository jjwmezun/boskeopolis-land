#pragma once

#include "input_component.hpp"

class InputComponentFlashlightPlayer : public InputComponent
{
	public:
		~InputComponentFlashlightPlayer();
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
};