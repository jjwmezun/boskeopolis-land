#pragma once

#include "counter_t.hpp"
#include "mezun_sdl2.hpp"
#include <string>
#include "timer_repeat_t2.hpp"
#include "unit.hpp"
#include <vector>

class OWLevelTileGraphics final
{
    public:
        OWLevelTileGraphics();
        void add( sdl2::SDLRect position, int level, bool reveal );
        void update( const sdl2::SDLRect& camera );
        void render() const;
        void showTile( const sdl2::SDLRect& camera, int level );

    private:
        static constexpr int NUMBER_OF_ANIMATION_FRAMES = 12;
        static constexpr int ANIMATION_FRAMES[ NUMBER_OF_ANIMATION_FRAMES ] =
        {
            0, 1, 2, 3, 4, 5, 5, 4, 3, 2, 1, 0
        };

        bool testOnCamera( const sdl2::SDLRect& position, const sdl2::SDLRect& camera ) const;
        void refreshGraphics( const sdl2::SDLRect& camera );

        int prev_camera_x_;
        int prev_camera_y_;
        TimerRepeatT2<Unit::DEFAULT_ANIMATION_SPEED> animation_timer_;
        CounterT<NUMBER_OF_ANIMATION_FRAMES - 1, 0, 0, true> animation_frame_;
        std::string tileset_;
        std::vector<sdl2::SDLRect> destinations_;
        sdl2::SDLRect source_;
        std::vector<bool> reveal_;
        std::vector<sdl2::SDLRect> positions_;
};