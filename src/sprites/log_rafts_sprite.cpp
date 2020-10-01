#include "log_rafts_sprite.hpp"
#include "collision.hpp"
#include "level_state.hpp"
#include "sprite_graphics.hpp"

#include <cstdio>

static constexpr int Y_ADJUST =  + Unit::PixelsToSubPixels( 8 );

LogRaftsSprite::LogRaftsSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/raft.png" ), 0, y, 64, 16, {}, 1500, 1500, 0, 0, Direction::Horizontal::LEFT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT )
{
    for ( int i = 0; i < MAX_RAFTS; ++i )
    {
        rafts_[ i ] = { 0, hit_box_.y, hit_box_.w, hit_box_.h };
    }
};

LogRaftsSprite::~LogRaftsSprite() {};

void LogRaftsSprite::customUpdate( LevelState& level_state )
{
    moveLeft();
    if ( hit_box_.x < -Unit::BlocksToSubPixels( 8 ) )
    {
        hit_box_.x = 0;
    }

    const Camera& camera = level_state.camera();
    int x = hit_box_.x;
    while ( x < Unit::PixelsToSubPixels( camera.x() ) - Unit::BlocksToSubPixels( 8 ) )
    {
        x += Unit::BlocksToSubPixels( 8 );
    }

    for ( int i = 0; i < MAX_RAFTS; ++i )
    {
        if ( x <= Unit::BlocksToSubPixels( 34 ) )
        {
            rafts_[ i ].y = hit_box_.y + ( Unit::BlocksToSubPixels( 34 ) - x ) + Y_ADJUST;
        }
        else if ( x < Unit::BlocksToSubPixels( 62 ) )
        {
            rafts_[ i ].y = hit_box_.y + Y_ADJUST;
        }
        else if ( x <= Unit::BlocksToSubPixels( 65 ) )
        {
            rafts_[ i ].y = hit_box_.y - Unit::BlocksToSubPixels( 3 ) + ( Unit::BlocksToSubPixels( 65 ) - x ) + Y_ADJUST;
        }
        else if ( x < Unit::BlocksToSubPixels( 138 ) )
        {
            rafts_[ i ].y = hit_box_.y - Unit::BlocksToSubPixels( 3 ) + Y_ADJUST;
        }
        else if ( x <= Unit::BlocksToSubPixels( 141 ) )
        {
            rafts_[ i ].y = hit_box_.y - Unit::BlocksToSubPixels( 6 ) + ( Unit::BlocksToSubPixels( 141 ) - x ) + Y_ADJUST;
        }
        else if ( x < Unit::BlocksToSubPixels( 147 ) )
        {
            rafts_[ i ].y = hit_box_.y - Unit::BlocksToSubPixels( 6 ) + Y_ADJUST;
        }
        else if ( x <= Unit::BlocksToSubPixels( 150 ) )
        {
            rafts_[ i ].y = hit_box_.y - Unit::BlocksToSubPixels( 9 ) + ( Unit::BlocksToSubPixels( 150 ) - x ) + Y_ADJUST;
        }
        else if ( x < Unit::BlocksToSubPixels( 156 ) )
        {
            rafts_[ i ].y = hit_box_.y - Unit::BlocksToSubPixels( 9 ) + Y_ADJUST;
        }
        else if ( x <= Unit::BlocksToSubPixels( 159 ) )
        {
            rafts_[ i ].y = hit_box_.y - Unit::BlocksToSubPixels( 12 ) + ( Unit::BlocksToSubPixels( 159 ) - x ) + Y_ADJUST;
        }
        else
        {
            rafts_[ i ].y = hit_box_.y - Unit::BlocksToSubPixels( 12 ) + Y_ADJUST;
        }

        rafts_[ i ].x = x;
        x += Unit::BlocksToSubPixels( 8 );
    }
};

void LogRaftsSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( them.hasType( SpriteType::HERO ) )
	{
        const bool blocks_in_the_way = level_state.blocks().blocksInTheWay
        (
            {
                them.hit_box_.x,
                them.topSubPixels(),
                1000,
                them.heightSubPixels()
            },
            BlockComponent::Type::SOLID
        );
        
        for ( int i = 0; i < MAX_RAFTS; ++i )
        {
            const Collision collision = movement_->testCollision( them, rafts_[ i ] );
            if ( collision.collideAny() )
            {
                if ( !blocks_in_the_way && collision.collideBottom() )
                {
                    them.hit_box_.x += vx_;
                }
                them.collideStopAny( collision );
            }
        }
	}
};

void LogRaftsSprite::render( const Camera& camera ) const
{
    for ( int i = 0; i < MAX_RAFTS; ++i )
    {
        graphics_->render( Unit::SubPixelsToPixels( rafts_[ i ] ), &camera );
    }
};
