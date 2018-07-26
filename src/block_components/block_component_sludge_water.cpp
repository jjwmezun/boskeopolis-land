#include "block.hpp"
#include "block_component_sludge_water.hpp"
#include "health.hpp"
#include "sprite.hpp"

BlockComponentSludgeWater::BlockComponentSludgeWater() {};

BlockComponentSludgeWater::~BlockComponentSludgeWater() {};

void BlockComponentSludgeWater::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites ) const
{
	health.submerge();
	sprite.in_water_ = true;
};
