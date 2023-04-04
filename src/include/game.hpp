#ifndef GAME_H
#define GAME_H

#include "game_state.hpp"
#include "renderer.hpp"
#include <vector>
#include <memory>
#include "tileset.hpp"

namespace BSL
{
    class Game
    {
        public:        
            Game( unsigned int max_states );
            void changeState( std::unique_ptr<GameState> && state );
            void pushState( std::unique_ptr<GameState> && state );
            void popState();

            void update( float dt );
            const Renderer & render() const;
            const Tileset & getObjects() const;

        private:
            std::vector<std::unique_ptr<GameState>> states_;
            Renderer renderer_;
            Tileset objects_;
            unsigned int max_states_;
            unsigned int current_state_;
            unsigned int number_of_states_;
            CounterGraphic fps_;
    };
}

#endif // GAME_H