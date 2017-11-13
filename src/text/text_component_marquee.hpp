#pragma once

#include "direction.hpp"
#include "text_component.hpp"

class TextComponentMarquee : public TextComponent
{
	public:
		TextComponentMarquee
		(
			Direction::Horizontal direction = Direction::Horizontal::LEFT,
			int speed = 1
		);
		~TextComponentMarquee();
		void update( Text& text );

	private:
		const Direction::Horizontal direction_;
		const int speed_;

		void moveLeft( Text& text );
		void moveRight( Text& text );
		void overrideLineLimit( Text& text );
};