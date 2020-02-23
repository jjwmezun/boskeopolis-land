#include "audio.hpp"
#include "block_component_money.hpp"
#include "inventory.hpp"

BlockComponentMoney::BlockComponentMoney( int amount ) : amount_ ( amount ) {};

BlockComponentMoney::~BlockComponentMoney() {};

void BlockComponentMoney::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const
{
	Inventory::addFunds( amount_ );
	Audio::playSound( Audio::SoundType::GEM );
};
