
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

    #include "direction.h"


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
            void restart();
            virtual void start();
            virtual void stop();
            void update();

            bool on()     const;
            int counter() const;


        protected:
            static constexpr int NORMAL_FRAMES = 8;
			
            Direction::Vertical direction_;
            int limit_;
            int starts_on_;
            bool on_;
            int  counter_;

            void tick();
            void restartCounter();
            bool countsDown();
			
            virtual void event( bool hit ) = 0;
    };

#endif // TIMER_H


