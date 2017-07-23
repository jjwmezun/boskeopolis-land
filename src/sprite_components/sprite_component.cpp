

// Name
//===================================
//
// SpriteComponent
//


// DEPENDENCIES
//===================================

    #include "sprite_component.hpp"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    SpriteComponent::SpriteComponent( bool before_update )
    :
        before_update_ ( before_update )
    {};

    SpriteComponent::~SpriteComponent() {};

    bool SpriteComponent::beforeUpdate() const { return before_update_; };

    void SpriteComponent::reset() {};
