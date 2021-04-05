#include "log_rafts_sprite.hpp"
#include "collision.hpp"
#include "level_state.hpp"
#include "sprite_graphics.hpp"

static constexpr int Y_ADJUST =  + Unit::PixelsToSubPixels( 8 );
static constexpr int SPACE_TWEEN_RAFTS = Unit::BlocksToSubPixels( 8 );

LogRaftsSprite::LogRaftsSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/raft.png" ), 0, y, 64, 16, {}, 1500, 1500, 0, 0, Direction::Horizontal::LEFT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT )
{
    // Set position for each raft.
    for ( int i = 0; i < MAX_RAFTS; ++i )
    {
        rafts_[ i ] = { 0, hit_box_.y, hit_box_.w, hit_box_.h };
    }
};

LogRaftsSprite::~LogRaftsSprite() {};

void LogRaftsSprite::customUpdate( LevelState& level_state )
{
    moveLeft();

    // Keep master x ’bove max increment under 0.
    // This is just to keep x from hitting int min & wrapping round or causing any other glitch.
    // I picked this # ’cause it’s just past the left edge o’ the screen.
    // Anything higher would cause the raft to weirdly disappear before it reaches the edge o’ the screen,
    // while anything lower would waste time with mo’ loops thru the next while loop.
    if ( hit_box_.x < -SPACE_TWEEN_RAFTS )
    {
        hit_box_.x = 0;
    }

    // Initialize x so that it’s the smallest space increment o’ the master hitbox’s x that shows onscreen.
    const Camera& camera = level_state.camera();
    int x = hit_box_.x;
    while ( x < Unit::PixelsToSubPixels( camera.x() ) - SPACE_TWEEN_RAFTS )
    {
        x += SPACE_TWEEN_RAFTS;
    }

    // Position all rafts.
    for ( int i = 0; i < MAX_RAFTS; ++i )
    {
        // Manually adjust y position based on x position.
        // These hard-coded #s represent the places where there are waterfalls & the raft needs to start falling.
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

        // Set current raft to x position & increment x to next space increment for next raft.
        rafts_[ i ].x = x;
        x += SPACE_TWEEN_RAFTS;
    }
};

void LogRaftsSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( them.hasType( SpriteType::HERO ) )
	{
        // Test if blocks to the left o’ player.
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
        
        // For each raft, test if player is colliding with it.
        for ( int i = 0; i < MAX_RAFTS; ++i )
        {
            const Collision collision = movement_->testBlockCollision( them, rafts_[ i ] );
            if ( collision.collideAny() )
            {
                // Don’t move player with raft if solid blocks are just to left o’ player so they aren’t moved thru
                // blocks.
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
    // Render all rafts.
    for ( int i = 0; i < MAX_RAFTS; ++i )
    {
        graphics_->render( Unit::SubPixelsToPixels( rafts_[ i ] ), &camera );
    }
};
