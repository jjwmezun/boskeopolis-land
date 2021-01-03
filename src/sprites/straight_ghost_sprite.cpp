#include "straight_ghost_sprite.hpp"
#include "sprite_graphics.hpp"

StraightGhostSprite::StraightGhostSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/autumn-skirt-ghost.png", 0, 0, false, false, 0, -1, -2, 2, 4 ), x, y + 9, 14, 23, { SpriteType::ENEMY }, 2000, 2000, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY, false, false )
{};

StraightGhostSprite::~StraightGhostSprite() {};

void StraightGhostSprite::customUpdate( LevelState& level_state )
{
    flipGraphicsOnRight();
    moveInDirectionX();

    // Update graphics.
    if ( animation_timer_.update() )
    {
        ++walk_counter_;
    }
    graphics_->current_frame_x_ = WALK_FRAMES[ walk_counter_.value() ];
};

void StraightGhostSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
    if ( direction_x_ == Direction::Horizontal::__NULL && them.hasType( SpriteType::HERO ) )
    {
        direction_x_ = ( them.centerXSubPixels() > centerXSubPixels() ) ? Direction::Horizontal::RIGHT : Direction::Horizontal::LEFT;
    }
};

void StraightGhostSprite::reset()
{
    resetPosition();
    fullStopX();
    direction_x_ = Direction::Horizontal::__NULL;
};