



// Name
//===================================
//
// StarvingGoal
//

#ifndef STARVING_GOAL_H
#define STARVING_GOAL_H


// FORWARD DECLARATIONS
//===================================


// DEPENDENCIES
//===================================

    #include "goal.h"
    #include "timers/timer_repeat.h"


// CLASS
//===================================

    class StarvingGoal : public Goal
    {

        public:
            StarvingGoal( int amount_lost_per_frame = 50 );
            ~StarvingGoal();

            void update( SpriteSystem& sprites, InventoryLevel& inventory, EventSystem& events, const Input& input );

        private:
            TimerRepeat timer_ = TimerRepeat( 16 );
            bool started_ = false;
    };

#endif // STARVING_GOAL_H
