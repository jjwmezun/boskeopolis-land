
// Name
//===================================
//
// TimerSimple
//


#ifndef TIMER_SIMPLE_H
#define TIMER_SIMPLE_H


// FORWARD DECLARATIONS
//===================================


// DEPENDENCIES
//===================================

    #include "timer.h"


// CLASS
//===================================

    class TimerSimple : public Timer
    {
        public:
            TimerSimple
            (
                int limit = NORMAL_FRAMES,
                bool starts_on = true    ,
                int start_count = 0      ,
                Direction::Vertical direction = Direction::Vertical::UP
            );
            ~TimerSimple();

            void start();

            void event( bool e );
            bool done() const;
            bool hit() const;
            void stop();

        protected:
            bool done_ = false;
    };

#endif // TIMER_SIMPLE_H
