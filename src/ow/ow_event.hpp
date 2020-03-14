#pragma once

#include "dpoint.hpp"
#include "ow_event_tile.hpp"
#include "timer_repeat_t2.hpp"
#include <vector>

class OWEvent final
{
    public:
        enum class MessageBack
        {
            __NULL = 0,
            SHOW_NEXT_LEVEL = 1,
            EVENT_OVER = 2,
            BG_TILES_1_CHANGED = 4,
            BG_TILES_2_CHANGED = 8,
            FG_TILES_1_CHANGED = 16,
            FG_TILES_2_CHANGED = 32
        };

        OWEvent();
        void init( int level, int map_width, bool is_secret );
        MessageBack update( std::vector<int>* bg_tiles, std::vector<int>* fg_tiles );
        const DPoint& getTargetPosition() const;
        void changeAllTiles( std::vector<int>* bg_tiles, std::vector<int>* fg_tiles );
        int getNextLevel() const;

        static constexpr bool testBG1Change( MessageBack message )
        {
            return ( ( int )( message ) >> 3 ) & 1;
        };

        static constexpr bool testBG2Change( MessageBack message )
        {
            return ( ( int )( message ) >> 4 ) & 1;
        };

        static constexpr bool testFG1Change( MessageBack message )
        {
            return ( ( int )( message ) >> 5 ) & 1;
        };

        static constexpr bool testFG2Change( MessageBack message )
        {
            return ( ( int )( message ) >> 6 ) & 1;
        };

    private:
        MessageBack changeTiles( std::vector<int>* bg_tiles, std::vector<int>* fg_tiles );

        int event_tile_speed_;
        int timer_;
        int current_change_;
        int next_level_;
        std::vector<std::vector<OWEventTile>> changes_;
        DPoint target_position_;
};

constexpr OWEvent::MessageBack operator|( OWEvent::MessageBack a, OWEvent::MessageBack b )
{
    return ( OWEvent::MessageBack )( ( int )( a ) | ( int )( b ) );
};