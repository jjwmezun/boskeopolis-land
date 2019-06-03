#include "camera.hpp"
#include "hook_sprite.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"

HookSprite::HookSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/hook.png" ), x, y, 16, 16, { SpriteType::ENEMY }, 500, 2000, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::DOWN, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY )
{};

HookSprite::~HookSprite() {};

void HookSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	if ( direction_y_ == Direction::Vertical::UP )
	{
		moveUp();
		if ( hit_box_.y < original_hit_box_.y )
		{
			stopY();
			if ( vy_ >= 0 )
			{
				direction_y_ = Direction::Vertical::DOWN;
			}
		}
	}
	else
	{
		moveDown();
		if ( hit_box_.y > original_hit_box_.y + Unit::BlocksToSubPixels( 5 ) )
		{
			stopY();
			if ( vy_ <= 0 )
			{
				direction_y_ = Direction::Vertical::UP;
			}
		}
	}

};

void HookSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
};


void HookSprite::render( Camera& camera, bool priority )
{
	if ( !priority )
	{
		sdl2::SDLRect dest = camera.relativeRect( { xPixels(), yPixels() - 192, 16, 192 } );
		sdl2::SDLRect src = { 0, 16, 16, 192 };
		Render::renderObject( "sprites/hook.png", src, dest );
		graphics_->render( Unit::SubPixelsToPixels( hit_box_ ), &camera, priority );
	}
};
