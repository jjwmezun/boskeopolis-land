#include "collision.hpp"
#include "hieroglyph_pusher_sprite.hpp"
#include "level_state.hpp"
#include "sprite_graphics.hpp"

HieroglyphPusherSprite::HieroglyphPusherSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/hieroglyph_pusher.png" ), x, y, 16, 16, {}, 100, 1500, 0, 0, Direction::Horizontal::LEFT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::RESET_OFFSCREEN_AND_AWAY )
{};

HieroglyphPusherSprite::~HieroglyphPusherSprite() {};

void HieroglyphPusherSprite::customUpdate( LevelState& level_state )
{
	turnOnEdge( level_state.blocks() );
	turnOnCollide();
	moveInDirectionX();

	switch ( direction_x_ )
	{
		case ( Direction::Horizontal::LEFT ):
			graphics_->flip_x_ = true;
		break;
		case ( Direction::Horizontal::RIGHT ):
			graphics_->flip_x_ = false;
		break;
	}
};

void HieroglyphPusherSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	const Collision collision = them.testBlockCollision( *this );
	if ( collision.collideAny() )
	{
		them.collideStopAny( collision );
		them.hit_box_.x += vx_;
		
		if
		(
			( them.collide_left_ && collision.collideRight() ) ||
			( them.collide_right_ && collision.collideLeft() )
		)
		{
			them.kill();
		}
	}
};