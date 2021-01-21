#include "glass_button_sprite.hpp"
#include "collision.hpp"
#include "event_system.hpp"
#include "level_state.hpp"
#include "sprite_graphics.hpp"

static constexpr int MAX_HITS = 5;

constexpr static int xFromDirection( Direction::Simple direction, int x )
{
    switch ( direction )
    {
        case ( Direction::Simple::DOWN ): return x;
        case ( Direction::Simple::LEFT ): return x + 10;
        case ( Direction::Simple::RIGHT ): return x;
        case ( Direction::Simple::UP ): return x;
    }
    return x;
};

GlassButtonSprite::GlassButtonSprite( int x, int y, int switch_number, Direction::Simple direction )
:
	Sprite
    (
        std::make_unique<SpriteGraphics>
        (
            "sprites/glass-button.png",
            ( Direction::SimpleIsHorizontal( direction ) ) ? 0 : 36,
            0,
            direction == Direction::Simple::RIGHT,
            direction == Direction::Simple::UP
        ),
        ( direction == Direction::Simple::LEFT ) ? x + 10 : x,
        ( direction == Direction::Simple::UP ) ? y + 10 : y,
        ( Direction::SimpleIsHorizontal( direction ) ) ? 6 : 16,
        ( Direction::SimpleIsHorizontal( direction ) ) ? 16 : 6,
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
    hits_ ( 0 ),
    switch_number_ ( switch_number )
{
    direction_ = direction;
};

GlassButtonSprite::~GlassButtonSprite() {};

void GlassButtonSprite::customUpdate( LevelState& level_state )
{
};

void GlassButtonSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
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
            if ( hits_ == MAX_HITS )
            {
                // Set “switch_number_” bit on switch value to 1.
                // Using bits allows us to use the single switch value as 8 or mo’ independent values.
                level_state.events().forceSwitch( level_state.events().getSwitchValue() | ( int )( std::pow( 2, switch_number_ - 1 ) ) );
            }
            if ( Direction::SimpleIsHorizontal( direction_ ) )
            {
                graphics_->current_frame_x_ = 6 * hits_;
            }
            else
            {
                graphics_->current_frame_y_ = 6 * hits_;
            }
        }
    }
};
