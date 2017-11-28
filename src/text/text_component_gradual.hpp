#pragma once

#include "text_component.hpp"
#include "unit.hpp"

class TextComponentGradual : public TextComponent
{
	public:
		TextComponentGradual( int speed = Unit::DEFAULT_ANIMATION_SPEED );
		~TextComponentGradual();
		void update( TextObj& text ) override;
	
	private:
		int speed_;
		int timer_;
};