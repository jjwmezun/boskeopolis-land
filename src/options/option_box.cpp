#include "option_box.hpp"
#include "render.hpp"
#include "unit.hpp"

static constexpr int SHADOW_LENGTH = 2;
static constexpr int SHADOW_COLOR = 6;

static int getBoxX( int x, int width )
{
	return ( x == -1 ) ? ( ( Unit::WINDOW_WIDTH_PIXELS - width ) / 2 ) : x;
};

OptionBox::OptionBox( const std::u32string& words, int y, int width, int x )
:
	words_ ( words ),
	box_ ( getBoxX( x, width ), y, width, BOX_HEIGHT ),
	shadow_box_ ( box_.x + SHADOW_LENGTH, box_.y + SHADOW_LENGTH, width, BOX_HEIGHT ),
	width_ ( width ),
	x_ ( getBoxX( x, width ) ),
	y_ ( y ),
	box_color_ ( 5 ),
	state_ ( OBState::NORMAL ),
	timer_ ( 0 )
{};

OptionBox::~OptionBox()
{
	for ( int i = 0; i < WTextObj::NUMBER_OF_COLORS; ++i )
	{
		text_[ i ].destroy();
	}
};

OptionBox::OptionBox( OptionBox&& o )
:
	words_ ( o.words_ ),
	box_ ( o.box_ ),
	shadow_box_ ( o.shadow_box_ ),
	width_ ( o.width_ ),
	x_ ( o.x_ ),
	y_ ( o.y_ ),
	box_color_ ( o.box_color_ ),
	state_ ( o.state_ ),
	timer_ ( o.timer_ )
{};

void OptionBox::init()
{
	for ( int i = 0; i < WTextObj::NUMBER_OF_COLORS - 1; ++i )
	{
		WTextObj::generateTexture
		(
			text_[ i ],
			words_, x_, y_, ( WTextObj::Color )( i ),
			width_, WTextObj::Align::CENTER, WTextObj::Color::__NULL,
			Unit::PIXELS_PER_MINIBLOCK, Unit::PIXELS_PER_MINIBLOCK,
			WTextObj::VAlign::CENTER, BOX_HEIGHT
		);
	}
	WTextObj::generateTexture
	(
		text_[ WTextObj::NUMBER_OF_COLORS - 1 ],
		words_, x_ + 2, y_ + 2, ( WTextObj::Color )( 2 ),
		width_, WTextObj::Align::CENTER, WTextObj::Color::__NULL,
		Unit::PIXELS_PER_MINIBLOCK, Unit::PIXELS_PER_MINIBLOCK,
		WTextObj::VAlign::CENTER, BOX_HEIGHT
	);
}

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
	text_[ box_color_ - 1 ].render();
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

int OptionBox::currentTextPosition( int value ) const
{
	return ( state_ == OBState::NULLIFIED ) ? value + SHADOW_LENGTH : value;
};

void OptionBox::changeText( const std::u32string& words )
{
	words_ = words;
	init();
};

int OptionBox::calculateMaxBoxWidthWithPadding( const std::vector<std::u32string>& options )
{
	int max_width = 0;
	for ( const std::u32string& name : options )
	{
		if ( name.size() > max_width )
		{
			max_width = name.size();
		}
	}
	if ( max_width > Unit::MINIBLOCKS_PER_SCREEN - 4 )
	{
		max_width = Unit::MINIBLOCKS_PER_SCREEN - 4;
	}
	return ( max_width + 2 ) * CharFrame::SIZE_PIXELS;
}