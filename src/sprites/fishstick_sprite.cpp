#include "collision.hpp"
#include "fishstick_sprite.hpp"
#include "health.hpp"
#include "sprite_graphics.hpp"

FishstickSprite::FishstickSprite( int x, int y, Direction::Horizontal direction )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/fishstick.png", 0, 0, false, false, 0.0, -2, -3, 3, 4 ), x, y, 20, 12, { SpriteType::ENEMY, SpriteType::BOPPABLE }, 1400, 1400, 0, 0, direction, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY, true, false ),
	animation_timer_ (),
	animation_frame_ ( 0 )
{};

FishstickSprite::~FishstickSprite() {};

void FishstickSprite::customUpdate( LevelState& level_state )
{
	moveInDirectionX();

	flipGraphicsOnRight();
	if ( animation_timer_.update() )
	{
		graphics_->current_frame_x_ = ( graphics_->current_frame_x_ == 0 ) ? 23 : 0;
	}
};

void FishstickSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) {};
