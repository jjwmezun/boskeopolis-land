#pragma once

class InputComponent
{
	public:
		virtual ~InputComponent();
		virtual void update() = 0;
		virtual void reset();
		virtual bool action1() = 0;
		virtual bool action2() = 0;
		virtual bool action3() = 0;
		virtual bool up() = 0;
		virtual bool down() = 0;
		virtual bool left() = 0;
		virtual bool right() = 0;
		virtual bool cUp() = 0;
		virtual bool cDown() = 0;
		virtual bool cLeft() = 0;
		virtual bool cRight() = 0;
};