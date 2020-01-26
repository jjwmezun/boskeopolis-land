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
            EVENT_OVER
        };

        OWEvent();
        void init( int level, int map_width );
        MessageBack update( std::vector<int>& bg_tiles, std::vector<int>& fg_tiles );
        const DPoint& getTargetPosition() const;
        void changeAllTiles( std::vector<int>& bg_tiles, std::vector<int>& fg_tiles );

    private:
        OWEvent::MessageBack changeTiles( std::vector<int>& bg_tiles, std::vector<int>& fg_tiles );

        TimerRepeatT2<16> timer_;
        int current_change_;
        std::vector<std::vector<OWEventTile>> changes_;
        DPoint target_position_;
};