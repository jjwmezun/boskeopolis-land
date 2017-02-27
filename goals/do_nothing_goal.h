




// Name
//===================================
//
// DoNothingGoal
//

#ifndef DO_NOTHING_GOAL_H
#define DO_NOTHING_GOAL_H


// FORWARD DECLARATIONS
//===================================


// DEPENDENCIES
//===================================

    #include "goal.h"


// CLASS
//===================================

    class DoNothingGoal : public Goal
    {

        public:
            DoNothingGoal( int wait_time = 60 );
            ~DoNothingGoal();

            void init( Sprite& hero, InventoryLevel& inventory, EventSystem& events );
            void update( SpriteSystem& sprites, InventoryLevel& inventory, EventSystem& events, const Input& input );
    };

#endif // DO_NOTHING_GOAL_H
