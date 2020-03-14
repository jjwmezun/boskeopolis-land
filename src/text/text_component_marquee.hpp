#pragma once

#include "direction.hpp"
#include "text_component.hpp"

class TextComponentMarquee final : public TextComponent
{
	public:
		TextComponentMarquee
		(
			Direction::Horizontal direction = Direction::Horizontal::LEFT,
			int speed = 1
		);
		~TextComponentMarquee();
		void update( TextObj& text ) override;

	private:
		const Direction::Horizontal direction_;
		const int speed_;

		void moveLeft( TextObj& text );
		void moveRight( TextObj& text );
		void overrideLineLimit( TextObj& text );
};