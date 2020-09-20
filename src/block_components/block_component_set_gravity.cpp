#include "block_component_set_gravity.hpp"
#include "sprite.hpp"

BlockComponentSetGravity::BlockComponentSetGravity( double value ) : value_ ( value ) {};

BlockComponentSetGravity::~BlockComponentSetGravity() {};

void BlockComponentSetGravity::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const
{
    sprite.setGravityModifier( value_ );
};
