#include "block_component_current.hpp"
#include "sprite.hpp"

BlockComponentCurrent::BlockComponentCurrent() {};
BlockComponentCurrent::~BlockComponentCurrent() {};

void BlockComponentCurrent::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites ) const
{
	sprite.hit_box_.x -= 910;

	if ( sprite.isDucking() )
	{
		// Kill the normal slide effect.
		sprite.vx_ = 0;
	}
	else if ( sprite.on_ground_prev_ || sprite.on_ground_ )
	{
		sprite.hit_box_.x -= 75;
	}
	else if ( sprite.collide_bottom_prev_ || sprite.collide_bottom_ )
	{
		sprite.hit_box_.x -= 1000;
	}
};
