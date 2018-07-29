#include "option_box.hpp"
#include "render.hpp"

static constexpr int SHADOW_LENGTH = 2;
static constexpr int SHADOW_COLOR = 6;

static bool isCenteredX( int x )
{
	return ( x < 0 );
};

static int getBoxX( int x, int width )
{
	return ( isCenteredX( x ) ) ? ( Unit::WINDOW_WIDTH_PIXELS / 2 ) - ( Unit::MiniBlocksToPixels( width ) / 2 ) : x;
};

static int getInitTextX( int x, int width, const std::string& words )
{
	return isCenteredX( x ) ? 0 : std::max( x, x + Unit::MiniBlocksToPixels( ( width - words.size() ) / 2 ) );
};

OptionBox::OptionBox( std::string words, int y, int width, int x )
:
	box_ ( getBoxX( x, width ), y, Unit::MiniBlocksToPixels( width ), BOX_HEIGHT ),
	words_ ( words ),
	text_y_ ( y + VERTICAL_PADDING_PIXELS ),
	text_x_ ( getInitTextX( x, width, words ) ),
	text_align_ ( isCenteredX( x ) ? Text::FontAlign::CENTER : Text::FontAlign::LEFT ),
	shadow_box_ ( box_.x + SHADOW_LENGTH, box_.y + SHADOW_LENGTH, Unit::MiniBlocksToPixels( width ), BOX_HEIGHT ),
	box_color_ ( 5 ),
	state_ ( OBState::NORMAL ),
	timer_ ( 0 )
{};

OptionBox::~OptionBox() {};

void OptionBox::update()
{
	switch ( state_ )
	{
		case ( OBState::NORMAL ):
			box_color_ = 5;
		break;
		case ( OBState::CURRENT ):
		{
			switch ( timer_ )
			{
				case ( 0 ):
				case ( 1 ):
					box_color_ = 4;
				break;
				case ( 2 ):
				case ( 3 ):
					box_color_ = 3;
				break;
				case ( 4 ):
				case ( 5 ):
					box_color_ = 2;
				break;
				default:
					box_color_ = 1;
				break;
			}
			++timer_;
		}
		break;
		case ( OBState::PREVIOUS ):
		{
			switch ( timer_ )
			{
				case ( 0 ):
				case ( 1 ):
					box_color_ = 2;
				break;
				case ( 2 ):
				case ( 3 ):
					box_color_ = 3;
				break;
				case ( 4 ):
				case ( 5 ):
					box_color_ = 4;
				break;
				default:
					box_color_ = 5;
				break;
			}
			++timer_;
		}
		break;
		case ( OBState::NULLIFIED ):
			box_color_ = 6;
		break;
	}
};

void OptionBox::render() const
{
	renderShadow();
	renderBox();
	renderText();
};

void OptionBox::setToNormal()
{
	timer_ = 6; // Go past color animation.
	state_ = OBState::NORMAL;
};

void OptionBox::setToCurrent()
{
	if ( state_ != OBState::CURRENT )
	{
		state_ = OBState::CURRENT;
		timer_ = 0;
	}
};

void OptionBox::setToPrevious()
{
	if ( state_ != OBState::PREVIOUS )
	{
		state_ = OBState::PREVIOUS;
		timer_ = 0;
	}
};

void OptionBox::setToNullified()
{
	timer_ = 6; // Go past color animation.
	state_ = OBState::NULLIFIED;
};

bool OptionBox::isNullified() const
{
	return state_ == OBState::NULLIFIED;;
};

void OptionBox::renderShadow() const
{
	Render::renderRect( shadow_box_, SHADOW_COLOR );
};

void OptionBox::renderBox() const
{
	if ( state_ != OBState::NULLIFIED )
	{
		Render::renderRect( box_, box_color_ );
	}
};

void OptionBox::renderText() const
{
	Text::renderText( words_, currentTextPosition( text_x_ ), currentTextPosition( text_y_ ), nullptr, getTextColor( box_color_ ), Text::DEFAULT_LINE_LENGTH, text_align_ );
};

Text::FontColor OptionBox::getTextColor( int box_color ) const
{
	return ( state_ == OBState::NULLIFIED ) ? Text::FontColor::DARK_MID_GRAY : ( Text::FontColor )( box_color );
};

int OptionBox::currentTextPosition( int value ) const
{
	return ( state_ == OBState::NULLIFIED ) ? value + SHADOW_LENGTH : value;
};
