#include "dungeon_healer_sprite.hpp"
#include "camera.hpp"
#include "collision.hpp"
#include "health.hpp"
#include "inventory.hpp"
#include "text_component_gradual.hpp"
#include <memory>

DungeonHealerSprite::DungeonHealerSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/nut-monk.png", 16, 16 ), x + Unit::BlocksToPixels( 12 ), y + Unit::BlocksToPixels( 5 ), 18, 16, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
    heart_gfx_ ( "tilesets/universal.png", 16, 0 ),
    heart_box_ ( Unit::PixelsToSubPixels( x ) + Unit::BlocksToSubPixels( 12 ), Unit::PixelsToSubPixels( y ) + Unit::BlocksToSubPixels( 6 ), Unit::BlocksToSubPixels( 1 ), Unit::BlocksToSubPixels( 1 ) ),
    text_ ( "Pay 5,000 & you can heal.", Unit::BlocksToPixels( 3 ), Unit::BlocksToPixels( 3 ), Text::FontColor::WHITE, Text::FontAlign::CENTER, Text::FontColor::BLACK, false, 38, 0, 1, std::make_unique<TextComponentGradual> ( 2 ) )
{};

DungeonHealerSprite::~DungeonHealerSprite() {};

void DungeonHealerSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
    text_.update();
};

void DungeonHealerSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
    if ( them.hasType( SpriteType::HERO ) )
    {
        if ( their_collision.collideAny() )
        {
            them.collideStopAny( their_collision );
        }
        const auto collision = them.testCollision( heart_box_ );
        if ( Inventory::funds() >= 5000 && health.hp() < health.maxHP() && collision.collideAny() )
        {
            Inventory::loseFunds( 5000 );
            health.heal();
            heart_box_.x = heart_box_.y = Unit::BlocksToPixels( -1 );
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
};
