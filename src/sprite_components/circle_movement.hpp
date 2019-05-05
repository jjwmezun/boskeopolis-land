#pragma once

#include "direction.hpp"
#include "mezun_sdl2.hpp"

class CircleMovement
{
    public:
        constexpr CircleMovement( sdl2::SDLRect position, int start_x, int start_y, int radius, double speed )
        :
            position_ ( position ),
            speed_ ( speed ),
            angle_( 0.0 ),
            start_x_ ( start_x ),
            start_y_ ( start_y ),
            radius_ ( radius )
        {};

        constexpr void update()
        {
            position_.x = start_x_ + ( position_.w  / 2 ) + ( std::cos( angle_ ) * radius_ );
            position_.y = start_y_ + ( position_.h / 2 ) + ( std::sin( angle_ ) * radius_ );
            angle_ += speed_;
        };

        constexpr const sdl2::SDLRect& getPosition() const
        {
            return position_;
        }

        constexpr double getAngle() const
        {
            return angle_;
        };

    private:
        sdl2::SDLRect position_;
        const double speed_;
        double angle_;
        const int start_x_;
        const int start_y_;
        const int radius_;
};
