#include "collision.hpp"
#include "event_system.hpp"
#include "level_state.hpp"
#include "rope_sprite.hpp"
#include "sprite_graphics.hpp"

RopeSprite::RopeSprite( int x, int y, int height, int distance, int speed )
:
    Sprite( std::make_unique<SpriteGraphics> ( "sprites/rope-sprite.png", 0, 384 - Unit::BlocksToPixels( height ) ), x, y, 8, Unit::BlocksToPixels( height ), {}, speed, speed, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY, false, false, true, true ),
    on_ ( false ),
    action_ ( distance, Direction::Simple::RIGHT )
{};

RopeSprite::~RopeSprite() {};

void RopeSprite::customUpdate( LevelState& level_state )
{
    if ( on_ )
    {
        action_.update( *this, level_state );
    }
    else
    {
		// Make it not affected by wind.
		hit_box_.x = original_hit_box_.x;
    }
};

void RopeSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
    if ( them.hasType( SpriteType::HERO ) )
    {
        if ( their_collision.collideAny() )
        {
            level_state.events().setTouchingLadder();
            if ( them.on_ladder_ )
            {
            	on_ = true;
                them.addToX( vx_ );
            }
        }
    }
};

void RopeSprite::reset()
{
    hit_box_.x = original_hit_box_.x;
    on_ = false;
    vx_ = 0;
    acceleration_x_ = 0;
    action_.reset();
};
