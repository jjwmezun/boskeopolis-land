#include "collision.h"
#include "snowboulder_sprite.h"
#include "sprite_graphics.h"

SnowboulderSprite::SnowboulderSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/snowboulder.png", 0, 0, false, false, 0, false, -2, -2, 4, 4 ), x, y, 28, 28, {}, 2000, 3000 )
{};

SnowboulderSprite::~SnowboulderSprite() {};

void SnowboulderSprite::customUpdate( const Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks )
{
	if ( vx_ < 0 )
	{
		graphics_->rotation_ -= ( vx_ / 500 );
	}
};

void SnowboulderSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap )
{
	if ( their_collision.collideBottom() )
	{
		them.collideStopYBottom( their_collision.overlapYBottom() );
	}
	else if ( their_collision.collideAny() )
	{
		them.collideStopAny( their_collision );
		them.hurt();
	}
};