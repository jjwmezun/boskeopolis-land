#include "audio.hpp"
#include "text_obj.hpp"
#include "text_component_gradual.hpp"

TextComponentGradual::TextComponentGradual( int speed ) : speed_ ( speed ), timer_ ( 0 ) {};
TextComponentGradual::~TextComponentGradual() {};

void TextComponentGradual::update( TextObj& text )
{
	if ( text.chars_to_render_ < 0 ) text.chars_to_render_ = 0;

	if ( text.chars_to_render_ < text.words_.length() )
	{
		if ( timer_ >= speed_ - 1 )
		{
			timer_ = 0;
			++text.chars_to_render_;
			Audio::playSound( Audio::SoundType::SELECT );
		}
		else
		{
			++timer_;
		}
	}
};