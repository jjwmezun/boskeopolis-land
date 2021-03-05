#include "audio.hpp"
#include "collision.hpp"
#include "event_system.hpp"
#include "level_state.hpp"
#include "sprite_graphics.hpp"
#include "message_block_sprite.hpp"

MessageBlockSprite::MessageBlockSprite( int x, int y )
:
	is_being_bumped_ ( false ),
	Sprite( std::make_unique<SpriteGraphics> ( "tilesets/universal.png", 96, 0 ), x, y, 16, 16, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY )
{};

MessageBlockSprite::~MessageBlockSprite() {};

void MessageBlockSprite::customUpdate( LevelState& level_state )
{
	bump_under_block_component.update( *this );
	if ( is_being_bumped_ )
	{
		if ( isBackInPlace() )
		{
			level_state.events().showMessage();
			is_being_bumped_ = false;
		}
	}
};

void MessageBlockSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	const Collision collision = them.testBlockCollision( *this );
	const bool hit = bump_under_block_component.testHit( *this, them, collision );
	if ( hit )
	{
		Audio::playSound( Audio::SoundType::BUMP );
		is_being_bumped_ = true;
	}
};
