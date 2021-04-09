#include "alarm_clock_sprite.hpp"
#include "alarm_clock_note_sprite.hpp"
#include "collision.hpp"
#include "level_state.hpp"
#include "sprite_graphics.hpp"
#include "sprite_system.hpp"

AlarmClockSprite::AlarmClockSprite( int x, int y, Direction::Horizontal dir_x )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/bad_apple.png" ), x, y, 16, 16, { SpriteType::ENEMY, SpriteType::BOPPABLE }, 100, 600, 1000, 1000, dir_x, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
    timer_ ( 0 )
{};

AlarmClockSprite::~AlarmClockSprite() {};

void AlarmClockSprite::customUpdate( LevelState& level_state )
{
	// Behavior
	moveInDirectionX();
	turnOnCollide();

	// Graphics
	flipGraphicsOnRight();

    if ( timer_ == 15 )
    {
		level_state.sprites().spawn( std::make_unique<AlarmClockNoteSprite> ( centerXPixels(), centerYPixels(), direction_x_ ) );
        timer_ = 0;
    }
    else
    {
        ++timer_;
    }
};

void AlarmClockSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{};
