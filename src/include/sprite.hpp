#ifndef SPRITE_H
#define SPRITE_H

#include "controller.hpp"

namespace BSL
{
    class Sprite
    {
        public:
            Sprite();
            void update( float dt, Controller & controller );
            bool isOnGround() const;

        private:
            unsigned int graphic_;
            float accx_;
            float vx_;
            float x_;
            float prevx_;
            float accy_;
            float vy_;
            float y_;
            float prevy_;
            float jump_start_;
            bool is_jumping_;
            bool jump_lock_;
    };
}

#endif // SPRITE_H