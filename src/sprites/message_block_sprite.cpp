#include "audio.hpp"
#include "event_system.hpp"
#include "sprite_graphics.hpp"
#include "message_block_sprite.hpp"

MessageBlockSprite::MessageBlockSprite( int x, int y )
:
	is_being_bumped_ ( false ),
	Sprite( std::make_unique<SpriteGraphics> ( "tilesets/universal.png", 96, 0 ), x, y, 16, 16, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY )
{};

MessageBlockSprite::~MessageBlockSprite() {};

void MessageBlockSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	bump_under_block_component.update( *this );
	if ( is_being_bumped_ )
	{
		if ( isBackInPlace() )
		{
			events.showMessage();
			is_being_bumped_ = false;
		}
	}
};

void MessageBlockSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	const bool hit = bump_under_block_component.testHit( *this, them, their_collision );
	if ( hit )
	{
		Audio::playSound( Audio::SoundType::BUMP );
		is_being_bumped_ = true;
	}
};
