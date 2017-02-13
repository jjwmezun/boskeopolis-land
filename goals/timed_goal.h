



// Name
//===================================
//
// TimedGoal
//

#ifndef TIMED_GOAL_H
#define TIMED_GOAL_H


// FORWARD DECLARATIONS
//===================================


// DEPENDENCIES
//===================================

    #include "goal.h"


// CLASS
//===================================

    class TimedGoal : public Goal
    {

        public:
            TimedGoal( int time_limit = 120 );
            ~TimedGoal();

            void update( SpriteSystem& sprites, InventoryLevel& inventory, EventSystem& events, Input& input );
    };

#endif // TIMED_GOAL_H
