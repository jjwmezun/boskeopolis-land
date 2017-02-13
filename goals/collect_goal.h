


// Name
//===================================
//
// CollectGoal
//

#ifndef COLLECT_GOAL_H
#define COLLECT_GOAL_H


// FORWARD DECLARATIONS
//===================================


// DEPENDENCIES
//===================================

    #include "goal.h"


// CLASS
//===================================

    class CollectGoal : public Goal
    {

        public:
            CollectGoal( int amount_needed = 100 );
            ~CollectGoal();

            void update( SpriteSystem& sprites, InventoryLevel& inventory, EventSystem& events, Input& input );
    };

#endif // COLLECT_GOAL_H
