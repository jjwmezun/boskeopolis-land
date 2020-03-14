#pragma once

#include "text_component.hpp"
#include "unit.hpp"

class TextComponentFlashing final : public TextComponent
{
	public:
		TextComponentFlashing( int speed = Unit::DEFAULT_ANIMATION_SPEED );
		~TextComponentFlashing();
		void update( TextObj& text ) override;
	
	private:
		int speed_;
		int frame_;
		int timer_;
};