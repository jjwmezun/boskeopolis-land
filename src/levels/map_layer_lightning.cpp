#include "map_layer_lightning.hpp"
#include "mezun_math.hpp"
#include "render.hpp"

static constexpr int ALPHA_CHANGE = 16;

static int generateNewNextDuration()
{
    return mezun::randInt( 256, 64 );
};

MapLayerLightning::MapLayerLightning( Unit::Layer layer_position )
:
    MapLayer( layer_position ),
    state_ ( State::STAGNANT ),
    timer_ ( 0 ),
    alpha_ ( 0 ),
    next_ ( generateNewNextDuration() )
{};

MapLayerLightning::~MapLayerLightning() {};

void MapLayerLightning::update( LevelState& level_state )
{
    switch ( state_ )
    {
        case ( State::STAGNANT ):
        {
            if ( timer_ >= next_ )
            {
                timer_ = 0;
                state_ = State::LIGHTENING;
            }
            else
            {
                ++timer_;
            }
        }
        break;
        case ( State::LIGHTENING ):
        {
            alpha_ += ALPHA_CHANGE;
            if ( alpha_ >= 255 )
            {
                alpha_ = 255;
                if ( timer_ > 8 )
                {
                    timer_ = 0;
                    state_ = State::DARKENING;
                }
                else
                {
                    ++timer_;
                }
            }
        }
        break;
        case ( State::DARKENING ):
        {
            alpha_ -= ALPHA_CHANGE;
            if ( alpha_ <= 0 )
            {
                alpha_ = 0;
                next_ = generateNewNextDuration();
                state_ = State::STAGNANT;
            }
        }
        break;
    }
};

void MapLayerLightning::render( const Camera& camera )
{
    Render::colorCanvas( 1, ( Uint8 )( alpha_ ) );
};