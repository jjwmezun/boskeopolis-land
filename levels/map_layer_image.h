


// Name
//===================================
//
// MapLayerImage
//

#ifndef MAP_LAYER_IMAGE_H
#define MAP_LAYER_IMAGE_H


// FORWARD DECLARATIONS
//===================================


// DEPENDENCIES
//===================================

    #include "counter.h"
    #include "graphics.h"
    #include "map_layer.h"
    #include "timers/timer_repeat.h"


// CLASS
//===================================

    class MapLayerImage : public MapLayer
    {
        public:
            MapLayerImage
            (
                Graphics::SpriteSheet texture,
                int width,
                int height,
                int offset_x = 0,
                int offset_y = 0,
                double scroll_speed_x = 1,
                double scroll_speed_y = 1,
                int num_o_frames = 1,
                bool repeat_x = true,
                bool repeat_y = true,
                int move_speed_x = 0,
                int move_speed_y = 0,
                int animation_speed = 1,
				bool flip = false
            );
            void update( EventSystem& events ) override;
            void render( Graphics& graphics, Camera& camera ) const override;

        private:
            const Graphics::SpriteSheet texture_;
            sdl2::SDLRect source_;
            const int offset_x_;
            const int offset_y_;
            const double scroll_speed_x_;
            const double scroll_speed_y_;
            const bool repeat_x_;
            const bool repeat_y_;
            const int move_speed_x_;
            const int move_speed_y_;
            const int num_o_frames_;
            const int animation_speed_;
            int movement_position_x_;
            int movement_position_y_;
            Counter current_frame_;
            TimerRepeat animation_timer_;
			Direction::Vertical frame_dir_;

            void renderY( Graphics& graphics, Camera& camera ) const;
            void renderX( Graphics& graphics, Camera& camera, sdl2::SDLRect& dest ) const;
            bool onscreen( const sdl2::SDLRect& r, Camera& camera ) const;
    };

#endif // MAP_LAYER_IMAGE_H
