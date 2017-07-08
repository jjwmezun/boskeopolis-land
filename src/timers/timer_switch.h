
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

	#include <cmath>


// CLASS
//===================================

    class TimerSwitch
    {
        public:
            TimerSwitch
            (
                int switch_duration = 8,
				int trans_duration  = 4,
                bool starts_on      = true
            );
            ~TimerSwitch();

			void pause();
			void resume();
			void restart();
            void start();
            void stop();
			void update();

			bool   on() const;
            bool   switchStatus() const;
			bool   inTrans() const;
			double transPercent() const;
			
			void changeDurations( int switch_duration = -1, int trans_duration = -1 );
			void speedUp();
			void slowDown();


        private:
			bool on_;
            bool switch_;
			bool trans_;
		
			int switch_duration_;
			int trans_duration_;
			int switch_counter_;
			int trans_counter_;
			
            void changeSwitch();
			void restartCounters();
    };

#endif // TIMER_SWITCH_H


