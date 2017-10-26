#include "block_component_hurt.hpp"
#include "health.hpp"

BlockComponentHurt::BlockComponentHurt() {};

BlockComponentHurt::~BlockComponentHurt() {};

void BlockComponentHurt::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks )
{
	health.hurt();
};