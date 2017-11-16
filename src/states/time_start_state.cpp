#include "clock.hpp"
#include "inventory.hpp"
#include "inventory_level.hpp"
#include "main.hpp"
#include "time_start_state.hpp"

static constexpr int MOVE_SPEED = 4;
static constexpr int SIZE_SPEED = 1;
static constexpr int NUM_O_CHAR = 4;
static constexpr int START_SIZE = 12;
static constexpr int HALF_SIZE = START_SIZE / 2;
static constexpr int TIMER_HEIGHT = START_SIZE * Unit::PIXELS_PER_MINIBLOCK;
static constexpr int START_Y = -TIMER_HEIGHT;
static constexpr int TIMER_WIDTH = START_SIZE * Unit::PIXELS_PER_MINIBLOCK * NUM_O_CHAR;
static constexpr int START_X = ( Unit::WINDOW_WIDTH_PIXELS / 2 ) - ( TIMER_WIDTH / 2 );
static constexpr int STOPPING_POINT = ( ( Unit::WINDOW_HEIGHT_PIXELS - InventoryLevel::HEIGHT ) / 2 ) - ( TIMER_HEIGHT / 2 );
static constexpr int BUMP_LENGTH = 4;
static constexpr int MOVEMENT_FRAMES = SIZE_SPEED * START_SIZE;
static constexpr int MOVEMENT_PER_FRAME_Y = ( int )( ceil( ( ( double )( InventoryLevel::Y ) - ( double )( STOPPING_POINT ) ) / ( double )( MOVEMENT_FRAMES ) ) );
static constexpr int MOVEMENT_PER_FRAME_X = ( int )( ceil( ( ( double )( InventoryLevel::CLOCK_X ) - ( double )( START_X ) ) / ( double )( MOVEMENT_FRAMES ) ) );
static constexpr int BLINK_SPEED = 8;
static constexpr int NUM_O_BLINKS = 3;

TimeStartState::TimeStartState( const Palette& palette )
:
	GameState( StateID::TIME_START_STATE, palette ),
	y_ ( START_Y ),
	x_ ( START_X ),
	size_ ( START_SIZE ),
	timer_ ( 0 ),
	color_ ( Text::FontColor::WHITE ),
	state_ ( State::GOING_DOWN )
{};

TimeStartState::~TimeStartState() {};

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
				color_ = Text::FontColor::WHITE;
				timer_ = 0; // Reset so it can be reused by next state.
				state_ = State::SHRINKING;
			}
			else
			{
				if ( timer_ % BLINK_SPEED == 0 )
				{
					color_ = ( color_ == Text::FontColor::WHITE ) ? Text::FontColor::LIGHT_GRAY : Text::FontColor::WHITE;
				}
			}
		break;
		case ( State::SHRINKING ):
			++timer_;

			y_ += MOVEMENT_PER_FRAME_Y;
			if ( y_ > InventoryLevel::Y )
			{
				y_ = InventoryLevel::Y;
			}

			x_ -= MOVEMENT_PER_FRAME_X;
			if ( x_ < InventoryLevel::CLOCK_X )
			{
				x_ = InventoryLevel::CLOCK_X;
			}

			if ( timer_ % SIZE_SPEED == 0 )
			{
				if ( size_ > 1 )
				{
					--size_;
				}
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
	Text::FontColor shadow_color = Text::FontColor::BLACK;
	if ( size_ <= HALF_SIZE )
	{
		shadow_color = Text::FontColor::__NULL;
		color_ = Text::FontColor::DARK_GRAY;
	}
	Inventory::clock().render( x_, y_, nullptr, color_, Text::FontAlign::LEFT, shadow_color, size_ );
};