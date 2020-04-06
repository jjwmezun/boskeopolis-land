#include "moving_space_stone_sprite.hpp"
#include "block_component.hpp"
#include "block_system.hpp"
#include "collision.hpp"
#include "level_state.hpp"
#include "sprite_graphics.hpp"

MovingSpaceStoneSprite::MovingSpaceStoneSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/moving-space-block.png" ), x, y, 32, 32, {}, 250, 2000, 0, 0, Direction::Horizontal::RIGHT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY )
{};

MovingSpaceStoneSprite::~MovingSpaceStoneSprite() {};

void MovingSpaceStoneSprite::customUpdate( LevelState& level_state )
{
    moveInDirectionX();
    switch ( direction_x_ )
    {
        case ( Direction::Horizontal::LEFT ):
        {
            if ( hit_box_.x < original_hit_box_.x )
            {
                direction_x_ = Direction::Horizontal::RIGHT;
            }
        }
        break;
        case ( Direction::Horizontal::RIGHT ):
        {
            if ( hit_box_.x > original_hit_box_.x + Unit::BlocksToSubPixels( 4 ) )
            {
                direction_x_ = Direction::Horizontal::LEFT;
            }
        }
        break;
    }
};

void MovingSpaceStoneSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
    if ( their_collision.collideAny() )
    {
        them.collideStopAny( their_collision );
        if ( their_collision.collideBottom() )
        {
            if
            (
                !level_state.blocks().blocksInTheWay
                (
                    {
                        them.rightSubPixels(),
                        them.topSubPixels(),
                        1000,
                        them.heightSubPixels()
                    },
                    BlockComponent::Type::SOLID
                )
            )
            {
                them.hit_box_.x += vx_;
            }
        }
    }
};
