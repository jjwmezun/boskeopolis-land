#pragma once

#include <string>
#include "text_component.hpp"
#include "text.hpp"

class Camera;

struct TextObj
{
	std::string words_;
	std::unique_ptr<TextComponent> component_;
	Text::FontColor color_;
	Text::FontColor shadow_;
	Text::FontAlign align_;
	unsigned int line_limit_;
	int x_;
	int y_;
	int char_size_;
	int chars_to_render_;

	void update();
	void render( const Camera* camera = nullptr ) const;
	int right() const;
	int width() const;

	TextObj
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
		int char_size = 1,
		std::unique_ptr<TextComponent> component = nullptr
	);
	~TextObj() noexcept;
	TextObj( TextObj&& m ) noexcept;
	TextObj& operator=( TextObj&& m ) noexcept;
	TextObj( const TextObj& ) = delete;
	TextObj& operator= ( const TextObj& ) = delete;
};