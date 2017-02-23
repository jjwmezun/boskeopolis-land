

// Name
//===================================
//
// Goal
//

#ifndef GOAL_H
#define GOAL_H


// FORWARD DECLARATIONS
//===================================

    class EventSystem;
    class Input;
    class InventoryLevel;
    class Sprite;
    class SpriteSystem;


// DEPENDENCIES
//===================================

    #include <string>


// CLASS
//===================================

    class Goal
    {

        public:
            enum class GoalType
            {
                NORMAL,
                COLLECT,
                TIMED,
				SURVIVE_TIME,
                STARVING,
                AVOID_MONEY,
                AVOID_WALLS,
                WINDY,
                DO_NOTHING
            };

            Goal( std::string message = "¡Get the Keycane!", GoalType type = GoalType::NORMAL, int value = 0 );
            virtual ~Goal();

            virtual void update( SpriteSystem& sprites, InventoryLevel& inventory, EventSystem& events, Input& input );
            virtual void init( Sprite& hero, InventoryLevel& inventory, EventSystem& events );

            std::string message();
            GoalType type() const;
            int value() const;

        private:
            const GoalType type_;
            const std::string message_;
            const int value_;
    };

#endif // GOAL_H
