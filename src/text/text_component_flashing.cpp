#include "text_obj.hpp"
#include "text_component_flashing.hpp"

static constexpr int MAX_FRAMES = 12;
static constexpr Text::FontColor FRAMES[ MAX_FRAMES ] =
{
	Text::FontColor::BLACK,
	Text::FontColor::DARK_GRAY,
	Text::FontColor::DARK_MID_GRAY,
	Text::FontColor::LIGHT_MID_GRAY,
	Text::FontColor::LIGHT_GRAY,
	Text::FontColor::WHITE,
	Text::FontColor::WHITE,
	Text::FontColor::LIGHT_GRAY,
	Text::FontColor::LIGHT_MID_GRAY,
	Text::FontColor::DARK_MID_GRAY,
	Text::FontColor::DARK_GRAY,
	Text::FontColor::BLACK
};

TextComponentFlashing::TextComponentFlashing( int speed ) : speed_ ( speed ), frame_ ( 0 ), timer_ ( 0 ) {};
TextComponentFlashing::~TextComponentFlashing() {};

void TextComponentFlashing::update( TextObj& text )
{
	if ( timer_ >= speed_ )
	{
		timer_ = 0;
		++frame_;
		if ( frame_ >= MAX_FRAMES )
		{
			frame_ = 0;
		}
		text.color_ = FRAMES[ frame_ ];
	}
	else
	{
		++timer_;
	}
};