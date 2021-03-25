#include "helicopter_friedman_sprite.hpp"
#include "bomb_sprite.hpp"
#include "camera.hpp"
#include "level_state.hpp"
#include "sprite_graphics.hpp"
#include "sprite_system.hpp"

HelicopterFriedmanSprite::HelicopterFriedmanSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/box.png" ), x, y, 48, 48, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY, false, false ),
	timer_ ( 0 )
{};

HelicopterFriedmanSprite::~HelicopterFriedmanSprite() {};

void HelicopterFriedmanSprite::customUpdate( LevelState& level_state )
{
	const Camera& camera = level_state.camera();
	if ( camera.onscreen( hit_box_ ) )
	{
		if ( camera.relativeY( yPixels() ) > 16 )
		{
			hit_box_.y = Unit::PixelsToSubPixels( camera.y() + 16 );
		}
		
		if ( timer_ >= 32 )
		{
			level_state.sprites().spawn( std::make_unique<BombSprite> ( xPixels(), yPixels() ) );
			timer_ = 0;
		}
		else
		{
			++timer_;
		}
	}
};

void HelicopterFriedmanSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
};
