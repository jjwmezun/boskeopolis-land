#include "collision.hpp"
#include "penguin_sprite.hpp"
#include "sprite_graphics.hpp"

PenguinSprite::PenguinSprite( int x, int y )
:
    Sprite( std::make_unique<SpriteGraphics> ( "sprites/penguin.png" ), x, y, 16, 24, { SpriteType::ENEMY, SpriteType::BOPPABLE }, 1200, 4000, 0, 0, Direction::Horizontal::LEFT ),
    turning_ ( false ),
    animation_frame_ (),
    delay_ (),
    animation_timer_ ()
{};
PenguinSprite::~PenguinSprite() {};

void PenguinSprite::customUpdate( LevelState& level_state )
{
    if ( turning_ )
    {
        if ( delay_.done() )
        {
            turning_ = false;
            delay_.stop();
        }
        else if ( delay_.on() )
        {
            delay_.update();
        }
        else
        {
            delay_.start();
            acceleration_x_ = 0;
        }
    }
    else
    {
        switch( direction_x_ )
        {
            case( Direction::Horizontal::LEFT ):
            {
                moveLeft();
                if ( hit_box_.x < original_hit_box_.x - Unit::BlocksToSubPixels( 1 ) )
                {
                    direction_x_ = Direction::Horizontal::RIGHT;
                    turning_ = true;
                }
            }
            break;

            case( Direction::Horizontal::RIGHT ):
            {
                moveRight();
                if ( hit_box_.x > original_hit_box_.x )
                {
                    direction_x_ = Direction::Horizontal::LEFT;
                    turning_ = true;
                }
            }
            break;
        }
    }
    updateGraphics();
};

void PenguinSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) {};

void PenguinSprite::updateGraphics()
{
	flipGraphicsOnRight();
	if ( animation_timer_.hit() )
	{
		++animation_frame_;
		switch ( animation_frame_() )
		{
			case ( 0 ):
			case ( 2 ):
			{
				graphics_->current_frame_x_ = 0;
			}
			break;
			case ( 1 ):
			{
				graphics_->current_frame_x_ = 16;
			}
			break;
			case ( 3 ):
			{
				graphics_->current_frame_x_ = 32;
			}
			break;
		}
	}
	animation_timer_.update();
};
