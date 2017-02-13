
// Name
//===================================
//
// TimerSwitch
//


#ifndef TIMER_SWITCH_H
#define TIMER_SWITCH_H


// FORWARD DECLARATIONS
//===================================


// DEPENDENCIES
//===================================

    #include "timer.h"


// CLASS
//===================================

    class TimerSwitch : public Timer
    {
        public:
            TimerSwitch
            (
                int limit = NORMAL_FRAMES,
                bool starts_on = true    ,
                int start_count = 0      ,
                Direction::Vertical direction = Direction::Vertical::UP
            );
            ~TimerSwitch();

            void start();
            void stop();

            void event( bool hit );

            bool switchStatus() const;

        protected:
            bool switch_ = false;
            void changeSwitch();
    };

#endif // TIMER_SWITCH_H


