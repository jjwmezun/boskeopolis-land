#pragma once

#include "direction.hpp"
#include "mezun_sdl2.hpp"

class CircleMovement
{
    public:
        constexpr CircleMovement( sdl2::SDLRect position, int start_x, int start_y, int radius, double speed, double starting_angle = 0.0 )
        :
            position_ ( position ),
            speed_ ( speed ),
            angle_( starting_angle ),
            start_x_ ( start_x ),
            start_y_ ( start_y ),
            radius_ ( radius )
        {};

        constexpr void update()
        {
            updatePosition();
            updateAngleClockwise();
        };

        constexpr void updatePosition()
        {
            position_.x = start_x_ + ( position_.w  / 2 ) + ( std::cos( angle_ ) * radius_ );
            position_.y = start_y_ + ( position_.h / 2 ) + ( std::sin( angle_ ) * radius_ );
        };

        constexpr void updateAngleClockwise()
        {
            angle_ += speed_;
        }

        constexpr void updateAngleCounterClockwise()
        {
            angle_ -= speed_;
        }

        constexpr const sdl2::SDLRect& getPosition() const
        {
            return position_;
        }

        constexpr double getAngle() const
        {
            return angle_;
        };

        sdl2::SDLRect position_;
        const double speed_;
        double angle_;
        const int start_x_;
        const int start_y_;
        const int radius_;
};
