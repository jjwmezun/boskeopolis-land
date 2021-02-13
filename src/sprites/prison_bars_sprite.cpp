#include "audio.hpp"
#include "collision.hpp"
#include "event_system.hpp"
#include "level_state.hpp"
#include "prison_bars_sprite.hpp"
#include "sprite_graphics.hpp"

PrisonBarsSprite::PrisonBarsSprite( int x, int y, int switch_number )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/jail-bars.png" ), x, y, 16, 112, { SpriteType::SOLID }, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
    switch_number_ ( switch_number - 1 )
{};

PrisonBarsSprite::~PrisonBarsSprite() {};

void PrisonBarsSprite::customUpdate( LevelState& level_state )
{
    // Test if “switch_number” bit is set on switch value.
    // Using bits allows us to use the single switch value as 8 or mo’ independent values.
    if ( level_state.events().getSwitchValue() & ( 1 << switch_number_ ) )
    {
        if ( hit_box_.h > 0 )
        {
            Audio::playSound( Audio::SoundType::CANCEL );
            hit_box_.h -= 1000;
            graphics_->current_frame_y_ = 112 - heightPixels();
        }
    }
    else
    {
        if ( hit_box_.h < Unit::BlocksToSubPixels( 6 ) )
        {
            Audio::playSound( Audio::SoundType::CANCEL );
            hit_box_.h += 1000;
            graphics_->current_frame_y_ = 112 - heightPixels();
        }
    }
};

void PrisonBarsSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	const Collision collision = them.testBlockCollision( *this );
	if ( !them.hasType( SpriteType::HEROS_BULLET ) && collision.collideAny() )
	{
		them.collideStopAny( collision );
	}
};
