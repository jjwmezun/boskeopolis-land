#include "audio.hpp"
#include "bullet_sprite.hpp"
#include "chamsby_sprite.hpp"
#include "collision.hpp"
#include "health.hpp"
#include "sprite_graphics.hpp"
#include "sprite_system.hpp"

ChamsbySprite::ChamsbySprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/box.png" ), x, y, 16, 20, {}, 200, 2000, 500, 4000, Direction::Horizontal::LEFT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::PERMANENT ),
	health_ ( 3 ),
	invincibility_ ( 0 ),
	walk_timer_ (),
	shoot_timer_ ()
{};

ChamsbySprite::~ChamsbySprite() {};

void ChamsbySprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	moveInDirectionX();
	if ( !on_ground_ || is_jumping_ )
	{
		jump();
	}
	else if ( walk_timer_.update() )
	{
		jump();
	}

	graphics_->visible_ = ( invincibility_ % 4 != 1 );

	if ( invincibility_ > 0 )
	{
		--invincibility_;
	}
};

void ChamsbySprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( them.hasType( SpriteType::HERO ) )
	{
		if ( them.rightSubPixels() < hit_box_.x )
		{
			direction_x_ = Direction::Horizontal::LEFT;
		}
		else if ( them.hit_box_.x > rightSubPixels() )
		{
			direction_x_ = Direction::Horizontal::RIGHT;
		}
		if ( shoot_timer_.update() && them.hit_box_.y < bottomSubPixels() && them.bottomSubPixels() > hit_box_.y )
		{
			sprites.spawn( std::make_unique<BulletSprite> ( centerXPixels(), centerYPixels(), Direction::horizontalToSimple( direction_x_ ), false ) );
		}

		if ( invincibility_ == 0 )
		{
			if ( them.collideBottomOnly( their_collision, *this ) )
			{
				them.bounce();
				Audio::playSound( Audio::SoundType::BOP );
				--health_;
				invincibility_ = 48;
				if ( health_ == 0 )
				{
					kill();
				}
			}
			else if ( their_collision.collideAny() )
			{
				health.hurt();
			}
		}
	}
};
