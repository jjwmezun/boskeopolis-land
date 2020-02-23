#include "audio.hpp"
#include "iron_wall_sprite.hpp"
#include "collision.hpp"
#include "event_system.hpp"
#include "level_state.hpp"
#include "sprite_graphics.hpp"

IronWallSprite::IronWallSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/iron-wall.png" ), x, y, 64, 300, {}, 500, 500, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT )
{};

IronWallSprite::~IronWallSprite() {};

void IronWallSprite::customUpdate( LevelState& level_state )
{
	if ( level_state.events().isSwitchOn() )
	{
		moveDown();
		Audio::playSound( Audio::SoundType::CANCEL );
		if ( fellInBottomlessPit( level_state.currentMap() ) )
		{
			kill();
		}
	}
};

void IronWallSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( them.hasType( SpriteType::HERO ) && their_collision.collideAny() )
	{
		them.collideStopAny( their_collision );
	}
};
