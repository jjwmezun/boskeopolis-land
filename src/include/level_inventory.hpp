#ifndef LEVEL_INVENTORY_H
#define LEVEL_INVENTORY_H

#include "clock.hpp"
#include <cstdint>
#include "ranged_float.hpp"
#include <vector>

namespace BSL
{
    class Game;
    class Level;

    class ScorePopUp
    {
        public:
            enum class State
            {
                FADING_IN,
                SHOWING,
                FADING_OUT,
                DONE
            };
            ScorePopUp( const std::string & text, const Game & game );
            void update( float dt, const Level & level );
            inline void deactivate()
            {
                state_ = ScorePopUp::State::FADING_OUT;
            };
            inline bool isDone() const
            {
                return state_ == ScorePopUp::State::DONE;
            };

        private:
            State state_;
            TextGraphic gfx_;
            float opacity_;
            float y_;
            float accy_;
            float vy_;
            float timer_;
    };

    class LevelInventory
    {
        public:
            inline LevelInventory()
            :
                clock_ ( 0 ),
                funds_gfx_ ( 0 ),
                funds_ ( 99999.0f, 0.0f, 0.0f ),
                funds_show_ ( 0.0f ),
                hp_ ( 2 )
            {};
            void init( Game & game );
            void update( float dt, const Level & level );
            void addFunds( float amount );
            void hurt();
            void showPtsPopUp( const std::string & text, const Game & game );
            bool isDead() const;

        private:
            Clock clock_;
            std::vector<ScorePopUp> score_pop_ups_;
            std::vector<SpriteGraphic> hp_gfx_;
            CounterGraphic funds_gfx_;
            RangedFloat funds_;
            float funds_show_;
            uint_fast8_t hp_;
    };
}

#endif // LEVEL_INVENTORY_H