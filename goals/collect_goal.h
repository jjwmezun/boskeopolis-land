


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
	#include "mezun_helpers.h"


// CLASS
//===================================

    class CollectGoal : public Goal
    {

        public:
            CollectGoal( int amount_needed, std::string message = mezun::emptyString() );
            ~CollectGoal();

            void update( SpriteSystem& sprites, InventoryLevel& inventory, EventSystem& events, const Input& input );
			
		private:
			const std::string collectMessage( int amount_needed, std::string message ) const;
    };

#endif // COLLECT_GOAL_H
