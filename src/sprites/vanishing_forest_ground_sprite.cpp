#include "audio.hpp"
#include "camera.hpp"
#include "collision.hpp"
#include "level_state.hpp"
#include "render.hpp"
#include "sprite_graphics.hpp"
#include "vanishing_forest_ground_sprite.hpp"

static constexpr int X_PADDING_BLOCKS = 1;
static constexpr int NUMBER_OF_ALPHAS = 18;
static constexpr Uint8 ALPHAS_LIST[ NUMBER_OF_ALPHAS ] =
{
    255,
    224,
    192,
    160,
    128,
    96,
    64,
    32,
    0,
    0,
    32,
    64,
    96,
    128,
    160,
    192,
    224,
    255
};

VanishingForestGroundSprite::VanishingForestGroundSprite( int x, int y, int length )
:
	Sprite( nullptr, x, y, Unit::BlocksToPixels( length ), Unit::BlocksToPixels( 12 ), {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
    texture_created_ ( false ),
    texture_ ( Unit::BlocksToPixels( length + 2 ), Unit::BlocksToPixels( 13 ) ),
    alpha_ ( 255 ),
    ground_state_ ( GroundState::NORMAL ),
    timer_ ( 0 )
{
    layer_ = Unit::Layer::BEFORE_SPRITES_2;
    texture_.init();
};

VanishingForestGroundSprite::~VanishingForestGroundSprite()
{
    texture_.destroy();
};

void VanishingForestGroundSprite::customUpdate( LevelState& level_state )
{
    switch ( ground_state_ )
    {
        case ( GroundState::NORMAL ):
        {
            if ( !texture_created_ )
            {
                texture_.startDrawing();
                // Texture has padding on both sides where it is drawn but is not interactable
                // by player.
                const int x_limit = widthPixels() + Unit::BlocksToPixels( X_PADDING_BLOCKS * 2 );
                // Texture has 1 extra block vertically to deal with extra top grass graphic,
                // which is not part o’ the actual block.
                const int y_limit = heightPixels() + Unit::BlocksToPixels( 1 );
                for ( int x = 0; x < x_limit; x += Unit::BlocksToPixels( 1 ) )
                {
                    // Draw top grass.
                    Render::renderObject( "tilesets/woods.png", { 16, 0, 16, 16 }, { x, 0, 16, 16 } );
                    // Draw top ground.
                    Render::renderObject( "tilesets/woods.png", { 16, 16, 16, 16 }, { x, Unit::BlocksToPixels( 1 ), 16, 16 } );
                    // Draw inner ground from just below top ground down to bottom.
                    for ( int y = Unit::BlocksToPixels( 2 ); y < y_limit; y += Unit::BlocksToPixels( 1 ) )
                    {
                        Render::renderObject( "tilesets/woods.png", { 16, 32, 16, 16 }, { x, y, 16, 16 } );
                    }
                }
                texture_.endDrawing();
                texture_created_ = true;
            }

            if ( timer_ >= 8 )
            {
                timer_ = 0;
                ground_state_ = GroundState::FLASHING;
            }
        }
        break;
        case ( GroundState::FLASHING ):
        {
            if ( timer_ >= 18 * 3 + 10 )
            {
                sprite_interact_ = false;
                ground_state_ = GroundState::GONE;
            }
            else
            {
                ++timer_;
                texture_.setAlpha( ALPHAS_LIST[ timer_ % NUMBER_OF_ALPHAS ] );
            }
        }
        break;
        case ( GroundState::GONE ):
            // Do nothing.
        break;
    }
};

void VanishingForestGroundSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
    switch ( ground_state_ )
    {
        case ( GroundState::NORMAL ):
        {
            if ( them.hasType( SpriteType::HERO ) )
            {
                if ( doHeroCollision( them ) )
                {
                    ++timer_;
                }
                else
                {
                    timer_ = 0; // Reset timer if no longer colliding.
                }
            }
        }
        break;
        case ( GroundState::FLASHING ):
        {
            if ( them.hasType( SpriteType::HERO ) )
            {
                doHeroCollision( them );
            }
        }
        break;
        case ( GroundState::GONE ):
            // Do nothing.
        break;
    }

};

void VanishingForestGroundSprite::render( const Camera& camera ) const
{
    switch ( ground_state_ )
    {
        case ( GroundState::NORMAL ): // fallthru
        case ( GroundState::FLASHING ):
        {
            const sdl2::SDLRect dest =
            {
                camera.relativeX( xPixels() - Unit::BlocksToPixels( X_PADDING_BLOCKS ) ),
                camera.relativeY( yPixels() - Unit::BlocksToPixels( 1 ) ),
                texture_.getWidth(),
                texture_.getHeight()
            };
            Render::renderRenderBox( texture_.getTexture(), texture_.getSrc(), dest );
        }
        break;
        case ( GroundState::GONE ):
            // Do nothing.
        break;
    }
};

void VanishingForestGroundSprite::reset()
{
    sprite_interact_ = true;
    timer_ = 0;
    ground_state_ = GroundState::NORMAL;
    texture_.setAlpha( 255 );
};

bool VanishingForestGroundSprite::doHeroCollision( Sprite& them ) const
{
    const Collision collision = them.testBlockCollision( *this );
    if ( collision.collideAny() )
    {
        them.collideStopAny( collision );
        return true;
    }
    return false;
};
