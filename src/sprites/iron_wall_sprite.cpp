#include "audio.hpp"
#include "iron_wall_sprite.hpp"
#include "collision.hpp"
#include "event_system.hpp"
#include "sprite_graphics.hpp"

IronWallSprite::IronWallSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/iron-wall.png" ), x, y, 64, 300, {}, 500, 500, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT )
{};

IronWallSprite::~IronWallSprite() {};

void IronWallSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	if ( events.switch_ )
	{
		moveDown();
		Audio::playSound( Audio::SoundType::CANCEL );
		if ( fellInBottomlessPit( lvmap ) )
		{
			kill();
		}
	}
};

void IronWallSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( them.hasType( SpriteType::HERO ) && their_collision.collideAny() )
	{
		them.collideStopAny( their_collision );
	}
};
