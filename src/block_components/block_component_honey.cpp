#include "block_component_honey.hpp"
#include "sprite.hpp"
#include "sprite_movement.hpp"

BlockComponentHoney::BlockComponentHoney() {};
BlockComponentHoney::~BlockComponentHoney() {};

void BlockComponentHoney::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const
{
	if ( !sprite.isJumping() && sprite.hasMovementType( SpriteMovement::Type::GROUNDED ) )
	{
		sprite.changeMovement( SpriteMovement::Type::STUCK );
	}
};
