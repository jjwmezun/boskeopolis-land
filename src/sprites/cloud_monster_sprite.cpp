#include <algorithm>
#include "camera.hpp"
#include "collision.hpp"
#include "cloud_monster_sprite.hpp"
#include "sprite_graphics.hpp"

CloudMonsterSprite::CloudMonsterSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/cloud_monster.png", 0, 0, false, false, 0, true ), x, y, 80, 48, {}, 60, 4200, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT ),
	lightning_ ( lightningPosition(), y + 31 )
{
};

CloudMonsterSprite::~CloudMonsterSprite() {};

void CloudMonsterSprite::render( Camera& camera, bool priority )
{
	if ( camera.onscreen( hitBox() ) && graphics_ != nullptr )
	{
		graphics_->render( Unit::SubPixelsToPixels( hit_box_ ), &camera, priority );
		lightning_.render( camera, priority );
	}
};

void CloudMonsterSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	lightning_.changeX( lightningPosition() );
	lightning_.update( camera, lvmap, events, sprites, blocks, health );
};

void CloudMonsterSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	lightning_.changeX( lightningPosition() );

	if ( them.hasType( SpriteType::HERO ) )
	{		
		if ( them.collideBottomOnly( their_collision, *this ) )
		{
			them.bounce();
			//vx_ = std::min( vx_, 3000 );
			//vx_ = std::max( vx_, -3000 );
		}
		
		if ( them.xPixels() > rightPixels() )
		{
			moveRight();
		}
		else if ( them.rightPixels() < xPixels() )
		{
			moveLeft();
		}
		
		lightning_.interact( them, blocks, sprites, lvmap, health, events );
	}
};

int CloudMonsterSprite::lightningPosition() const
{
	return centerXSubPixels() - 2;
}