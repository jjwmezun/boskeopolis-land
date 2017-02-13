





// Name
//===================================
//
// WindyGoal
//

#ifndef WINDY_GOAL_H
#define WINDY_GOAL_H


// FORWARD DECLARATIONS
//===================================


// DEPENDENCIES
//===================================

    #include "direction.h"
    #include "goal.h"
    #include "sprite.h"


// CLASS
//===================================

    class WindyGoal : public Goal
    {

        public:
            WindyGoal( Direction::Horizontal direction = Direction::Horizontal::LEFT, unsigned int wind_strength = Sprite::RESISTANCE_X_WINDY );
            ~WindyGoal();

            void init( Sprite& hero, InventoryLevel& inventory, EventSystem& events );

        private:
            static constexpr int windStrength( Direction::Horizontal direction, unsigned int wind_strength )
            {
                return ( direction == Direction::Horizontal::RIGHT ? (int)wind_strength : (int)-wind_strength );
            };
    };

#endif // WINDY_GOAL_H
