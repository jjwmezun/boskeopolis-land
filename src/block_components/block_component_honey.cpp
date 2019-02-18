#include "block_component_honey.hpp"
#include "sprite.hpp"
#include "sprite_movement.hpp"

BlockComponentHoney::BlockComponentHoney() {};
BlockComponentHoney::~BlockComponentHoney() {};

void BlockComponentHoney::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites ) const
{
	if ( !sprite.isJumping() && sprite.hasMovementType( SpriteMovement::Type::GROUNDED ) )
	{
		sprite.changeMovement( SpriteMovement::Type::STUCK );
	}
};
