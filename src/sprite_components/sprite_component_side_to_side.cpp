#include "sprite.hpp"
#include "sprite_component_side_to_side.hpp"

SpriteComponentSideToSide::SpriteComponentSideToSide( int move_space_before_change, Direction::Simple direction )
:
    SpriteComponent (),
    past_top_for_change_
    (
        ( direction == Direction::Simple::UP || direction == Direction::Simple::LEFT ) ? Unit::BlocksToSubPixels( move_space_before_change ) : 0
    ),
    past_bottom_for_change_
    (
        ( direction == Direction::Simple::DOWN || direction == Direction::Simple::RIGHT ) ? Unit::BlocksToSubPixels( move_space_before_change ) : 0
    ),
    direction_ ( direction ),
    start_direction_ ( direction )
{};

SpriteComponentSideToSide::~SpriteComponentSideToSide() {};

void SpriteComponentSideToSide::update( Sprite& sprite, LevelState& state )
{
    switch( direction_ )
    {
        case ( Direction::Simple::UP ):
            sprite.moveUp();
            if ( sprite.topSubPixels() <= sprite.originalYSubPixels() - past_top_for_change_ )
            {
                sprite.stopY();
                direction_ = Direction::Simple::DOWN;
            }
        break;
        case ( Direction::Simple::RIGHT ):
        {
            sprite.moveRight();
            if ( sprite.leftSubPixels() >= sprite.originalXSubPixels() + past_bottom_for_change_ )
            {
                sprite.stopX();
                direction_ = Direction::Simple::LEFT;
            }
        }
        break;
        case ( Direction::Simple::LEFT ):
        {
            sprite.moveLeft();
            if ( sprite.leftSubPixels() <= sprite.originalXSubPixels() - past_top_for_change_ )
            {
                sprite.stopX();
                direction_ = Direction::Simple::RIGHT;
            }
        }
        break;
        case ( Direction::Simple::DOWN ):
            sprite.moveDown();
            if ( sprite.topSubPixels() >= sprite.originalYSubPixels() + past_bottom_for_change_ )
            {
                sprite.stopY();
                direction_ = Direction::Simple::UP;
            }
        break;
    }
};

void SpriteComponentSideToSide::reset()
{
    direction_ = start_direction_;
};
