#include "collision.hpp"
#include "bad_apple_sprite.hpp"
#include "sprite_graphics.hpp"

BadAppleSprite::BadAppleSprite( int x, int y, Direction::Horizontal dir_x )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/bad_apple.png" ), x, y, 16, 16, { SpriteType::ENEMY, SpriteType::BOPPABLE }, 100, 600, 1000, 1000, dir_x, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
	animation_timer_ ()
{};

BadAppleSprite::~BadAppleSprite() {};

void BadAppleSprite::customUpdate( LevelState& level_state )
{
	// Behavior
	moveInDirectionX();
	turnOnCollide();

	// Graphics
	flipGraphicsOnRight();
	if ( animation_timer_.update() )
	{
		graphics_->current_frame_x_ = ( graphics_->current_frame_x_ == 0 ) ? 16 : 0;
	}
};

void BadAppleSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{};
