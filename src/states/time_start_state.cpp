#include <iostream>
#include "clock.hpp"
#include "inventory.hpp"
#include "inventory_level.hpp"
#include "main.hpp"
#include "time_start_state.hpp"

static constexpr int START_SIZE = 12;
static constexpr int TIMER_HEIGHT = START_SIZE * Unit::PIXELS_PER_MINIBLOCK;
static constexpr int START_Y = -TIMER_HEIGHT;
static constexpr int MOVE_SPEED = 2;
static constexpr int STOPPING_POINT = ( ( Unit::WINDOW_HEIGHT_PIXELS - InventoryLevel::HEIGHT ) / 2 ) - ( TIMER_HEIGHT / 2 );
static constexpr int BUMP_LENGTH = 4;
static constexpr int SIZE_SPEED = 8;
static constexpr int MOVEMENT_FRAMES = SIZE_SPEED * START_SIZE;
static constexpr int MOVEMENT_PER_FRAME = ( int )( ceil( ( ( double )( InventoryLevel::TOP_ROW_Y ) - ( double )( STOPPING_POINT ) ) / ( double )( MOVEMENT_FRAMES ) ) );

TimeStartState::TimeStartState( const Palette& palette )
:
	GameState( StateID::TIME_START_STATE, palette ),
	y_ ( START_Y ),
	size_ ( START_SIZE ),
	size_timer_ ( 0 ),
	state_ ( State::GOING_DOWN )
{};

TimeStartState::~TimeStartState() {};

void TimeStartState::update()
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
				state_ = State::DONE;
				y_ = STOPPING_POINT;
			}
			else
			{
				y_ += MOVE_SPEED;
			}
		break;
		case ( State::DONE ):
			++size_timer_;
			y_ += MOVEMENT_PER_FRAME;
			if ( size_timer_ >= SIZE_SPEED )
			{
				size_timer_ = 0;
				if ( size_ > 1 )
				{
					--size_;
				}
			}
		break;
	}
	//Main::popState();
	std::cout<<MOVEMENT_PER_FRAME<<std::endl;
};

void TimeStartState::stateRender()
{
	Inventory::clock().render( 0, y_, nullptr, Text::FontShade::WHITE, Text::FontAlign::CENTER, size_ );
};

void TimeStartState::init()
{
};