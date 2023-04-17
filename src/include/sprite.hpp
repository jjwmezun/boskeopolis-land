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
            friend class SpriteSystem;

            enum class Type
            {
                AUTUMN,
                CRAB,
                BADAPPLE,
                TRUCK,
                SCALE_LIFT
            };

            enum class Attribute
            {
                PROTAG,
                ENEMY,
                BOPPABLE
            };

            enum class LivingState
            {
                ALIVE,
                DYING,
                DEAD
            };

            Sprite( Type type, Rect pos, std::set<Attribute> atts = {}, ArgList args = {} );
            void init( Game & game );
            void update( float dt, const Controller & controller, Level & level, Game & game, std::vector<Sprite> & sprites );
            bool isOnGround() const;
            inline const Rect & getPos() const { return pos_; };
            bool hasAttribute( Attribute att ) const;

            struct Collision
            {
                float top;
                float bottom;
                float left;
                float right;
            };

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
            bool on_ground_;
            Dir::X dir_x_;
            bool is_moving_;
            float animation_timer_;
            float start_speed_;
            float max_speed_;
            Collision collision_;
            LivingState living_;
            bool block_interact_;
            union
            {
                struct
                {
                    float jump_padding;
                    float invincibility_;
                    uint_fast8_t walk_frame;
                    bool jump_lock;
                } autumn;
                struct
                {
                    Rect left;
                    Rect right;
                    float broken_timer;
                    float wheel_rotation;
                    SpriteGraphic left_gfx;
                    SpriteGraphic right_gfx;
                    SpriteGraphic top_bar_gfx;
                    SpriteGraphic left_bar_gfx;
                    SpriteGraphic right_bar_gfx;
                    SpriteGraphic left_wheel_gfx;
                    SpriteGraphic right_wheel_gfx;
                } scale_lift;
            } misc_;
            float max_jump_;

            void updatePositionGraphics();
            void goLeft();
            void goRight();
            void moveInDirectionX();
            void handleGeneralCollision( float dt, Level & level, Game & game );
            void rotateOnDirectionChange( float dt );
            void flipXOnCollision();
            bool isAutumnGoingFast() const;
            void autumnLanding();
    };
}

#endif // SPRITE_H