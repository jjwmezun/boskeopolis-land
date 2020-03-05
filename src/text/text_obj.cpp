#include "text_obj.hpp"

void TextObj::render( const Camera* camera ) const
{
	Text::renderText( words_, x_, y_, camera, color_, line_limit_, align_, shadow_, char_size_, chars_to_render_ );
};

void TextObj::update()
{
	if ( component_ ) { component_->update( *this ); }
};

TextObj::TextObj
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
	int char_size,
	std::unique_ptr<TextComponent> component
)
:
	words_ ( words ),
	x_ ( x ),
	y_ ( ( center_y ) ? Text::centerY( words_, ( ( line_limit == 0 ) ? Text::DEFAULT_LINE_LENGTH : line_limit ) ) : y ),
	color_ ( color ),
	shadow_ ( shadow ),
	align_ ( align ),
	component_ ( std::move( component ) ),
	line_limit_ ( ( line_limit == 0 ) ? Text::DEFAULT_LINE_LENGTH : line_limit ),
	chars_to_render_ ( chars_to_render ),
	char_size_ ( char_size )
{};

TextObj::~TextObj() noexcept {};

TextObj::TextObj( TextObj&& m ) noexcept
:
	words_ ( m.words_ ),
	x_ ( m.x_ ),
	y_ ( m.y_ ),
	color_ ( m.color_ ),
	shadow_ ( m.shadow_ ),
	align_ ( m.align_ ),
	component_ ( std::move( m.component_ ) ),
	line_limit_ ( m.line_limit_ ),
	chars_to_render_ ( m.chars_to_render_ ),
	char_size_ ( m.char_size_ )
{};

TextObj& TextObj::operator=( TextObj&& m ) noexcept
{
	words_ = m.words_;
	x_ = m.x_;
	y_ = m.y_;
	color_ = m.color_;
	shadow_ = m.shadow_;
	align_ = m.align_;
	component_ = std::move( m.component_ );
	line_limit_ = m.line_limit_;
	chars_to_render_ = m.chars_to_render_;
	char_size_ = m.char_size_;
};

int TextObj::right() const
{
	return x_ + width();
};

int TextObj::width() const
{
	return words_.size() * Text::CHAR_SIZE_PIXELS;
};
