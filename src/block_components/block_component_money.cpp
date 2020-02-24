#include "audio.hpp"
#include "block_component_money.hpp"
#include "inventory_level.hpp"
#include "level_state.hpp"

BlockComponentMoney::BlockComponentMoney( int amount ) : amount_ ( amount ) {};

BlockComponentMoney::~BlockComponentMoney() {};

void BlockComponentMoney::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const
{
	level_state.inventory().addFunds( amount_ );
	Audio::playSound( Audio::SoundType::GEM );
};
