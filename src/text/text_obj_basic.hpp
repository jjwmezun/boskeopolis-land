#pragma once

#include <string>
#include "text.hpp"

class Camera;

struct TextObjBasic final
{
	std::string words_;
	Text::FontColor color_;
	Text::FontColor shadow_;
	Text::FontAlign align_;
	unsigned int line_limit_;
	int x_;
	int y_;
	int char_size_;
	int chars_to_render_;

	TextObjBasic
	(
		std::string words,
		int x = 0,
		int y = 0,
		Text::FontColor color = Text::FontColor::BLACK,
		Text::FontAlign align = Text::FontAlign::LEFT,
		Text::FontColor shadow = Text::FontColor::__NULL,
		bool center_y = false,
		unsigned int line_limit = Text::DEFAULT_LINE_LENGTH,
		int chars_to_render = -1,
		int char_size = 1
	);

	void render( const Camera* camera = nullptr ) const;
};
