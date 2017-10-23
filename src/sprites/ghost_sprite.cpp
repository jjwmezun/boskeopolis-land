#include "ghost_sprite.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"

static constexpr int VERTICAL_DIST_LIMIT_BLOCKS = 1;
static constexpr int VERTICAL_DIST_LIMIT_SUBPIXELS = Unit::BlocksToSubPixels( VERTICAL_DIST_LIMIT_BLOCKS );

GhostSprite::GhostSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/ghost.png", 0, 0, true ), x, y, 24, 24, { SpriteType::ENEMY }, 2500, 2500, 0, 0, Direction::Horizontal::RIGHT, Direction::Vertical::DOWN, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY, false, false )
{
	graphics_->priority_ = true;
};

GhostSprite::~GhostSprite() {};

void GhostSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	switch ( direction_x_ )
	{
		case ( Direction::Horizontal::RIGHT ):
			graphics_->flip_x_ = true;
			moveRight();
		break;
			
		default:
			graphics_->flip_x_ = false;
			moveLeft();
		break;
	}
	
	switch ( direction_y_ )
	{
		case ( Direction::Vertical::DOWN ):
			moveDown();
		break;

		case ( Direction::Vertical::UP ):
			moveUp();
		break;
	}
	
	if ( hit_box_.y > original_hit_box_.y + VERTICAL_DIST_LIMIT_SUBPIXELS + 24000 )
	{
		direction_y_ = Direction::Vertical::UP;
	}
	else if ( hit_box_.y < original_hit_box_.y - VERTICAL_DIST_LIMIT_SUBPIXELS )
	{
		direction_y_ = Direction::Vertical::DOWN;
	}
};

void GhostSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health )
{
};