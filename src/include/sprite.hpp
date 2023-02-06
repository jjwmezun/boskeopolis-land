#ifndef SPRITE_H
#define SPRITE_H

#include "rect.hpp"

namespace BSL
{
    class Controller;
    class Map;

    class Sprite
    {
        public:
            Sprite();
            void update( float dt, const Controller & controller, Map & map );
            bool isOnGround() const;

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
    };
}

#endif // SPRITE_H