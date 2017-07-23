#include "collision.h"
#include "lightning_sprite.h"
#include "sprite_graphics.h"

LightningSprite::LightningSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/lightning.png", 0, 0, false, false, 0, true ), x, y, 4, 160, { SpriteType::LIGHTNING }, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT ),
	on_ ( false ),
	counter_ ( 0 )
{
};

LightningSprite::~LightningSprite() {};

void LightningSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks )
{
	graphics_->visible_ = on_;
	++counter_;
	
	if ( counter_ >= 32 )
	{
		on_ = !on_;
		counter_ = 0;
	}
};

void LightningSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap )
{
	if ( them.hasType( SpriteType::HERO ) && on_ )
	{
		if ( my_collision.collideAny() )
		{
			them.hurt();
		}
	}
};