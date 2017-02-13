

// Name
//===================================
//
// Warp
//

#ifndef WARP_H
#define WARP_H


// FORWARD DECLARATIONS
//===================================


// DEPENDENCIES
//===================================

    #include "direction.h"
    #include "unit.h"


// CLASS
//===================================

    class Warp
    {
        public:
            constexpr Warp
            (
                int map_num,
                int entrance_x,
                int entrance_y,
                int warp_location_blocks_left,
                int warp_location_blocks_right,
                int warp_location_blocks_top,
                int warp_location_blocks_bottom,
                int camera_x = -1,
                int camera_y = -1
            )
            :
                map_num_ ( map_num ),
                entrance_x_ ( entrance_x ),
                entrance_y_ ( entrance_y ),
                warp_location_blocks_left_ ( warp_location_blocks_left ),
                warp_location_blocks_right_ ( warp_location_blocks_right ),
                warp_location_blocks_top_ ( warp_location_blocks_top ),
                warp_location_blocks_bottom_ ( warp_location_blocks_bottom ),
                camera_x_ ( camera_x ),
                camera_y_ ( camera_y )
            {};

            constexpr int mapNum() const
            {
                return map_num_;
            };

            constexpr int entranceX() const
            {
                return entrance_x_;
            };

            constexpr int entranceY() const
            {
                return entrance_y_;
            };

            constexpr int cameraX() const
            {
                return camera_x_;
            };

            constexpr int cameraY() const
            {
                return camera_y_;
            };

            constexpr bool inInterval( int x_sub_pixels, int y_sub_pixels )
            {
                return
                (
                    Unit::SubPixelsToBlocks( x_sub_pixels ) >= warp_location_blocks_left_  &&
                    Unit::SubPixelsToBlocks( x_sub_pixels ) <= warp_location_blocks_right_ &&
                    Unit::SubPixelsToBlocks( y_sub_pixels ) >= warp_location_blocks_top_   &&
                    Unit::SubPixelsToBlocks( y_sub_pixels ) <= warp_location_blocks_bottom_
                );
            };


        private:
            const int map_num_;
            const int entrance_x_;
            const int entrance_y_;
            const int warp_location_blocks_left_;
            const int warp_location_blocks_right_;
            const int warp_location_blocks_top_;
            const int warp_location_blocks_bottom_;
            const int camera_x_;
            const int camera_y_;
    };

#endif // WARP_H
