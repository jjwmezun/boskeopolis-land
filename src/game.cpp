#include "game.hpp"

namespace BSL
{
    Game::Game( unsigned int max_states )
    :
        max_states_ ( max_states ),
        current_state_ ( 0 ),
        number_of_states_ ( 0 )
    {
        states_.reserve( max_states );
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

    void Game::changeState( std::unique_ptr<GameState> && state )
    {
        states_.clear();
        NasrClearGraphics();
        NasrClearTextures();

        states_.push_back( std::unique_ptr<GameState>( state.release() ) );
        renderer_.current_state_ = current_state_ = 0;
        number_of_states_ = 1;
        states_[ current_state_ ]->init( *this );

        fps_ = renderer_.addCounter
        (
            0.0f,
            4,
            0,
            128,
            16.0f,
            16.0f,
            {
                { "shadow", 0.5f }
            }
        );
    };

    void Game::pushState( std::unique_ptr<GameState> && state )
    {
        states_.push_back( std::unique_ptr<GameState>( state.release() ) );
        renderer_.current_state_ = ++current_state_;
        ++number_of_states_;
        states_[ current_state_ ]->init( *this );
    };

    void Game::popState()
    {
        states_.pop_back();
        NasrGraphicsClearState( current_state_ );
        renderer_.current_state_ = --current_state_;
        --number_of_states_;
    };
}