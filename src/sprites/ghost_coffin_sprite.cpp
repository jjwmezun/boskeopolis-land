#include "audio.hpp"
#include "event_system.hpp"
#include "ghost_coffin_sprite.hpp"
#include "mansion_ghost_sprite.hpp"
#include "sprite_graphics.hpp"
#include "sprite_system.hpp"

GhostCoffinSprite::GhostCoffinSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "tilesets/woods.png", 12 * 16, 0 ), x, y, 16, 32, {}, 25, 250, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
    done_ ( false )
{};

GhostCoffinSprite::~GhostCoffinSprite() {};

void GhostCoffinSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
    if ( !done_ && events.isSwitchOn() )
    {
        if ( hit_box_.x == original_hit_box_.x )
        {
            Audio::playSound( Audio::SoundType::CHEST_LOCK );
        }
        if ( hit_box_.x >= original_hit_box_.x + 16000 )
        {
            hit_box_.x = original_hit_box_.x + 16000;
            fullStopX();
            sprites.spawn( std::make_unique<MansionGhostSprite> ( xPixels() - 16, yPixels() ) );
            done_ = true;
        }
        else
        {
            moveRight();
        }
    }
};

void GhostCoffinSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{};

void GhostCoffinSprite::reset() {};
