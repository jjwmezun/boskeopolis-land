#pragma once

#include "direction.hpp"
#include "sprite_component.hpp"

class SpriteComponentCircle final : public SpriteComponent
{

    public:
        SpriteComponentCircle( int radius = 50, int speed = 50, double start_angle = 0.0 );
        ~SpriteComponentCircle();
        void update( Sprite& sprite, LevelState& state ) override;

    private:
        const double radius_;
        const double speed_;
        double angle_;
};