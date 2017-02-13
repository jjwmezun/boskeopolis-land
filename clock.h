




// Name
//===================================
//
// Clock
//

#ifndef CLOCK_H
#define CLOCK_H


// FORWARD DECLARATIONS
//===================================

    class Graphics;


// DEPENDENCIES
//===================================

    #include <string>
    #include "text.h"
    #include "timers/timer_repeat.h"
    #include "unit.h"


// CLASS
//===================================

    class Clock
    {
        public:
            Clock( int start_time = 0, Direction::Vertical direction = DEFAULT_DIRECTION, int limit = DEFAULT_LIMIT );
            ~Clock();

            void update();
            void reset( Direction::Vertical direction = DEFAULT_DIRECTION, int limit = DEFAULT_LIMIT );
            void render( Graphics& graphics, int x, int y, Camera* camera = nullptr, Text::FontShade shade = Text::FontShade::BLACK );
            static void renderTime( Graphics& graphics, int x, int y, int total_seconds, Camera* camera = nullptr, Text::FontShade shade = Text::FontShade::BLACK );
            int secondsFromTotal() const;
            int minutesFromTotalSeconds() const;
            static int secondsFromTotal( int total_seconds );
            static int minutesFromTotalSeconds( int total_seconds );
            static std::string timeToString( int total_seconds );
            int totalSeconds() const;
            bool hitLimit() const;

        private:
            static const int DEFAULT_LIMIT = 599;
            static const Direction::Vertical DEFAULT_DIRECTION = Direction::Vertical::UP;
            static const int SECONDS_PER_MINUTE = 60;
            TimerRepeat timer_ = TimerRepeat( Unit::FPS );
            int total_seconds_;
            int limit_;
            Direction::Vertical direction_;
    };

#endif // CLOCK_H

