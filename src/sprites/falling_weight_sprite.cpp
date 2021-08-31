#include "falling_weight_sprite.hpp"
#include "camera.hpp"
#include "collision.hpp"
#include "level_state.hpp"
#include "sprite_graphics.hpp"

FallingWeightSprite::FallingWeightSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/box.png" ), x, y, 32, 32, { SpriteType::ENEMY }, 6000, 6000, 0, 0, Direction::Horizontal::RIGHT, Direction::Vertical::DOWN, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT, false, false )
{};

FallingWeightSprite::~FallingWeightSprite() {};

void FallingWeightSprite::customUpdate( LevelState& level_state )
{
	switch ( direction_x_ )
	{
		case ( Direction::Horizontal::LEFT ):
			hit_box_.x = original_hit_box_.x;
		break;

		case ( Direction::Horizontal::RIGHT ):
			hit_box_.x = original_hit_box_.x + 32000; 
		break;
	}

	Camera& camera = level_state.camera();
	if ( camera.offscreenBottom( hit_box_ ) )
	{
		hit_box_.y = Unit::PixelsToSubPixels( camera.y() ) - ( hit_box_.h * 2 );
		direction_x_ = Direction::switchHorizontal( direction_x_ );
	}
	moveDown();
};

void FallingWeightSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{};
