#include "collision.hpp"
#include "health.hpp"
#include "level_state.hpp"
#include "snowboulder_sprite.hpp"
#include "sprite_graphics.hpp"

SnowboulderSprite::SnowboulderSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/snowboulder.png", 0, 0, false, false, 0, false, -2, -2, 4, 4 ), x, y, 28, 28, {}, 2000, 3000 )
{};

SnowboulderSprite::~SnowboulderSprite() {};

void SnowboulderSprite::customUpdate( LevelState& level_state )
{
	if ( vx_ < 0 )
	{
		graphics_->rotation_ -= ( vx_ / 500 );
	}
};

void SnowboulderSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( their_collision.collideBottom() )
	{
		them.collideStopYBottom( their_collision.overlapYBottom() );
	}
	else if ( their_collision.collideAny() )
	{
		them.collideStopAny( their_collision );
		level_state.health().hurt();
	}
};