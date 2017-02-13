
// Name
//===================================
//
// TimerRepeat
//


#ifndef TIMER_REPEAT_H
#define TIMER_REPEAT_H


// FORWARD DECLARATIONS
//===================================


// DEPENDENCIES
//===================================

    #include "timer.h"


// CLASS
//===================================

    class TimerRepeat : public Timer
    {
        public:
            TimerRepeat
            (
                int limit = NORMAL_FRAMES,
                bool starts_on = true    ,
                int start_count = 0      ,
                Direction::Vertical direction = Direction::Vertical::UP
            );
            ~TimerRepeat();

            void start();
            void stop();

            void event( bool hit );

            bool hit() const;


        protected:
            bool hit_ = false;
    };

#endif // TIMER_REPEAT_H

