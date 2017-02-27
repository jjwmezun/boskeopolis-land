



// Name
//===================================
//
// DoNothingGoal
//


// DEPENDENCIES
//===================================

    #include "do_nothing_goal.h"
    #include "event_system.h"
    #include "input.h"
    #include "inventory_level.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    DoNothingGoal::DoNothingGoal( int wait_time )
    :
        Goal( "¡Don't do anything @ all!", GoalType::DO_NOTHING, wait_time )
    {};

    DoNothingGoal::~DoNothingGoal() {};

    void DoNothingGoal::update( SpriteSystem& sprites, InventoryLevel& inventory, EventSystem& events, const Input& input )
    {
        if ( input.pressed( Input::Action::JUMP ) || input.pressed( Input::Action::MOVE_DOWN ) || input.pressed( Input::Action::MOVE_LEFT ) || input.pressed( Input::Action::MOVE_RIGHT ) )
            events.failed_ = true;

        if ( inventory.clockTime() >= value() )
            events.won_ = true;
    };

    void DoNothingGoal::init( Sprite& hero, InventoryLevel& inventory, EventSystem& events )
    {
        //inventory.clock().reset( Direction::Vertical::DOWN, value() );
    };
