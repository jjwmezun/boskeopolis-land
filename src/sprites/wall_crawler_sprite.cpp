#include "block_system.hpp"
#include "camera.hpp"
#include "collision.hpp"
#include "level_state.hpp"
#include "sprite_graphics.hpp"
#include "wall_crawler_sprite.hpp"

static constexpr int MOVE_SPEED = 1000;

WallCrawlerSprite::WallCrawlerSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/spider.png" ), x, y, 16, 16, { SpriteType::ENEMY, SpriteType::BOPPABLE }, 500, 500, 0, 0, Direction::Horizontal::LEFT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
    fall_delay_ ( 0 )
{
    direction_ = Direction::Simple::DOWN;
};

WallCrawlerSprite::~WallCrawlerSprite() {};

void WallCrawlerSprite::customUpdate( LevelState& level_state )
{
    BlockSystem& blocks = level_state.blocks();
    switch ( direction_ )
    {
        case ( Direction::Simple::__NULL ):
        {
            if ( blocks.blocksInTheWay( getFirstRect(), BlockComponent::Type::SOLID ) )
            {
                direction_ = Direction::horizontalToSimple( direction_x_ );
            }
            else
            {
                hit_box_.y += 2000;
            }
        }
        break;

        case ( Direction::Simple::UP ):
        {
            if ( !blocks.blocksInTheWay( getFirstRect(), BlockComponent::Type::SOLID ) )
            {
                direction_ = ( direction_x_ == Direction::Horizontal::LEFT ) ? Direction::Simple::LEFT : Direction::Simple::RIGHT;
            }
            else if ( blocks.blocksInTheWay( getSecondRect(), BlockComponent::Type::SOLID ) )
            {
                direction_ = ( direction_x_ == Direction::Horizontal::LEFT ) ? Direction::Simple::RIGHT : Direction::Simple::LEFT;
            }
            else
            {
                hit_box_.y -= MOVE_SPEED;
            }
        }
        break;

        case ( Direction::Simple::RIGHT ):
        {
            if ( !blocks.blocksInTheWay( getFirstRect(), BlockComponent::Type::SOLID ) )
            {
                direction_ = ( direction_x_ == Direction::Horizontal::LEFT ) ? Direction::Simple::UP : Direction::Simple::DOWN;
            }
            else if ( blocks.blocksInTheWay( getSecondRect(), BlockComponent::Type::SOLID ) )
            {
                direction_ = ( direction_x_ == Direction::Horizontal::LEFT ) ? Direction::Simple::DOWN : Direction::Simple::UP;
            }
            else
            {
                hit_box_.x += MOVE_SPEED;
            }
        }
        break;

        case ( Direction::Simple::DOWN ):
        {
            if ( !blocks.blocksInTheWay( getFirstRect(), BlockComponent::Type::SOLID ) )
            {
                direction_ = ( direction_x_ == Direction::Horizontal::LEFT ) ? Direction::Simple::RIGHT : Direction::Simple::LEFT;
            }
            else if ( blocks.blocksInTheWay( getSecondRect(), BlockComponent::Type::SOLID ) )
            {
                direction_ = ( direction_x_ == Direction::Horizontal::LEFT ) ? Direction::Simple::LEFT : Direction::Simple::RIGHT;
            }
            else
            {
                hit_box_.y += MOVE_SPEED;
            }
        }
        break;

        case ( Direction::Simple::LEFT ):
        {
            if ( !blocks.blocksInTheWay( getFirstRect(), BlockComponent::Type::SOLID ) )
            {
                direction_ = ( direction_x_ == Direction::Horizontal::LEFT ) ? Direction::Simple::DOWN : Direction::Simple::UP;
            }
            else if ( blocks.blocksInTheWay( getSecondRect(), BlockComponent::Type::SOLID ) )
            {
                direction_ = ( direction_x_ == Direction::Horizontal::LEFT ) ? Direction::Simple::UP : Direction::Simple::DOWN;
            }
            else
            {
                hit_box_.x -= MOVE_SPEED;
            }
        }
        break;
    }

    if 
    (
        ( direction_x_ == Direction::Horizontal::LEFT && direction_ == Direction::Simple::RIGHT ) ||
        ( direction_x_ == Direction::Horizontal::RIGHT && direction_ == Direction::Simple::LEFT )
    )
    {
        ++fall_delay_;
    }
    else
    {
        fall_delay_ = 0;
    }

    graphics_->flip_y_ = direction_x_ == Direction::Horizontal::RIGHT;
    switch ( direction_ )
    {
        case ( Direction::Simple::__NULL ):
        {
            graphics_->rotation_ = 0.0;
        }
        break;

        case ( Direction::Simple::UP ):
        {
            graphics_->rotation_ = 90.0;
        }
        break;

        case ( Direction::Simple::RIGHT ):
        {
            graphics_->rotation_ = 180.0;
        }
        break;

        case ( Direction::Simple::DOWN ):
        {
            graphics_->rotation_ = 270.0;
        }
        break;

        case ( Direction::Simple::LEFT ):
        {
            graphics_->rotation_ = 0.0;
        }
        break;
    }
};

void WallCrawlerSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
    if 
    (
        fall_delay_ > 60 &&
        (
            ( direction_x_ == Direction::Horizontal::LEFT && direction_ == Direction::Simple::RIGHT ) ||
            ( direction_x_ == Direction::Horizontal::RIGHT && direction_ == Direction::Simple::LEFT )
        ) &&
        them.hasType( SpriteType::HERO )
    )
    {
        if
        (
            them.rightSubPixels() > hit_box_.x - 32000 &&
            them.hit_box_.x < rightSubPixels() + 32000 &&
            them.bottomSubPixels() > hit_box_.y &&
            them.hit_box_.y < bottomSubPixels() + Unit::BlocksToSubPixels( 8 )
        )
        {
            direction_ = Direction::Simple::__NULL;
            if ( them.rightSubPixels() < hit_box_.x )
            {
                direction_x_ = Direction::Horizontal::LEFT;
            }
            else if ( them.hit_box_.x > rightSubPixels() )
            {
                direction_x_ = Direction::Horizontal::RIGHT;
            }
        }
    }
};

void WallCrawlerSprite::render( Camera& camera, bool priority )
{
    graphics_->render( Unit::SubPixelsToPixels( hit_box_ ), &camera, priority );
    /*
    Render::renderRectDebug( camera.relativeRect( Unit::SubPixelsToPixels( getFirstRect() ) ), { 255, 0, 0, 128 } );
    Render::renderRectDebug( camera.relativeRect( Unit::SubPixelsToPixels( getSecondRect() ) ), { 0, 0, 255, 128 } );
    if 
    (
        ( direction_x_ == Direction::Horizontal::LEFT && direction_ == Direction::Simple::RIGHT ) ||
        ( direction_x_ == Direction::Horizontal::RIGHT && direction_ == Direction::Simple::LEFT )
    )
    {
        Render::renderRectDebug( camera.relativeRect( Unit::SubPixelsToPixels( sdl2::SDLRect{ hit_box_.x - 32000, hit_box_.y, hit_box_.w + 64000, Unit::BlocksToSubPixels( 8 ) } ) ), { 0, 255, 0, 128 } );
    }*/
};

sdl2::SDLRect WallCrawlerSprite::getFirstRect() const
{
    switch ( direction_ )
    {
        case ( Direction::Simple::__NULL ):
        {
            return { hit_box_.x + 4000, bottomSubPixels() - 1000, hit_box_.w - 8000, 1000 };
        }
        break;

        case ( Direction::Simple::UP ):
        {
            return ( direction_x_ == Direction::Horizontal::LEFT ) ? sdl2::SDLRect{ hit_box_.x - 16000, hit_box_.y - 1000, 16000, hit_box_.h + 1000 } : sdl2::SDLRect{ rightSubPixels(), hit_box_.y - 1000, 16000, hit_box_.h };
        }
        break;

        case ( Direction::Simple::RIGHT ):
        {
            return ( direction_x_ == Direction::Horizontal::LEFT ) ? sdl2::SDLRect{ hit_box_.x, hit_box_.y - 16000, hit_box_.w + 1000, hit_box_.h + 1000 } : sdl2::SDLRect{ hit_box_.x, bottomSubPixels() - 1000, hit_box_.w + 1000, hit_box_.h };
        }
        break;

        case ( Direction::Simple::DOWN ):
        {
            return ( direction_x_ == Direction::Horizontal::LEFT ) ? sdl2::SDLRect{ rightSubPixels(), hit_box_.y, 16000, hit_box_.h + 1000 } : sdl2::SDLRect{ hit_box_.x - 16000, hit_box_.y, 16000, hit_box_.h + 4000 };
        }
        break;

        case ( Direction::Simple::LEFT ):
        {
            return ( direction_x_ == Direction::Horizontal::LEFT ) ? sdl2::SDLRect{ hit_box_.x - 1000, bottomSubPixels() - 1000, hit_box_.w + 1000, hit_box_.h } : sdl2::SDLRect{ hit_box_.x - 1000, hit_box_.y - 16000, hit_box_.w + 1000, hit_box_.h + 1000 };
        }
        break;
    }
};

sdl2::SDLRect WallCrawlerSprite::getSecondRect() const
{
    switch ( direction_ )
    {
        case ( Direction::Simple::__NULL ):
        {
            return { 0, 0, 0, 0 };
        }
        break;

        case ( Direction::Simple::UP ):
        {
            return sdl2::SDLRect{ hit_box_.x, hit_box_.y - 1000, hit_box_.w - 1000, 1000 };
        }
        break;

        case ( Direction::Simple::RIGHT ):
        {
            return sdl2::SDLRect{ rightSubPixels() - 1000, hit_box_.y, 1000, hit_box_.h - 4000 };
        }
        break;

        case ( Direction::Simple::DOWN ):
        {
            return ( direction_x_ == Direction::Horizontal::LEFT ) ? sdl2::SDLRect{ hit_box_.x, bottomSubPixels() - 1000, hit_box_.w - 4000, 1000 } : sdl2::SDLRect{ hit_box_.x + 4000, bottomSubPixels() - 1000, hit_box_.w - 4000, 1000 };
        }
        break;

        case ( Direction::Simple::LEFT ):
        {
            return sdl2::SDLRect{ hit_box_.x - 1000, hit_box_.y + 4000, 1000, hit_box_.h - 4000 };
        }
        break;
    }
};