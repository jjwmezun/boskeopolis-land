
// Name
//===================================
//
// Goal
//


// DEPENDENCIES
//===================================

    #include "goal.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    Goal::Goal( std::string message, GoalType type, int value ) : type_ ( type ), value_ ( value ), message_ ( message ) {};

    Goal::~Goal() {};

    void Goal::update( SpriteSystem& sprites, InventoryLevel& inventory, EventSystem& events, Input& input )
    {
    };

    void Goal::init( Sprite& hero, InventoryLevel& inventory, EventSystem& events )
    {
    };

    std::string Goal::message()
    {
        return message_;
    };

    Goal::GoalType Goal::type() const { return type_; };
    int Goal::value() const { return value_; };
