#include "game.hpp"

namespace BSL
{
    Game::Game( unsigned int max_states )
    :
        max_states_ ( max_states ),
        current_state_ ( 0 ),
        number_of_states_ ( 0 ),
        objects_ ( "objects" )
    {
        states_.reserve( max_states );
        objects_.init();
    };

    void Game::update( float dt )
    {
        states_[ current_state_ ]->update( *this, dt );
        fps_.setNumber( 60.0f / dt );
    };

    const Renderer & Game::render() const
    {
        return renderer_;
    };

    const Tileset & Game::getObjects() const
    {
        return objects_;
    };

    void Game::changeState( std::unique_ptr<GameState> && state )
    {
        states_.clear();
        NasrClearGraphics();
        NasrClearTextures();

        states_.push_back( std::unique_ptr<GameState>( state.release() ) );
        renderer_.current_state_ = current_state_ = 0;
        number_of_states_ = 1;
        states_[ current_state_ ]->init( *this );

        const ArgList fps_args =
        {
            { "num", 0.0f },
            { "maxdigits", 4 },
            { "color", 128 },
            { "x", 16.0f },
            { "y", 16.0f },
            { "shadow", 0.5f },
            { "layer", Layer::SUPER }
        };
        fps_ = renderer_.addCounter( fps_args );
    };

    void Game::pushState( std::unique_ptr<GameState> && state )
    {
        states_.push_back( std::unique_ptr<GameState>( state.release() ) );
        renderer_.current_state_ = ++current_state_;
        ++number_of_states_;
        states_[ current_state_ ]->init( *this );
    };

    void Game::popState( int message )
    {
        states_.pop_back();
        NasrGraphicsClearState( current_state_ );
        renderer_.current_state_ = --current_state_;
        --number_of_states_;
        if ( current_state_ >= 0 )
        {
            states_[ current_state_ ]->backToState( *this, message );
        }
    };

    void Game::clearGraphics()
    {
        NasrClearGraphics();
        NasrClearTextures();
        const ArgList fps_args =
        {
            { "num", 0.0f },
            { "maxdigits", 4 },
            { "color", 128 },
            { "x", 16.0f },
            { "y", 16.0f },
            { "shadow", 0.5f },
            { "layer", Layer::SUPER }
        };
        fps_ = renderer_.addCounter( fps_args );
    };
}