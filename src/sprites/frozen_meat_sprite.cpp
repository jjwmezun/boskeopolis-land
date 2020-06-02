#include "frozen_meat_sprite.hpp"
#include "collision.hpp"
#include "level_state.hpp"
#include "sprite_graphics.hpp"

FrozenMeatSprite::FrozenMeatSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/frozen-meat.png" ), x, y, 32, 32, {}, 500, 2000, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::RESET_INSTANTLY_OFFSCREEN
	, false ),
	top_hit_box_ ( hit_box_.x, hit_box_.y - Unit::PixelsToSubPixels( 32 ), Unit::PixelsToSubPixels( 32 ), Unit::PixelsToSubPixels( 32 ) ),
	image_box_ ( x, y, 32, 64 )
{};

FrozenMeatSprite::~FrozenMeatSprite() {};

void FrozenMeatSprite::customUpdate( LevelState& level_state )
{
	updateOtherHitBoxes();
};

void FrozenMeatSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	const Collision top_collision = movement_->testCollision( them, top_hit_box_ );
	if ( their_collision.collideAny() )
	{
		them.collideStopAny( their_collision );
		
		if ( them.hasType( SpriteType::HERO ) && their_collision.collideTop() && them.collide_top_ )
		{
			them.kill();
		}
	}
	if ( top_collision.collideAny() )
	{
		if ( top_collision.collideLeft() )
		{
			them.collideStopXLeft( top_collision.overlapXLeft() );
		}

		if ( top_collision.collideRight() )
		{
			them.collideStopXRight( top_collision.overlapXRight() );
		}

		if ( top_collision.collideBottom() )
		{
			them.hit_box_.x += hit_box_.x - x_prev_;
			them.collideStopYBottom( top_collision.overlapYBottom() );
		}
	}
};

void FrozenMeatSprite::render( Camera& camera, bool priority )
{
	graphics_->render( image_box_, &camera, priority );
};

void FrozenMeatSprite::updateOtherHitBoxes()
{
	top_hit_box_.x = hit_box_.x;
	top_hit_box_.y = hit_box_.y - Unit::PixelsToSubPixels( 32 );
	image_box_.x = Unit::SubPixelsToPixels( hit_box_.x );
	image_box_.y = Unit::SubPixelsToPixels( hit_box_.y ) - 32;
};