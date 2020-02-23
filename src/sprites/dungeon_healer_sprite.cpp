#include "dungeon_healer_sprite.hpp"
#include "camera.hpp"
#include "collision.hpp"
#include "health.hpp"
#include "inventory.hpp"
#include "level_state.hpp"
#include "text_component_gradual.hpp"
#include <memory>

DungeonHealerSprite::DungeonHealerSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/nut-monk.png", 16, 16, false, false, 0.0, true ), x + Unit::BlocksToPixels( 12 ), y + Unit::BlocksToPixels( 5 ), 18, 16, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
    laser_gfx_ ( "sprites/nut-monk.png", 0, 32 ),
    heart_gfx_ ( "tilesets/universal.png", 16, 0 ),
    laser_box_ ( Unit::PixelsToSubPixels(  sdl2::SDLRect{ x + Unit::BlocksToPixels( 12 ), y + Unit::BlocksToPixels( 5 ), 16, 96 }) ),
    heart_box_ ( Unit::PixelsToSubPixels( x ) + Unit::BlocksToSubPixels( 12 ), Unit::PixelsToSubPixels( y ) + Unit::BlocksToSubPixels( 6 ), Unit::BlocksToSubPixels( 1 ), Unit::BlocksToSubPixels( 1 ) ),
    text_ ( "Pay 5,000 & you can heal.", Unit::BlocksToPixels( 3 ), Unit::BlocksToPixels( 3 ), Text::FontColor::WHITE, Text::FontAlign::CENTER, Text::FontColor::BLACK, false, 38, 0, 1, std::make_unique<TextComponentGradual> ( 2 ) ),
    timer_ ( -1 )
{};

DungeonHealerSprite::~DungeonHealerSprite() {};

void DungeonHealerSprite::customUpdate( LevelState& level_state )
{
    text_.update();
    if ( timer_ >= 0 && timer_ < 48 )
    {
        ++timer_;
    }
    graphics_->visible_ = !invincibilityFlickerOff();
};

void DungeonHealerSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
    if ( them.hasType( SpriteType::HERO ) )
    {
        if ( their_collision.collideAny() )
        {
            them.collideStopAny( their_collision );
        }
        const auto collision = them.testCollision( heart_box_ );
        Health& health = level_state.health();
        if ( Inventory::funds() >= 5000 && health.hp() < health.maxHP() && collision.collideAny() )
        {
            Inventory::loseFunds( 5000 );
            health.heal();
            heart_box_.x = heart_box_.y = Unit::BlocksToPixels( -1 );
        }
        if ( timer_ >= 47 )
        {
            const auto laser_collision = them.testCollision( laser_box_ );
            if ( laser_collision.collideAny() )
            {
                them.kill();
            }
        }
    }
    else if ( them.hasType( SpriteType::HEROS_BULLET ) )
    {
        if ( their_collision.collideAny() )
        {
            timer_ = 0;
        }
    }
};

void DungeonHealerSprite::render( Camera& camera, bool priority )
{
    graphics_->render( Unit::SubPixelsToPixels( hit_box_ ), &camera, priority );
    heart_gfx_.render( Unit::SubPixelsToPixels( heart_box_ ), &camera, priority );
    if ( camera.onscreen( hit_box_ ) )
    {
        text_.render();
    }
    if ( timer_ >= 47 )
    {
        laser_gfx_.render( Unit::SubPixelsToPixels( laser_box_ ), &camera, priority );
        Render::colorCanvas( 1 );
    }
};

bool DungeonHealerSprite::invincibilityFlickerOff() const
{
	return timer_ >= 0 && timer_ % 4 == 1;
};

void DungeonHealerSprite::reset()
{
    text_.chars_to_render_ = 0;
};