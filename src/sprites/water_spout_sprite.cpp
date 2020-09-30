#include "water_spout_sprite.hpp"
#include "collision.hpp"
#include "animated_graphics.hpp"

WaterSpoutSprite::WaterSpoutSprite( int x, int y )
:
	Sprite( std::make_unique<AnimatedGraphics> ( "sprites/water-spout.png", std::vector<std::pair<int, int>>{ { 0, 0 }, { 40, 0 } }, false, false, 0, -4, -1, 8, 1 ), x, y, 32, 0, { SpriteType::WATER_SPOUT }, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT ),
	delay_timer_ ( 0 ),
	grow_speed_ ( 0 ),
	state_ ( SpoutState::DORMANT ),
	barrel_ ( nullptr )
{};

WaterSpoutSprite::~WaterSpoutSprite() {};

void WaterSpoutSprite::customUpdate( LevelState& level_state )
{
	switch ( state_ )
	{
		case ( SpoutState::DORMANT ):
		{
			if ( delay_timer_ >= 48 )
			{
				state_ = SpoutState::RISE;
				delay_timer_ = 0;
			}
			else
			{
				++delay_timer_;
			}
		}
		break;

		case ( SpoutState::RISE ):
		{
			grow_speed_ += 250;
			if ( grow_speed_ > 2000 )
			{
				grow_speed_ = 2000;
			}
			hit_box_.h += grow_speed_;
			if ( hit_box_.h >= Unit::BlocksToSubPixels( 16 ) )
			{
				hit_box_.h = Unit::BlocksToSubPixels( 16 );
				state_ = SpoutState::TOP;
				grow_speed_ = 0;
			}
			adjustHitBox();
		}
		break;

		case ( SpoutState::TOP ):
		{
			if ( delay_timer_ >= 48 )
			{
				state_ = SpoutState::FALL;
				delay_timer_ = 0;
			}
			else
			{
				++delay_timer_;
			}
		}
		break;

		case ( SpoutState::FALL ):
		{
			grow_speed_ -= 250;
			hit_box_.h += grow_speed_;
			if ( hit_box_.h <= 0 )
			{
				hit_box_.h = 0;
				state_ = SpoutState::DORMANT;
				grow_speed_ = 0;
			}
			adjustHitBox();
		}
		break;
	}
};

void WaterSpoutSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( their_collision.collideAny() )
	{
		if ( their_collision.collideBottom() )
		{
			them.collideStopYBottom( their_collision.overlapYBottom() );
			if ( them.hasType( SpriteType::BARREL ) )
			{
				them.collide_bottom_ = true;
			}
		}
		else
		{
			them.collideStopAny( their_collision );
		}
	}
};
