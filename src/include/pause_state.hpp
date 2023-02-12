#ifndef PAUSE_H
#define PAUSE_H

#include <array>
#include "game_state.hpp"

#include <functional>

namespace BSL
{
    class Game;

    template <typename T, typename U>
    T ArrayMap( const T & list, const U & action )
    {
        T out = {};
        for ( unsigned int i = 0; i < list.size(); ++i )
        {
            out[ i ] = action( list[ i ] );
        }
        return out;
    };

    template<typename T, typename U, typename V>
    T ArrayReduce( const U & list, const V & action, T initial = 0 )
    {
        T value = initial;
        for ( unsigned int i = 0; i < list.size(); ++i )
        {
            value = action( value, list[ i ], i );
        }
        return value;
    };

    class PauseState : public GameState
    {
        public:
            PauseState();
            void update( Game & game, float dt ) override;
            void init( Game & game ) override;

        private:
            static constexpr int NUM_O_OPTIONS = 2;
            typedef std::array<const char *, NUM_O_OPTIONS> OptionList;
            static constexpr OptionList OPTIONS =
            {
                "Continue Level",
                "Exit Level"
            };

            SpriteGraphic boxgfx_;
            std::array<unsigned int, NUM_O_OPTIONS> textgfx_;
            float opacity_;
    };
}

#endif // PAUSE_H