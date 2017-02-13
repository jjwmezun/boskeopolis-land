




// Name
//===================================
//
// WindyGoal
//


// DEPENDENCIES
//===================================

    #include "windy_goal.h"
    #include "sprite.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    WindyGoal::WindyGoal( Direction::Horizontal direction, unsigned int wind_strength )
    :
        Goal( "O, fun: a lazy wind gimmick.", GoalType::WINDY, windStrength( direction, wind_strength ) )
    {};

    WindyGoal::~WindyGoal() {};

    void WindyGoal::init( Sprite& hero, InventoryLevel& inventory, EventSystem& events )
    {
        Sprite::resistance_x_ = value();
    };
