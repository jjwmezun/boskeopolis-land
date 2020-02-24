#include "clock.hpp"
#include "inventory.hpp"
#include "inventory_level.hpp"
#include "level_state.hpp"
#include "main.hpp"
#include "render.hpp"
#include "time_start_state.hpp"
#include "wtext_obj.hpp"

static constexpr int MOVE_SPEED = 4;
static constexpr int SIZE_SPEED = 1;
static constexpr int NUM_O_CHAR = 4;
static constexpr int START_SIZE = 12;
static constexpr int HALF_SIZE = START_SIZE / 2;
static constexpr int TIMER_HEIGHT = START_SIZE * Unit::PIXELS_PER_MINIBLOCK;
static constexpr int SRC_WIDTH = WTextCharacter::SIZE_PIXELS * 4;
static constexpr int SRC_HEIGHT = WTextCharacter::SIZE_PIXELS;
static constexpr int DEST_WIDTH = START_SIZE * SRC_WIDTH;
static constexpr int DEST_HEIGHT = START_SIZE * SRC_HEIGHT;
static constexpr int DEST_X = ( int )( ( double )( Unit::WINDOW_WIDTH_PIXELS - DEST_WIDTH ) / 2.0 );
static constexpr int DEST_Y = ( int )( ( double )( Unit::WINDOW_HEIGHT_PIXELS - InventoryLevelGraphics::HEIGHT - DEST_HEIGHT ) / 2.0 );
static constexpr int TIMER_WIDTH = START_SIZE * Unit::PIXELS_PER_MINIBLOCK * NUM_O_CHAR;
static constexpr int STOPPING_POINT = ( ( Unit::WINDOW_HEIGHT_PIXELS - InventoryLevelGraphics::HEIGHT ) / 2 ) - ( TIMER_HEIGHT / 2 );
static constexpr int BUMP_LENGTH = 4;
static constexpr int MOVEMENT_FRAMES = SIZE_SPEED * START_SIZE;
static constexpr int MOVEMENT_PER_FRAME_Y = ( int )( ceil( ( ( double )( InventoryLevelGraphics::Y ) - ( double )( STOPPING_POINT ) ) / ( double )( MOVEMENT_FRAMES ) ) );
static constexpr int MOVEMENT_PER_FRAME_X = ( int )( ceil( ( ( double )( InventoryLevelGraphics::CLOCK_X ) - ( double )( DEST_X ) ) / ( double )( MOVEMENT_FRAMES ) ) );
static constexpr int BLINK_SPEED = 8;
static constexpr int NUM_O_BLINKS = 3;

TimeStartState::TimeStartState( const Palette& palette, std::u32string text )
:
	GameState( StateID::TIME_START_STATE, palette ),
	y_ ( DEST_Y ),
	x_ ( DEST_X ),
	size_ ( START_SIZE ),
	timer_ ( 0 ),
	color_ ( 1 ),
	state_ ( State::GOING_DOWN ),
	textures_ (),
	src_ ( 0, 0, SRC_WIDTH, SRC_HEIGHT ),
	dest_ ( DEST_X, DEST_Y, DEST_WIDTH, DEST_HEIGHT ),
	text_ ( text )
{};

TimeStartState::~TimeStartState()
{
	for ( int i = 0; i < NUMBER_OF_COLORS; ++i )
	{
		SDL_DestroyTexture( textures_[ i ] );
	}
};

void TimeStartState::stateUpdate()
{
	switch ( state_ )
	{
		case ( State::GOING_DOWN ):
			if ( y_ >= STOPPING_POINT + BUMP_LENGTH )
			{
				state_ = State::BUMPING_UPWARD;
			}
			else
			{
				y_ += MOVE_SPEED;
			}
		break;
		case ( State::BUMPING_UPWARD ):
			if ( y_ <= STOPPING_POINT - BUMP_LENGTH )
			{
				state_ = State::BUMPING_BACK_DOWN;
			}
			else
			{
				y_ -= MOVE_SPEED;
			}
		break;
		case ( State::BUMPING_BACK_DOWN ):
			if ( y_ >= STOPPING_POINT )
			{
				state_ = State::BLINKING;
				y_ = STOPPING_POINT;
			}
			else
			{
				y_ += MOVE_SPEED;
			}
		break;
		case ( State::BLINKING ):
			++timer_;
			if ( timer_ >= BLINK_SPEED * NUM_O_BLINKS * 2 )
			{
				color_ = 1;
				timer_ = 0; // Reset so it can be reused by next state.
				state_ = State::SHRINKING;
			}
			else
			{
				if ( timer_ % BLINK_SPEED == 0 )
				{
					color_ = ( color_ == 1 ) ? 0 : 1;
				}
			}
		break;
		case ( State::SHRINKING ):
			++timer_;

			y_ += MOVEMENT_PER_FRAME_Y;
			if ( y_ > InventoryLevelGraphics::TOP_ROW_Y )
			{
				y_ = InventoryLevelGraphics::TOP_ROW_Y;
			}

			x_ -= MOVEMENT_PER_FRAME_X;
			if ( x_ < InventoryLevelGraphics::CLOCK_X )
			{
				x_ = InventoryLevelGraphics::CLOCK_X;
			}

			dest_.x = x_;
			dest_.y = y_;

			if ( timer_ % SIZE_SPEED == 0 )
			{
				if ( size_ > 1 )
				{
					--size_;
					dest_.w = SRC_WIDTH * size_;
					dest_.h = SRC_HEIGHT * size_;
				}
			}

			if ( size_ <= HALF_SIZE )
			{
				color_ = 2;
			}
			
			if ( timer_ >= MOVEMENT_FRAMES )
			{
				state_ = State::DONE;
			}
		break;
		case ( State::DONE ):
			Main::popState();
		break;
	}
};

void TimeStartState::stateRender()
{
	Render::renderObject( textures_[ color_ ], src_, dest_ );
};

void TimeStartState::init()
{
	constexpr WTextCharacter::Color colors[ 3 ] =
	{
		WTextCharacter::Color::LIGHT_GRAY,
		WTextCharacter::Color::WHITE,
		WTextCharacter::Color::DARK_GRAY
	};
	for ( int i = 0; i < NUMBER_OF_COLORS; ++i )
	{
		const WTextCharacter::Color shadow = ( i == 2 ) ? WTextCharacter::Color::__NULL : WTextCharacter::Color::BLACK;
		textures_[ i ] = Render::createRenderBox( src_.w, src_.h );
		Render::setRenderTarget( textures_[ i ] );
		WTextObj text( text_, 0, 0, colors[ i ], src_.w, WTextObj::Align::LEFT, shadow );
		text.render();
		Render::releaseRenderTarget();
	}
};