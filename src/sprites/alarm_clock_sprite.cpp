#include "alarm_clock_sprite.hpp"
#include "alarm_clock_note_sprite.hpp"
#include "collision.hpp"
#include "level_state.hpp"
#include "sprite_graphics.hpp"
#include "sprite_system.hpp"

static constexpr int FRAMES[ 4 ] = { 0, 16, 0, 32 };

AlarmClockSprite::AlarmClockSprite( int x, int y, Direction::Horizontal dir_x )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/clock.png", 0, 0, false, false, 0.0, -3, -2, 6, 2 ), x, y, 10, 14, { SpriteType::ENEMY, SpriteType::BOPPABLE }, 100, 600, 1000, 1000, dir_x, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
    timer_ ( 0 ),
    animation_timer_ (),
    animation_frame_ ()
{};

AlarmClockSprite::~AlarmClockSprite() {};

void AlarmClockSprite::customUpdate( LevelState& level_state )
{
	// Behavior
	moveInDirectionX();
	turnOnCollide();

    if ( timer_ == 15 )
    {
		level_state.sprites().spawn( std::make_unique<AlarmClockNoteSprite> ( centerXPixels(), centerYPixels(), direction_x_ ) );
        timer_ = 0;
    }
    else
    {
        ++timer_;
    }

	// Graphics
	flipGraphicsOnRight();
	if ( animation_timer_.update() )
	{
		graphics_->current_frame_x_ = FRAMES[ (++animation_frame_)() ];
	}
};

void AlarmClockSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{};
