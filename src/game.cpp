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
    };
}