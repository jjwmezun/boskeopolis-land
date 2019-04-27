#include "block.hpp"
#include "block_component_hole.hpp"
#include "block_type.hpp"
#include "level.hpp"
#include "map.hpp"
#include "sprite.hpp"
#include "sprite_graphics.hpp"

BlockComponentHole::BlockComponentHole() {};
BlockComponentHole::~BlockComponentHole() {};

void BlockComponentHole::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites ) const
{
	if ( sprite.hasType( Sprite::SpriteType::MOVEABLE_BLOCK ) )
	{
		const bool condition = sprite.hit_box_.x + 2000 > block.hit_box_.x &&
							   sprite.rightSubPixels() - 2000 < block.rightSubPixels() &&
							   sprite.bottomSubPixels() > block.hit_box_.y &&
							   sprite.hit_box_.y < block.bottomSubPixels();
		if ( condition )
		{
			sprite.hit_box_.x = block.hit_box_.x;
			sprite.graphics_->current_frame_x_ = 16;
			sprite.top_speed_ = 1;
			level.currentMap().changeBlock( block.location(), 75 + 65 );
		}
	}
};