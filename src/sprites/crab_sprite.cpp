#include "crab_sprite.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"

CrabSprite::CrabSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/crab.png", 0, 0, false, false, 0, 0, 0, 0, 2 ), x, y, 16, 14, { SpriteType::ENEMY, SpriteType::CRAB }, 100, 800, 0, 0, Direction::Horizontal::LEFT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
	next_direction_ ( Direction::Horizontal::__NULL ),
	timer_ ( 0 ),
	animation_timer_ ( 0 )
{};

CrabSprite::~CrabSprite() {};

void CrabSprite::customUpdate( LevelState& level_state )
{
	moveInDirectionX();

	if ( timer_ >= 30 )
	{
		direction_x_ = next_direction_;
		next_direction_ = Direction::Horizontal::__NULL;
		timer_ = 0;
	}
	else if ( next_direction_ != Direction::Horizontal::__NULL )
	{
		++timer_;
	}

	if ( animation_timer_ >= 7 )
	{
		animation_timer_ = 0;
		graphics_->flip_x_ = !graphics_->flip_x_;
	}
	else
	{
		++animation_timer_;
	}
};

void CrabSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( them.hasType( SpriteType::HERO ) )
	{
		if ( them.hit_box_.x > rightSubPixels() )
		{
			next_direction_ = Direction::Horizontal::RIGHT;
		}
		else
		{
			next_direction_ = Direction::Horizontal::LEFT;
		}
	}
	else if ( them.hasType( SpriteType::CRAB ) )
	{
		if ( their_collision.collideLeft() || their_collision.collideRight() )
		{
			next_direction_ = Direction::switchHorizontal( direction_x_ );
			collideStopAny( my_collision );
		}
	}
};
