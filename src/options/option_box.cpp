#include "option_box.hpp"
#include "render.hpp"
#include "unit.hpp"

static constexpr int SHADOW_LENGTH = 2;
static constexpr int SHADOW_COLOR = 6;

static int getBoxX( int x, int width, WTextObj::Align align )
{
	return ( align == WTextObj::Align::CENTER ) ? ( ( Unit::WINDOW_WIDTH_PIXELS - Unit::MiniBlocksToPixels( width ) ) / 2 ) : x;
};

OptionBox::OptionBox( const char32_t* text, int y, int width, int x, WTextObj::Align align )
:
	text_
	(
		text, x, y, WTextObj::Color::DARK_MID_GRAY, width,
		align, WTextObj::Color::__NULL, Unit::PIXELS_PER_MINIBLOCK,
		Unit::PIXELS_PER_MINIBLOCK, WTextObj::VAlign::CENTER, Unit::PIXELS_PER_MINIBLOCK
	),
	box_ ( getBoxX( x, width ), y, Unit::MiniBlocksToPixels( width ), BOX_HEIGHT ),
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
	text_.render();
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

WTextObj::Color OptionBox::getTextColor( int box_color ) const
{
	return ( state_ == OBState::NULLIFIED ) ? WTextObj::Color::DARK_MID_GRAY : ( WTextObj::Color )( box_color );
};

int OptionBox::currentTextPosition( int value ) const
{
	return ( state_ == OBState::NULLIFIED ) ? value + SHADOW_LENGTH : value;
};
