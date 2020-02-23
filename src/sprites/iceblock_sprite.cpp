#include "collision.hpp"
#include "iceblock_sprite.hpp"
#include "sprite_graphics.hpp"

IceBlockSprite::IceBlockSprite( int x, int y )
:
    Sprite( std::make_unique<SpriteGraphics> ( "sprites/iceblock.png" ), x, y, 16, 16, {}, 400, 1200, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_INSTANTLY_OFFSCREEN, false, false, true, true ),
    crack_state_ ( CrackState::NORMAL ),
    delay_ (),
    grow_delay_ ()
{};

IceBlockSprite::~IceBlockSprite() {};

void IceBlockSprite::customUpdate( LevelState& level_state )
{
    switch ( crack_state_ )
    {
        case ( CrackState::CRACKING_GROWING ):
        {
            if ( delay_.done() )
            {
                crack_state_ = CrackState::NORMAL;
                delay_.stop();
            }
            else if ( delay_.on() )
            {
                delay_.update();
            }
            else
            {
                delay_.start();
                graphics_->current_frame_x_ = 16;
                graphics_->visible_ = true;
                sprite_interact_ = true;
            }
        }
        break;

        case ( CrackState::GONE_GROWING ):
        {
            if ( grow_delay_.done() )
            {
                crack_state_ = CrackState::CRACKING_GROWING;
                grow_delay_.stop();
                delay_.stop();
            }
            else if ( grow_delay_.on() )
            {
                grow_delay_.update();
            }
            else
            {
                grow_delay_.start();
            }
        }
        break;

        case ( CrackState::SHATTERED ):
        {
            changeMovement( SpriteMovement::Type::GROUNDED );
            sprite_interact_ = false;
            graphics_->current_frame_x_ = 32;
        }
        break;

        case ( CrackState::CRACKING_HEAD ):
        {
            if ( delay_.done() )
            {
                // Do Nothing.
            }
            else if ( delay_.on() )
            {
                delay_.update();
            }
            else
            {
                delay_.start();
                graphics_->current_frame_x_ = 16;
            }
        }
        break;

        case ( CrackState::CRACKING_FEET ):
        {
            if ( delay_.done() )
            {
                crack_state_ = CrackState::SHATTERED;
                delay_.stop();
            }
            else if ( delay_.on() )
            {
                delay_.update();
            }
            else
            {
                delay_.start();
                graphics_->current_frame_x_ = 16;
            }
        }
        break;

        case ( CrackState::NORMAL ):
        {
            graphics_->current_frame_x_ = 0;
        }
        break;
    }
};

void IceBlockSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
    if ( their_collision.collideAny() )
    {
        if ( crack_state_ != CrackState::SHATTERED && crack_state_ != CrackState::GONE_GROWING )
        {
            them.collideStopAny( their_collision );
        }
    }

    if ( them.hasType( SpriteType::HERO ) )
    {
        if ( their_collision.collideBottom() )
        {
            if ( crack_state_ == CrackState::NORMAL )
            {
                crack_state_ = CrackState::CRACKING_FEET;
            }
            else if ( crack_state_ == CrackState::CRACKING_HEAD || crack_state_ == CrackState::CRACKING_GROWING )
            {
                crack_state_ = CrackState::SHATTERED;
            }
        }

        if ( their_collision.collideTop() )
        {
            if ( crack_state_ == CrackState::NORMAL )
            {
                crack_state_ = CrackState::CRACKING_HEAD;
                delay_.stop();
            }
            else if ( crack_state_ == CrackState::CRACKING_HEAD && delay_.done() )
            {
                crack_state_ = CrackState::CRACKING_FEET;
            }
        }
    }
};

void IceBlockSprite::reset()
{
    hit_box_.y = original_hit_box_.y;
    fullStopY();
    delay_.stop();
    grow_delay_.stop();
    graphics_->visible_ = false;
    changeMovement( SpriteMovement::Type::FLOATING );
    crack_state_ = CrackState::GONE_GROWING;
};
