#pragma once

#include "dpoint.hpp"
#include "ow_event_tile.hpp"
#include "timer_repeat_t2.hpp"
#include <vector>

class OWEvent
{
    public:
        enum class MessageBack
        {
            __NULL,
            BOTH_TILES_CHANGED,
            BG_TILES_CHANGED,
            FG_TILES_CHANGED,
            SHOW_NEXT_LEVEL,
            EVENT_OVER
        };

        OWEvent();
        void init( int level, int map_width, bool is_secret );
        MessageBack update( std::vector<int>& bg_tiles, std::vector<int>& fg_tiles );
        const DPoint& getTargetPosition() const;
        void changeAllTiles( std::vector<int>& bg_tiles, std::vector<int>& fg_tiles );

    private:
        OWEvent::MessageBack changeTiles( std::vector<int>& bg_tiles, std::vector<int>& fg_tiles );

        int event_tile_speed_;
        int timer_;
        int current_change_;
        std::vector<std::vector<OWEventTile>> changes_;
        DPoint target_position_;
};