#include <algorithm>
#include "audio.hpp"
#include "camera.hpp"
#include "collision.hpp"
#include "cloud_monster_sprite.hpp"
#include "sprite_graphics.hpp"

CloudMonsterSprite::CloudMonsterSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/cloud-monster.png", 0, 0, false, false, 0, true, 0, -8, 0, 8 ), x, y, 80, 40, { SpriteType::CLOUD_MONSTER }, 60, 4200, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT, false, false ),
	lightning_ ( lightningPosition(), y + 28 ),
	lightning_is_on_ ( false ),
	lightning_timer_ ( 0 )
{};

CloudMonsterSprite::~CloudMonsterSprite() {};

void CloudMonsterSprite::render( Camera& camera, bool priority )
{
	graphics_->render( Unit::SubPixelsToPixels( hit_box_ ), &camera, priority );
	lightning_.render( camera, priority );
};

void CloudMonsterSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	lightning_.changeX( lightningPosition() );
	lightningUpdate();
};

void CloudMonsterSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	lightning_.changeX( lightningPosition() );

	if ( them.hasType( SpriteType::HERO ) )
	{		
		if ( them.collideBottomOnly( their_collision, *this ) )
		{
			them.bounce();
			Audio::playSound( Audio::SoundType::BOUNCE );
		}

		if ( them.xPixels() > rightPixels() )
		{
			moveRight();
		}
		else if ( them.rightPixels() < xPixels() )
		{
			moveLeft();
		}
		
		if ( lightning_is_on_ )
		{
			lightning_.interact( them, blocks, sprites, lvmap, health, events );
			graphics_->current_frame_x_ = 80;
		}
		else
		{
			graphics_->current_frame_x_ = 0;
		}
	}
};

int CloudMonsterSprite::lightningPosition() const
{
	return centerXSubPixels() - 3000;
}

void CloudMonsterSprite::lightningUpdate()
{
	lightning_.graphics_->visible_ = lightning_is_on_;
	++lightning_timer_;

	if ( lightning_timer_ >= 32 )
	{
		lightning_is_on_ = !lightning_is_on_;
		lightning_timer_ = 0;
		if ( lightning_is_on_ )
		{
			Audio::playSound( Audio::SoundType::LIGHTNING );
		}
	}

	if ( lightning_timer_ % 4 == 1 )
	{
		lightning_.graphics_->current_frame_x_ = ( lightning_.graphics_->current_frame_x_ == 0 ) ? 4 : 0;
	}
};