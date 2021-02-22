#include "block_component_set_gravity.hpp"
#include "level_state.hpp"
#include "sprite.hpp"
#include "sprite_system.hpp"

BlockComponentSetGravity::BlockComponentSetGravity( double value ) : value_ ( value ) {};

BlockComponentSetGravity::~BlockComponentSetGravity() {};

void BlockComponentSetGravity::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const
{
    sprite.setGravityModifier( value_ );
    for ( auto& other : level_state.sprites().getSpritesList() )
    {
        other->setGravityModifier( value_ );
    }
};
