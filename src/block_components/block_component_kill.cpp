#include "block_component_kill.hpp"
#include "sprite.hpp"

BlockComponentKill::BlockComponentKill() {};

BlockComponentKill::~BlockComponentKill() {};

void BlockComponentKill::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera )
{
	sprite.killNoAnimation();
};