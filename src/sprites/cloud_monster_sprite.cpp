#include <algorithm>
#include "camera.h"
#include "collision.h"
#include "cloud_monster_sprite.h"
#include "sprite_graphics.h"

CloudMonsterSprite::CloudMonsterSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/cloud_monster.png", 0, 0, false, false, 0, true ), x, y, 80, 48, {}, 60, 4200, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT ),
	lightning_ ( lightningPosition(), y + 31 )
{
};

CloudMonsterSprite::~CloudMonsterSprite() {};

void CloudMonsterSprite::render( Graphics& graphics, Camera& camera, bool priority )
{
	if ( camera.onscreen( hitBox() ) && graphics_ != nullptr )
	{
		graphics_->render( graphics, Unit::SubPixelsToPixels( hit_box_ ), &camera, priority );
		lightning_.render( graphics, camera, priority );
	}
};

void CloudMonsterSprite::customUpdate( const Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks )
{
	lightning_.changeX( lightningPosition() );
	lightning_.update( input, camera, lvmap, game, events, sprites, blocks );
};

void CloudMonsterSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap )
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
		
		lightning_.interact( them, blocks, sprites, lvmap );
	}
};

int CloudMonsterSprite::lightningPosition() const
{
	return centerXSubPixels() - 2;
}