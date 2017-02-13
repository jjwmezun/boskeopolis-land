




// Name
//===================================
//
// AvoidMoneyGoal
//

#ifndef AVOID_MONEY_GOAL_H
#define AVOID_MONEY_GOAL_H


// FORWARD DECLARATIONS
//===================================


// DEPENDENCIES
//===================================

    #include "goal.h"


// CLASS
//===================================

    class AvoidMoneyGoal : public Goal
    {

        public:
            AvoidMoneyGoal();
            ~AvoidMoneyGoal();

            void update( SpriteSystem& sprites, InventoryLevel& inventory, EventSystem& events, Input& input );
    };

#endif // AVOID_MONEY_GOAL_H
