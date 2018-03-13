#include "ice_box_rock_solid_hack_sprite.hpp"
#include "collision.hpp"
#include "render.hpp"
#include "sprite_graphics.hpp"

#include <iostream>
#include "main.hpp"

IceBoxRockSolidHackSprite::IceBoxRockSolidHackSprite( int x, int y )
:
	Sprite( nullptr, x, y, 12 * 16, 32, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT, false, false ),
	extra_solids_
	({
		Unit::PixelsToSubPixels( { x + 240, y - 32,  32, 128 } ),
		Unit::PixelsToSubPixels( { x + 80,  y + 96, 320,  32 } ),
		Unit::PixelsToSubPixels( { x, y + 32, 32, 160 } ),
		Unit::PixelsToSubPixels( { 0, 0, 0, 0 } ),
		Unit::PixelsToSubPixels( { 0, 0, 0, 0 } ),
		Unit::PixelsToSubPixels( { 0, 0, 0, 0 } ),
		Unit::PixelsToSubPixels( { 0, 0, 0, 0 } )
	})
{};

IceBoxRockSolidHackSprite::~IceBoxRockSolidHackSprite() {};

void IceBoxRockSolidHackSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{};

void IceBoxRockSolidHackSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( their_collision.collideAny() )
	{
		them.collideStopAny( their_collision );
	}
	for ( int i = 0; i < NUMBER_OF_EXTRA_SOLIDS; ++i )
	{
		const auto collision = them.movement_->testCollision( them, extra_solids_[ i ] );
		them.collideStopAny( collision );
		if ( them.hasType( SpriteType::ENEMY ) )
		{
			std::cout<<them.hit_box_.y<<std::endl;
		}
	}
};

void IceBoxRockSolidHackSprite::render( Camera& camera, bool priority )
{
	auto main_box = Unit::SubPixelsToPixels( hit_box_ );
	Render::renderRectCamera( main_box, camera, 6, 128 );

	for ( int i = 0; i < NUMBER_OF_EXTRA_SOLIDS; ++i )
	{
		auto i_box = Unit::SubPixelsToPixels( extra_solids_[ i ] );
		Render::renderRectCamera( i_box, camera, 6, 128 );
	}
};