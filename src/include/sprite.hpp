#ifndef SPRITE_H
#define SPRITE_H

#include "counter.hpp"
#include "dir.hpp"
#include "rect.hpp"
#include "timer.hpp"

namespace BSL
{
    class Controller;
    class Game;
    class Level;

    class Sprite
    {
        public:
            Sprite();
            void init( Game & game );
            void update( float dt, const Controller & controller, Level & level );
            bool isOnGround() const;
            inline const Rect & getPos() const { return pos_; };

        private:
            unsigned int graphic_;
            Rect pos_;
            float accx_;
            float vx_;
            float prevx_;
            float accy_;
            float vy_;
            float prevy_;
            bool is_jumping_;
            bool jump_lock_;
            bool on_ground_;
            float jump_padding_;
            Dir::X dir_x_;
            bool is_moving_;
            Counter<int, 3, 0, true> walk_frame_;
            float animation_timer_;
    };
}

#endif // SPRITE_H