#include "audio.hpp"
#include "block_component_mcguffin.hpp"
#include "inventory.hpp"

BlockComponentMcGuffin::BlockComponentMcGuffin() {};
BlockComponentMcGuffin::~BlockComponentMcGuffin() {};

void BlockComponentMcGuffin::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites ) const
{
	Inventory::addMcGuffin();
	Audio::playSound( Audio::SoundType::ITEM );
};