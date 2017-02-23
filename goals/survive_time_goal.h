



// Name
//===================================
//
// SurviveTimeGoal
//

#ifndef SURVIVE_TIME_GOAL_H
#define SURVIVE_TIME_GOAL_H


// FORWARD DECLARATIONS
//===================================


// DEPENDENCIES
//===================================

    #include "goal.h"


// CLASS
//===================================

    class SurviveTimeGoal : public Goal
    {

        public:
            SurviveTimeGoal( int wait_time = 30, std::string message = "" );
            ~SurviveTimeGoal();

            void update( SpriteSystem& sprites, InventoryLevel& inventory, EventSystem& events, Input& input );
			
		private:
			const std::string surviveMessage( int wait_time, std::string message ) const;
    };

#endif // SURVIVE_TIME_GOAL_H
