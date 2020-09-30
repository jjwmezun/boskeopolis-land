#include "audio.hpp"
#include "mansion_door_sprite.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"

MansionDoorSprite::MansionDoorSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/mansion-door.png", 0, 0, false, false, 0.0, -3, 0, 16, 0 ), x, y, 5, 48, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
    animation_counter_ (),
    state_ ( MansionDoorState::CLOSED )
{};

MansionDoorSprite::~MansionDoorSprite() {};

void MansionDoorSprite::customUpdate( LevelState& level_state )
{
    switch ( state_ )
    {
        case ( MansionDoorState::OPENING ):
        {
            if ( animation_counter_.get() == 5 )
            {
                state_ = MansionDoorState::OPEN;
            }
        }
        // ¡FALLTHROUGH!
        case ( MansionDoorState::OPEN ):
        {
            animation_counter_.update();
            graphics_->current_frame_x_ = 21 * animation_counter_.get();
        }
        break;
    }
};

void MansionDoorSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
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
