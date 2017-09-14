#include "collision.hpp"
#include "health.hpp"
#include "sprite_graphics.hpp"
#include "underground_subway_sprite.hpp"

UndergroundSubwaySprite::UndergroundSubwaySprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/underground_subway.png" ), x, y + 8, 1088, 104, {}, 30000, 30000, 0, 0, Direction::Horizontal::LEFT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT, false, false ),
	activated_ ( false )
{
	hit_box_.x = Unit::BlocksToSubPixels( 150 );
};

UndergroundSubwaySprite::~UndergroundSubwaySprite() {};

void UndergroundSubwaySprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	if ( activated_ )
	{
		moveLeft();
	}

	if ( rightSubPixels() < 0 )
	{
		hit_box_.x = Unit::BlocksToSubPixels( 300 );
	}	
};

void UndergroundSubwaySprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health )
{
	if ( them.hasType( SpriteType::HERO ) )
	{
		if ( them.hit_box_.x > Unit::BlocksToSubPixels( 39 ) && them.hit_box_.y > Unit::BlocksToSubPixels( 48 ) )
		{
			activated_ = true;
		}
		
		if ( their_collision.collideAny() )
		{
			health.hurt();
		}
	}
};