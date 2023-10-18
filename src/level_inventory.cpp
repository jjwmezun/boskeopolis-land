#include "config.hpp"
#include "game.hpp"
#include "level.hpp"
#include "level_inventory.hpp"

namespace BSL
{
    void LevelInventory::init( Game & game )
    {
        const float y = WINDOW_HEIGHT_PIXELS - 32.0f;
        ArgList text_args = 
        {
            { "num", 0.0f },
            { "maxdigits", 5 },
            { "x", 16.0f },
            { "y", y},
            { "type", "gradient" },
            { "dir", Dir::XY::DOWN },
            { "color1", 64 },
            { "color2", 16 },
            { "shadow", 0.25f }
        };
        float x = 16.0f;

        // Render box.
        game.render().addMenuBox
        (
            Text::Align::LEFT,
            Text::Valign::TOP,
            static_cast<float>( WINDOW_WIDTH_PIXELS - 16 ),
            32.0f,
            {
                { "x", 8.0f },
                { "y", static_cast<float>( WINDOW_HEIGHT_PIXELS - 40 ) }
            }
        );

        // Render HP
        for ( unsigned int i = 0; i < hp_; ++i )
        {
            hp_gfx_.emplace_back
            (
                game.render().addSprite
                (
                    "misc/inventory.png",
                    0.0f,
                    0.0f,
                    8.0f,
                    8.0f,
                    x,
                    y,
                    { { "scrollx", 1.0f }, { "scrolly", 1.0f }, { "layer", Layer::AFTER_FG_2 } }
                )
            );
            x += 8.0f;
        }
        x += 8.0f;

        // Render funds.
        text_args[ "x" ] = x;
        text_args[ "text" ] = "₧";
        game.render().addText( text_args );
        x += 8.0f;
        text_args[ "x" ] = x;
        funds_gfx_ = game.render().addCounter( text_args );
        funds_gfx_.setNumber( funds_show_ );

        // Render clock.
        x += ( 8.0f * 6 );
        text_args[ "x" ] = x;
        clock_.init( game, text_args );

        x += ( 8.0f * 6 );
    };

    void LevelInventory::update( float dt, const Level & level )
    {
        clock_.addSeconds( 1.0f / 60.0f * dt );

        if ( funds_show_ < funds_.get() )
        {
            funds_show_ = std::min( funds_.get(), funds_show_ + ( 10.0f * dt ) );
            funds_gfx_.setNumber( funds_show_ );
        }

        for ( auto & popup : score_pop_ups_ )
        {
            popup.update( dt, level );
        }

        int i = 0;
        while ( i < score_pop_ups_.size() )
        {
            if ( score_pop_ups_[ i ].isDone() )
            {
                score_pop_ups_.erase( score_pop_ups_.begin() + i );
            }
            else
            {
                ++i;
            }
        }
    };

    void LevelInventory::addFunds( float amount )
    {
        funds_ += amount;
    };

    void LevelInventory::hurt()
    {
        if ( hp_ > 0 )
        {
            --hp_;
            hp_gfx_[ hp_ ].setSrcX( 8.0f );
        }
    };

    void LevelInventory::showPtsPopUp( const std::string & text, const Game & game )
    {
        for ( auto & popup : score_pop_ups_ )
        {
            popup.deactivate();
        }
        score_pop_ups_.emplace_back( text, game );
    };

    bool LevelInventory::isDead() const { return hp_ == 0; };

    ScorePopUp::ScorePopUp( const std::string & text, const Game & game )
    :
        state_ ( State::FADING_IN ),
        gfx_ ( 0 ),
        opacity_ ( 0.0f ),
        y_ ( 0.0f ),
        accy_ ( 0.0f ),
        vy_ ( 0.0f ),
        timer_ ( 0.0f )
    {
        ArgList text_args = 
        {
            { "text", text },
            { "scrollx", 0.0f },
            { "scrolly", 0.0f },
            { "width", 16.0f * 6.0f },
            { "type", "gradient" },
            { "dir", Dir::XY::DOWN },
            { "color1", 64 },
            { "color2", 16 },
            { "shadow", 0.25f },
            { "opacity", 0.0f }
        };
        gfx_ = game.render().addText( text_args );
    };

    void ScorePopUp::update( float dt, const Level & level )
    {
        switch ( state_ )
        {
            case ( State::FADING_IN ):
            {
                if ( opacity_ < 1.0f )
                {
                    opacity_ += 0.05f * dt;
                }
                accy_ = ( y_ > -8.0f ) ? -0.25f : 0.0f;
                vy_ += accy_;
                if ( vy_ < -2.0f )
                {
                    vy_ = -2.0f;
                }

                if ( accy_ == 0.0f )
                {
                    vy_ /= 1.0f + 0.5 * dt;
                }

                y_ += vy_;

                gfx_.setOpacity( opacity_ );

                if ( opacity_ >= 1.0f && y_ <= -8.0f && vy_ < 0.0001f )
                {
                    state_ = State::SHOWING;
                }
            }
            break;
            case ( State::SHOWING ):
            {
                if ( timer_ >= 30.0f )
                {
                    state_ = State::FADING_OUT;
                }
                else
                {
                    timer_ += dt;
                }
            }
            break;
            case ( State::FADING_OUT ):
            {
                if ( opacity_ > 0.0f )
                {
                    opacity_ -= 0.05f * dt;
                }
                gfx_.setOpacity( opacity_ );

                if ( opacity_ <= 0.0f )
                {
                    state_ = State::DONE;
                }
            }
            break;
        }
        gfx_.setXOffset( level.getPos().x );
        gfx_.setYOffset( level.getPos().y + y_ );
    }
}