#include "collision.hpp"
#include "bad_apple_sprite.hpp"
#include "sprite_graphics.hpp"

BadAppleSprite::BadAppleSprite( int x, int y, Direction::Horizontal dir_x )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/bad_apple.png" ), x, y, 16, 16, { SpriteType::ENEMY, SpriteType::BOPPABLE }, 100, 600, 1000, 1000, dir_x, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
	animation_frame_ (),
	animation_timer_ ()
{};

BadAppleSprite::~BadAppleSprite() {};

void BadAppleSprite::customUpdate( LevelState& level_state )
{
	moveInDirectionX();
	turnOnCollide();
	updateGraphics();
};

void BadAppleSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{};

void BadAppleSprite::updateGraphics()
{
	flipGraphicsOnRight();
	if ( animation_timer_.hit() )
	{
		++animation_frame_;
		graphics_->current_frame_x_ = ( animation_frame_() == 0 ) ? 16 : 0;
	}
	animation_timer_.update();
}
