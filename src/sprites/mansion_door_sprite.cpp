#include "audio.hpp"
#include "mansion_door_sprite.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"

static constexpr int NUMBER_OF_FRAMES = 8;
static constexpr int TICKS_PER_FRAME = 8;

MansionDoorSprite::MansionDoorSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/mansion-door.png", 0, 0, false, false, 0.0, false, -3, 0, 16, 0 ), x, y, 5, 48, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
    state_ ( MansionDoorState::CLOSED ),
    animation_timer_ ( 0 ),
    current_animation_frame_ ( 0 )
{};

MansionDoorSprite::~MansionDoorSprite() {};

void MansionDoorSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
    switch ( state_ )
    {
        case ( MansionDoorState::OPENING ):
        {
            if ( current_animation_frame_ == 5 )
            {
                state_ = MansionDoorState::OPEN;
            }
        }
        // ¡FALLTHROUGH!
        case ( MansionDoorState::OPEN ):
        {
            if ( current_animation_frame_ < NUMBER_OF_FRAMES - 1 )
            {
                if ( animation_timer_ >= TICKS_PER_FRAME - 1 )
                {
                    ++current_animation_frame_;
                    graphics_->current_frame_x_ = 21 * current_animation_frame_;
                    animation_timer_ = 0;
                }
                else
                {
                    ++animation_timer_;
                }
            }
        }
        break;
    }
};

void MansionDoorSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( them.hasType( SpriteType::HERO ) )
    {
        switch ( state_ )
        {
            case ( MansionDoorState::CLOSED ):
            {
                if ( their_collision.collideRight() )
                {
                    state_ = MansionDoorState::OPENING;
                    Audio::playSound( Audio::SoundType::CHEST_OPEN );
                }
            }
            // ¡FALLTHROUGH!
            case ( MansionDoorState::OPENING ):
            {
                if ( their_collision.collideAny() )
                {
                    them.collideStopAny( their_collision );
                }
            }
            break;
        }


    }
};
