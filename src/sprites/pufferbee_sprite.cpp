#include "pufferbee_sprite.hpp"
#include "pufferbee_graphics.hpp"

PufferbeeSprite::PufferbeeSprite
(
	int x,
	int y,
	std::unique_ptr<SpriteComponent> component,
    Unit::Layer layer
)
:
    Sprite
	(
		std::make_unique<PufferbeeGraphics> (),
		x,
		y,
		20,
		20,
		{ SpriteType::ENEMY },
		600,
		2000,
		0,
		0,
		Direction::Horizontal::__NULL,
		Direction::Vertical::__NULL,
		nullptr,
		SpriteMovement::Type::FLOATING,
		CameraMovement::RESET_OFFSCREEN_AND_AWAY,
		false,
		false
	),
    component_ ( std::move( component ) )
{
    layer_ = layer;
};

PufferbeeSprite::~PufferbeeSprite() {};

void PufferbeeSprite::customUpdate( LevelState& level_state )
{
    if ( component_ == nullptr )
    {
        switch ( randomDirection() )
        {
            case ( Direction::Rotation::UP ):
                hit_box_.y = original_hit_box_.y - 1000;
                hit_box_.x = original_hit_box_.x;
            break;
            case ( Direction::Rotation::UP_RIGHT ):
                hit_box_.y = original_hit_box_.y - 1000;
                hit_box_.x = original_hit_box_.x + 1000;
            break;
            case ( Direction::Rotation::RIGHT ):
                hit_box_.x = original_hit_box_.x + 1000;
                hit_box_.y = original_hit_box_.y;
            break;
            case ( Direction::Rotation::DOWN_RIGHT ):
                hit_box_.y = original_hit_box_.y + 1000;
                hit_box_.x = original_hit_box_.x + 1000;
            break;
            case ( Direction::Rotation::DOWN ):
                hit_box_.y = original_hit_box_.y + 1000;
                hit_box_.x = original_hit_box_.x;
            break;
            case ( Direction::Rotation::DOWN_LEFT ):
                hit_box_.y = original_hit_box_.y + 1000;
                hit_box_.x = original_hit_box_.x - 1000;
            break;
            case ( Direction::Rotation::LEFT ):
                hit_box_.x = original_hit_box_.x - 1000;
                hit_box_.y = original_hit_box_.y;
            break;
            case ( Direction::Rotation::UP_LEFT ):
                hit_box_.y = original_hit_box_.y - 1000;
                hit_box_.x = original_hit_box_.x - 1000;
            break;
            default:
                hit_box_.y = original_hit_box_.y;
                hit_box_.x = original_hit_box_.x;
        }
    }
    else
    {
        component_->update( *this, level_state );
    }
};

void PufferbeeSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) {};

Direction::Rotation PufferbeeSprite::randomDirection() const
{
    return ( Direction::Rotation )( rand() % 9 );
};

void PufferbeeSprite::reset()
{
	resetPosition();
    if ( component_ != nullptr )
    {
        component_->reset();
    }
};
