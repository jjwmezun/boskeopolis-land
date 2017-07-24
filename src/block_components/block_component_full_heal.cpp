#include "block_component_full_heal.hpp"
#include "health.hpp"

BlockComponentFullHeal::BlockComponentFullHeal() {};

BlockComponentFullHeal::~BlockComponentFullHeal() {};

void BlockComponentFullHeal::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health )
{
	health.fullHeal();
};