#ifndef SPRITE_H
#define SPRITE_H

#include "arg.hpp"
#include "counter.hpp"
#include "dir.hpp"
#include "rect.hpp"
#include "renderer.hpp"
#include "timer.hpp"
#include <set>

namespace BSL
{
    class Controller;
    class Game;
    class Level;

    class Sprite
    {
        public:
            enum class Type
            {
                AUTUMN,
                CRAB
            };

            enum class Attribute
            {
                PROTAG,
                ENEMY
            };

            Sprite( Type type, Rect pos, ArgList args = {} );
            void init( Game & game );
            void update( float dt, const Controller & controller, Level & level, Game & game );
            void interact( Sprite & other, Level & level, Game & game );
            bool isOnGround() const;
            inline const Rect & getPos() const { return pos_; };
            bool hasAttribute( Attribute att ) const;

        private:
            Type type_;
            std::set<Attribute> attributes_;
            SpriteGraphic graphic_;
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
            float start_speed_;
            float max_speed_;

            void updatePositionGraphics();
            void goLeft();
            void goRight();
    };
}

#endif // SPRITE_H