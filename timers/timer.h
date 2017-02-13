
// Name
//===================================
//
// Timer
//


#ifndef TIMER_H
#define TIMER_H


// FORWARD DECLARATIONS
//===================================


// DEPENDENCIES
//===================================

    #include "../direction.h"


// CLASS
//===================================

    class Timer
    {
        public:
            Timer
            (
                int limit = NORMAL_FRAMES,
                bool starts_on = true    ,
                int start_count = 0      ,
                Direction::Vertical direction = Direction::Vertical::UP
            );
            virtual ~Timer();
            void pause();
            void resume();
            virtual void start();
            virtual void stop();
            void update();

            virtual void event( bool hit ) = 0;

            bool on()     const;
            int counter() const;

        protected:
            static const int NORMAL_FRAMES = 8;
            Direction::Vertical direction_ = Direction::Vertical::UP;
            int limit_;
            int starts_on_;
            bool on_ = true;
            int  counter_ = 0;

            void tick();
            void restartCounter();
            bool countsDown();
    };

#endif // TIMER_H


