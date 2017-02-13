

// Name
//===================================
//
// Object
//


// DEPENDENCIES
//===================================

    #include "collision.h"
    #include "object.h"
    #include "unit.h"
    #include <iostream>


// STATIC PROPERTIES
//===================================

// METHODS
//===================================

    Object::Object
    (
        int x,
        int y,
        int width,
        int height
    )
    :
        original_hit_box_ ( { Unit::PixelsToSubPixels( x ), Unit::PixelsToSubPixels( y ), Unit::PixelsToSubPixels( width ), Unit::PixelsToSubPixels( height ) } ),
        hit_box_ ( { Unit::PixelsToSubPixels( x ), Unit::PixelsToSubPixels( y ), Unit::PixelsToSubPixels( width ), Unit::PixelsToSubPixels( height ) } )
    {};

    Object::~Object() {};

    void Object::changeX( int new_x_pixels )
    {
        hit_box_.x = new_x_pixels;
    };

    void Object::changeY( int new_y_pixels )
    {
        hit_box_.y = new_y_pixels;
    };

    void Object::addToX( int addition_x_pixels )
    {
        hit_box_.x += addition_x_pixels;
    };

    void Object::addToY( int addition_y_pixels )
    {
        hit_box_.y += addition_y_pixels;
    };

    int Object::heightPixels() const
    {
        return Unit::SubPixelsToPixels( hit_box_.h );
    };

    int Object::widthPixels() const
    {
        return Unit::SubPixelsToPixels( hit_box_.w );
    };

    int Object::heightSubPixels() const
    {
        return hit_box_.h;
    };

    int Object::widthSubPixels() const
    {
        return hit_box_.w;
    };

    int Object::xPixels() const
    {
        return Unit::SubPixelsToPixels( hit_box_.x );
    };

    int Object::yPixels() const
    {
        return Unit::SubPixelsToPixels( hit_box_.y );
    };

    int Object::xSubPixels() const
    {
        return hit_box_.x;
    };

    int Object::ySubPixels() const
    {
        return hit_box_.y;
    };

    int Object::halfWidthSubPixels() const
    {
        return round( hit_box_.w / 2 );
    };

    int Object::halfHeightSubPixels() const
    {
        return round( hit_box_.h / 2 );
    };

    int Object::centerXSubPixels() const
    {
        return xSubPixels() + halfWidthSubPixels();
    };

    int Object::centerYSubPixels() const
    {
        return ySubPixels() + halfHeightSubPixels();
    };

    int Object::halfWidthPixels() const
    {
        return round( widthPixels() / 2 );
    };

    int Object::halfHeightPixels() const
    {
        return round( heightPixels() / 2 );
    };

    int Object::centerXPixels() const
    {
        return xPixels() + halfWidthPixels();
    };

    int Object::centerYPixels() const
    {
        return yPixels() + halfHeightPixels();
    };

    int Object::leftSubPixels() const
    {
        return hit_box_.x;
    };

    int Object::rightSubPixels() const
    {
        return hit_box_.x + hit_box_.w;
    };

    int Object::topSubPixels() const
    {
        return hit_box_.y;
    };

    int Object::bottomSubPixels() const
    {
        return hit_box_.y + hit_box_.h;
    };

    int Object::prevLeftSubPixels() const
    {
        return xPrevSubPixels();
    };

    int Object::prevRightSubPixels() const
    {
        return xPrevSubPixels() + hit_box_.w;
    };

    int Object::prevTopSubPixels() const
    {
        return yPrevSubPixels();
    };

    int Object::prevBottomSubPixels() const
    {
        return yPrevSubPixels() + hit_box_.h;
    };

    const Collision Object::collide( Object* other ) const
    {
        int overlap_x_left   = 0;
        int overlap_x_right  = 0;
        int overlap_y_top    = 0;
        int overlap_y_bottom = 0;
/*
        double dx = centerXSubPixels() - other->centerXSubPixels();
        double dy = centerYSubPixels() - other->centerYSubPixels();

        double hx = ( other->halfWidthSubPixels() ) + ( halfWidthSubPixels() );
        double hy = ( other->halfHeightSubPixels() ) + ( halfHeightSubPixels() );

        int overlap_x = hx - abs(dx);
        int overlap_y = hy - abs(dy);

        bool theres_a_collision = abs(dx) < hx && abs(dy) < hy;

        if ( theres_a_collision )
        {
            if ( dx > 0 )
              overlap_x_left = overlap_x;
            else
              overlap_x_right = overlap_x;

            if ( dy > 0 )
              overlap_y_top = overlap_y;
            else
              overlap_y_bottom = overlap_y;
        }
*/

        //sdl2::SDLRect their_hit_box = other->hitBox();
        if
        (
            leftSubPixels() + 4000 < other->rightSubPixels() &&
            rightSubPixels() - 4000 > other->leftSubPixels() &&
            topSubPixels() < other->bottomSubPixels() &&
            bottomSubPixels() > other->topSubPixels()
        )
        {
            if ( centerYSubPixels() > other->centerYSubPixels() )
                overlap_y_top = other->bottomSubPixels() - topSubPixels();
            else
                overlap_y_bottom = bottomSubPixels() - other->topSubPixels();
        }

        if
        (
            leftSubPixels() + 1000 < other->rightSubPixels() &&
            rightSubPixels() - 3000 > other->leftSubPixels() &&
            topSubPixels() < other->bottomSubPixels() &&
            bottomSubPixels() > other->topSubPixels()
        )
        {
            if ( centerYSubPixels() < other->centerYSubPixels() )
                overlap_y_bottom = bottomSubPixels() - other->topSubPixels();
        }

        if
        (
            leftSubPixels() < other->rightSubPixels() &&
            rightSubPixels() > other->leftSubPixels() &&
            topSubPixels() < other->bottomSubPixels() &&
            bottomSubPixels() - 4000 > other->topSubPixels()
        )
        {
            if ( centerXSubPixels() < other->centerXSubPixels() )
                overlap_x_right = other->rightSubPixels() - leftSubPixels();
            else if ( centerXSubPixels() > other->centerXSubPixels() )
                overlap_x_left = rightSubPixels() - other->leftSubPixels();
        }

/*
        const int left_bottom_point_x = hit_box_.x + 2000;
        const int right_bottom_point_x = hit_box_.x + hit_box_.w - 2000;
        const int center_bottom_point_x = hit_box_.x + halfHeightSubPixels();
        const int bottom_points_y = hit_box_.y + hit_box_.h;

        const int target_downward_left_bound_x = other->hit_box_.x;
        const int target_downward_right_bound_x = other->hit_box_.x + other->hit_box_.w;
        const int target_downward_top_bound_y = other->hit_box_.y;
        const int target_downward_bottom_bound_y = other->hit_box_.y + other->halfHeightSubPixels();//other->hit_box_.h;

        const int top_points_y = hit_box_.y;
        const int target_upward_top_bound_y = other->hit_box_.y + other->halfHeightSubPixels();
        const int target_upward_bottom_bound_y = other->hit_box_.y + other->hit_box_.h;

        const int left_point_x = hit_box_.x;
        const int left_point_y_top = hit_box_.y;
        const int left_point_y_center = hit_box_.y + halfHeightSubPixels();
        const int left_point_y_bottom = hit_box_.y + hit_box_.h - 8000;

        const int left_target_bound_x_left = other->hit_box_.x;
        const int left_target_bound_x_right = other->hit_box_.x + other->hit_box_.w;
        const int left_target_bound_y_top = other->hit_box_.y;
        const int left_target_bound_y_bottom = other->hit_box_.y + other->hit_box_.h;

        const int right_point_x = hit_box_.x + hit_box_.w;

        const int right_target_bound_x_left = other->hit_box_.x;
        const int right_target_bound_x_right = other->hit_box_.x + other->halfWidthSubPixels();

        if
        (
            ( left_bottom_point_x > target_downward_left_bound_x && left_bottom_point_x < target_downward_right_bound_x ) ||
            ( right_bottom_point_x > target_downward_left_bound_x && right_bottom_point_x < target_downward_right_bound_x ) ||
            ( center_bottom_point_x > target_downward_left_bound_x && center_bottom_point_x < target_downward_right_bound_x )
        )
        {
            if
            (
                bottom_points_y > target_downward_top_bound_y &&
                bottom_points_y < target_downward_bottom_bound_y
            )
            {
                overlap_y_bottom = bottom_points_y - target_downward_top_bound_y;
            }
        }

        if
        (
            ( left_bottom_point_x > target_downward_left_bound_x && left_bottom_point_x < target_downward_right_bound_x ) ||
            ( right_bottom_point_x > target_downward_left_bound_x && right_bottom_point_x < target_downward_right_bound_x ) ||
            ( center_bottom_point_x > target_downward_left_bound_x && center_bottom_point_x < target_downward_right_bound_x )
        )
        {
            if
            (
                top_points_y < target_upward_bottom_bound_y &&
                top_points_y > target_upward_top_bound_y
            )
            {
                overlap_y_top = target_upward_bottom_bound_y - top_points_y;
            }
        }

        if
        (
            ( left_point_y_top > left_target_bound_y_top && left_point_y_top < left_target_bound_y_bottom ) ||
            ( left_point_y_bottom > left_target_bound_y_top && left_point_y_bottom < left_target_bound_y_bottom ) ||
            ( left_point_y_center > left_target_bound_y_top && left_point_y_center < left_target_bound_y_bottom )
        )
        {
            if
            (
                left_point_x > left_target_bound_x_left &&
                left_point_x < left_target_bound_x_right
            )
            {
                overlap_x_left = left_target_bound_x_right - left_point_x;
            }
        }

        if
        (
            ( left_point_y_top > left_target_bound_y_top && left_point_y_top < left_target_bound_y_bottom ) ||
            ( left_point_y_bottom > left_target_bound_y_top && left_point_y_bottom < left_target_bound_y_bottom ) ||
            ( left_point_y_center > left_target_bound_y_top && left_point_y_center < left_target_bound_y_bottom )
        )
        {
            if
            (
                right_point_x > right_target_bound_x_left &&
                right_point_x < right_target_bound_x_right
            )
            {
                overlap_x_right = right_point_x - right_target_bound_x_left;
            }
        }
        */



        return Collision( overlap_x_left, overlap_x_right, overlap_y_top, overlap_y_bottom );
    };

    int Object::xPrevSubPixels() const
    {
        return x_prev_;
    };

    int Object::yPrevSubPixels() const
    {
        return y_prev_;
    };

    int Object::xPrevPixels() const
    {
        return Unit::SubPixelsToPixels( x_prev_ );
    };

    int Object::yPrevPixels() const
    {
        return Unit::SubPixelsToPixels( y_prev_ );
    };

    int Object::rightPixels() const
    {
        return Unit::SubPixelsToPixels( rightSubPixels() );
    };

    int Object::bottomPixels() const
    {
        return Unit::SubPixelsToPixels( bottomSubPixels() );
    };

    int Object::prevRightPixels() const
    {
        return Unit::SubPixelsToPixels( prevRightSubPixels() );
    };

    int Object::prevBottomPixels() const
    {
        return Unit::SubPixelsToPixels( prevBottomSubPixels() );
    };

    const sdl2::SDLRect& Object::hitBox()
    {
        return hit_box_;
    };

    int Object::originalXSubPixels() const
    {
        return original_hit_box_.x;
    };

    int Object::originalYSubPixels() const
    {
        return original_hit_box_.y;
    };

    const sdl2::SDLRect& Object::originalHitBox()
    {
        return original_hit_box_;
    };
