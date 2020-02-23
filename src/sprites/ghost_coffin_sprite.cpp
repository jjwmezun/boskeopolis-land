#include "audio.hpp"
#include "event_system.hpp"
#include "ghost_coffin_sprite.hpp"
#include "level_state.hpp"
#include "mansion_ghost_sprite.hpp"
#include "sprite_graphics.hpp"
#include "sprite_system.hpp"

GhostCoffinSprite::GhostCoffinSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "tilesets/woods.png", 12 * 16, 0 ), x, y, 16, 32, {}, 25, 250, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
    done_ ( false )
{};

GhostCoffinSprite::~GhostCoffinSprite() {};

void GhostCoffinSprite::customUpdate( LevelState& level_state )
{
    if ( !done_ && level_state.events().isSwitchOn() )
    {
        if ( hit_box_.x == original_hit_box_.x )
        {
            Audio::playSound( Audio::SoundType::CHEST_LOCK );
        }
        if ( hit_box_.x >= original_hit_box_.x + 16000 )
        {
            hit_box_.x = original_hit_box_.x + 16000;
            fullStopX();
            level_state.sprites().spawn( std::make_unique<MansionGhostSprite> ( xPixels() - 16, yPixels() ) );
            done_ = true;
        }
        else
        {
            moveRight();
        }
    }
};

void GhostCoffinSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{};

void GhostCoffinSprite::reset() {};
