#include "glass_door_sprite.hpp"
#include "collision.hpp"
#include "level_state.hpp"
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
        0,
        0,
        0,
        0,
        Direction::Horizontal::__NULL,
        Direction::Vertical::__NULL,
        nullptr,
        SpriteMovement::Type::FLOATING,
        CameraMovement::RESET_OFFSCREEN_AND_AWAY
    ),
    hits_ ( 0 )
{};

GlassDoorSprite::~GlassDoorSprite() {};

void GlassDoorSprite::customUpdate( LevelState& level_state )
{
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
