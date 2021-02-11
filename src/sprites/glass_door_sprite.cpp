#include "glass_door_sprite.hpp"
#include "collision.hpp"
#include "level_state.hpp"
#include "render.hpp"
#include "sprite_graphics.hpp"

static constexpr int MAX_HITS = 5;

GlassDoorSprite::GlassDoorSprite( int x, int y )
:
	Sprite
    (
        std::make_unique<SpriteGraphics>
        (
            "sprites/glass-door.png"
        ),
        x + 4,
        y,
        8,
        16*7,
        {},
        50,
        4000,
        0,
        0,
        Direction::Horizontal::__NULL,
        Direction::Vertical::__NULL,
        nullptr,
        SpriteMovement::Type::FLOATING,
        CameraMovement::RESET_OFFSCREEN_AND_AWAY,
        true,
        false
    ),
    hits_ ( 0 )/*,
    (
    shard_dest_
        Unit::PixelsToSubPixels( x, y, 6, 7 ),
        Unit::PixelsToSubPixels( x + 2, y + 9, 6, 11 ),
        Unit::PixelsToSubPixels( x, y + 21, 6, 5 ),
        Unit::PixelsToSubPixels( x + 2, y + 29, 6, 5 ),
        Unit::PixelsToSubPixels( x, y + 33, 6, 7 ),
        Unit::PixelsToSubPixels( x + 2, y + 43, 6, 8 ),
        Unit::PixelsToSubPixels( x + 3, y + 60, 5, 8 ),
        Unit::PixelsToSubPixels( x, y + 70, 4, 4 ),
        Unit::PixelsToSubPixels( x, y + 77, 4, 6 )
    ),
    shard_src_
    (

    )*/
{};

GlassDoorSprite::~GlassDoorSprite() {};

void GlassDoorSprite::customUpdate( LevelState& level_state )
{
    if ( hits_ == MAX_HITS )
    {
        moveDown();
        hit_box_.h = original_hit_box_.h - ( hit_box_.y - original_hit_box_.y );
    }
};

void GlassDoorSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
    if ( hits_ < MAX_HITS )
    {
        if ( them.hasType( SpriteType::HERO ) )
        {
            const Collision collision = them.testBlockCollision( *this );
            if ( collision.collideAny() )
            {
                them.collideStopAny( collision );
            }
        }
        else if ( them.hasType( SpriteType::HEROS_BULLET ) && !them.is_dead_ && their_collision.collideAny() )
        {
            ++hits_;
            them.kill();
            graphics_->current_frame_x_ = 8 * hits_;
        }
    }
};