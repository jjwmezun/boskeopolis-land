#include "config.hpp"
#include "controls.hpp"
#include "gfx.hpp"

#include <cstdio>

static bool running = true;
static double fixed_timestep = 0.0f;
static double fixed_timestep_update = 1.0f / fixed_timestep;
float total_dt = 0.0f;
unsigned int dtcount = 0;

int main( int argc, char ** argv )
{
    BSL::GFX::init();
    BSL::Controls::init();

    BSL::GFX::addGraphicRectGradient( 1, 0, 0, BSL::WINDOW_WIDTH_PIXELS, BSL::WINDOW_HEIGHT_PIXELS, 224, 48 );

    int tileset_texture = BSL::GFX::loadFileAsTexture( "tilesets/urban.png" );
    BSL::GFX::Tile tiles[ BSL::WINDOW_WIDTH_BLOCKS * 2 * BSL::WINDOW_HEIGHT_BLOCKS ];
    for ( unsigned int i = 0; i < BSL::WINDOW_WIDTH_BLOCKS * 2 * BSL::WINDOW_HEIGHT_BLOCKS; ++i )
    {
        tiles[ i ].set = false;
        tiles[ i ].x = 0;
        tiles[ i ].y = 0;
        tiles[ i ].animation = 0;
    }
    for ( unsigned int x = 0; x < BSL::WINDOW_WIDTH_BLOCKS * 2; ++x )
    {
        tiles[ BSL::WINDOW_WIDTH_BLOCKS * 2 * ( BSL::WINDOW_HEIGHT_BLOCKS - 2 ) + x ].set = true;
        tiles[ BSL::WINDOW_WIDTH_BLOCKS * 2 * ( BSL::WINDOW_HEIGHT_BLOCKS - 2 ) + x ].x = 0;
        tiles[ BSL::WINDOW_WIDTH_BLOCKS * 2 * ( BSL::WINDOW_HEIGHT_BLOCKS - 2 ) + x ].y = 0;
        tiles[ BSL::WINDOW_WIDTH_BLOCKS * 2 * ( BSL::WINDOW_HEIGHT_BLOCKS - 1 ) + x ].set = true;
        tiles[ BSL::WINDOW_WIDTH_BLOCKS * 2 * ( BSL::WINDOW_HEIGHT_BLOCKS - 1 ) + x ].x = 1;
        tiles[ BSL::WINDOW_WIDTH_BLOCKS * 2 * ( BSL::WINDOW_HEIGHT_BLOCKS - 1 ) + x ].y = 0;
    }
    BSL::GFX::Tilemap tilemap = BSL::GFX::addGraphicTilemap
    (
        0,
        tileset_texture,
        tiles,
        BSL::WINDOW_WIDTH_BLOCKS * 2,
        BSL::WINDOW_HEIGHT_BLOCKS,
        0,
        -16
    );
    float taccy = 0.05f;
    float tvy = 0.0f;
    float ty = -16.0f;
    int prevty = 0;

    int objects_texture = BSL::GFX::loadFileAsTexture( "tilesets/objects.png" );
    BSL::GFX::Tile objtiles[ BSL::WINDOW_WIDTH_BLOCKS * 2 * BSL::WINDOW_HEIGHT_BLOCKS ];
    for ( unsigned int i = 0; i < BSL::WINDOW_WIDTH_BLOCKS * 2 * BSL::WINDOW_HEIGHT_BLOCKS; ++i )
    {
        objtiles[ i ].set = false;
        objtiles[ i ].x = 0;
        objtiles[ i ].y = 0;
        objtiles[ i ].animation = 0;
    }
    for ( unsigned int x = 6; x < 12; ++x )
    {
        objtiles[ BSL::WINDOW_WIDTH_BLOCKS * 2 * ( BSL::WINDOW_HEIGHT_BLOCKS - 6 ) + x ].set = true;
        objtiles[ BSL::WINDOW_WIDTH_BLOCKS * 2 * ( BSL::WINDOW_HEIGHT_BLOCKS - 6 ) + x ].x = 0;
        objtiles[ BSL::WINDOW_WIDTH_BLOCKS * 2 * ( BSL::WINDOW_HEIGHT_BLOCKS - 6 ) + x ].y = 0;
        objtiles[ BSL::WINDOW_WIDTH_BLOCKS * 2 * ( BSL::WINDOW_HEIGHT_BLOCKS - 6 ) + x ].animation = 6;
        objtiles[ BSL::WINDOW_WIDTH_BLOCKS * 2 * ( BSL::WINDOW_HEIGHT_BLOCKS - 6 ) + x ].frame = 0;
    }
    addGraphicTilemap
    (
        0,
        objects_texture,
        objtiles,
        BSL::WINDOW_WIDTH_BLOCKS * 2,
        BSL::WINDOW_HEIGHT_BLOCKS,
        0,
        0
    );

    BSL::GFX::addGraphicText
    (
        "¡BAM! ¡LOOK @ THAT BACON SIZZLE!"
    );

    float accx = 0.0f;
    float vx = 0.0f;
    float prevx = -8.0f;
    float x = -8.0f;
    float accy = 0.0f;
    float vy = 0.0f;
    float prevy = -8.0f;
    float y = -8.0f;
    float camerax = 0.0f;
    float cameray = 0.0f;
    float animation = 0.0f;
    unsigned int animation_frame = 0;
    unsigned int animation_frames[ 4 ] = { 0, 16, 0, 32 };

    int autumn_texture = BSL::GFX::loadFileAsTexture( "sprites/autumn.png" );
    if ( autumn_texture < 0 )
    {
        return -1;
    }
    BSL::GFX::Sprite autumn_sprite = BSL::GFX::addGraphicSprite
    (
        0,
        static_cast<unsigned int> ( autumn_texture ),
        16,
        0,
        16,
        26,
        x,
        y,
        1.0f
    );

    unsigned char palette = 0;
    float palette_timer = 0.0f;

    double prev_time = BSL::GFX::getTime();
    double accumulated_time = prev_time;

    while ( running )
    {
        if ( BSL::GFX::testHasClosed() )
        {
            running = false;
        }
        else
        {
            BSL::GFX::handleEvents();
            double current_time = BSL::GFX::getTime();
            double timechange = current_time - prev_time;
            double fps = 1.0 / timechange;
            float dt = 60.0f / ( float )( fps );

            total_dt += ( 60.0f / dt );
            ++dtcount;

            if ( BSL::Controls::heldRight() )
            {
                accx = 1.0f;
            }
            else if ( BSL::Controls::heldLeft() )
            {
                accx = -1.0f;
            }
            else
            {
                accx = 0.0f;
            }

            if ( BSL::Controls::heldUp() )
            {
                accy = -1.0f;
            }
            else if ( BSL::Controls::heldDown() )
            {
                accy = 1.0f;
            }
            else
            {
                accy = 0.0f;
            }

            vx = vx + accx * dt;
            if ( vx > 4.0f )
            {
                vx = 4.0f;
            }
            else if ( vx < -4.0f )
            {
                vx = -4.0f;
            }

            if ( accx == 0.0f )
            {
                vx /= ( 1.0f + 0.25f * dt );
            }

            x = x + vx * dt;

            if ( x != prevx )
            {
                autumn_sprite.setX( static_cast<int> ( x ) );
            }
            prevx = x;

            vy += accy * dt;
            if ( vy > 4.0f )
            {
                vy = 4.0f;
            }
            else if ( vy < -4.0f )
            {
                vy = -4.0f;
            }

            if ( accy == 0.0f )
            {
                vy /= ( 1.0f + 0.25f * dt );
            }

            y = y + vy * dt;

            if ( y != prevy )
            {
                autumn_sprite.setY( static_cast<int> ( y ) );
            }
            prevy = y;

            if ( x + 16.0f > camerax + ( static_cast<float>( BSL::WINDOW_WIDTH_PIXELS ) * 0.75f ) )
            {
                camerax = x + 16.0f - ( static_cast<float>( BSL::WINDOW_WIDTH_PIXELS ) * 0.75f );
                if ( camerax + BSL::WINDOW_WIDTH_PIXELS > 2000.0f )
                {
                    camerax = 2000.0f - BSL::WINDOW_WIDTH_PIXELS;
                }
                BSL::GFX::setCameraX( camerax );
            }
            else if ( x < camerax + ( static_cast<float>( BSL::WINDOW_WIDTH_PIXELS ) * 0.25f ) )
            {
                camerax = x - ( static_cast<float>( BSL::WINDOW_WIDTH_PIXELS ) * 0.25f );
                if ( camerax < 0.0f )
                {
                    camerax = 0.0f;
                }
                BSL::GFX::setCameraX( camerax );
            }

            animation += dt;
            if ( animation >= 8.0f )
            {
                animation -= 8.0f;
                ++animation_frame;
                if ( animation_frame >= 4 )
                {
                    animation_frame = 0;
                }
                autumn_sprite.setSrcX( animation_frames[ animation_frame ] );
            }

            tvy += taccy;
            if ( tvy >= 1.0f )
            {
                taccy = -0.05f;
            }
            else if ( tvy <= -1.0f )
            {
                taccy = 0.05f;
            }
            ty += tvy;
            if ( static_cast<int> ( ty ) != prevty )
            {
                tilemap.setY( static_cast<int> ( ty ) );
            }
            prevty = static_cast<int> ( ty );

                BSL::GFX::setPalette( 255 );

            BSL::GFX::update( dt );
            prev_time = current_time;
        }
    }

    printf( "%f\n", total_dt / dtcount );
    BSL::GFX::close();

    return 0;
};