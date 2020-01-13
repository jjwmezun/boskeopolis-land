#include "camera.hpp"
#include "input_component_sequence.hpp"
#include "racer_sprite.hpp"
#include "sprite_graphics.hpp"

RacerSprite::RacerSprite( int x, int y )
:
	PlayerSprite
	(
		x,
		y,
		800,
		4000,
		std::unique_ptr<InputComponent> ( new InputComponentSequence( "racer" ) ),
		std::make_unique<SpriteGraphics> ( "sprites/racer.png", 0, 0, false, false, 0, false, -1, -2, 2, 4 ),
		SpriteType::RIVAL,
		2100,
		2100
	)
{
};

RacerSprite::~RacerSprite() {};

void RacerSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	actions( blocks, events );
	if ( !on_ground_ )
	{
		vx_ *= .2;
	}
	player_gfx_.update( *this, graphics_.get() );
};

void RacerSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
};
