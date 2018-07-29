#include "text_obj_basic.hpp"

void TextObjBasic::render( const Camera* camera ) const
{
	Text::renderText( words_, x_, y_, camera, color_, line_limit_, align_, shadow_, char_size_, chars_to_render_ );
};

TextObjBasic::TextObjBasic
(
	std::string words,
	int x,
	int y,
	Text::FontColor color,
	Text::FontAlign align,
	Text::FontColor shadow,
	bool center_y,
	unsigned int line_limit,
	int chars_to_render,
	int char_size
)
:
	words_ ( words ),
	x_ ( x ),
	y_ ( ( center_y ) ? Text::centerY( words_, ( ( line_limit == 0 ) ? Text::DEFAULT_LINE_LENGTH : line_limit ) ) : y ),
	color_ ( color ),
	shadow_ ( shadow ),
	align_ ( align ),
	line_limit_ ( ( line_limit == 0 ) ? Text::DEFAULT_LINE_LENGTH : line_limit ),
	chars_to_render_ ( chars_to_render ),
	char_size_ ( char_size )
{};
