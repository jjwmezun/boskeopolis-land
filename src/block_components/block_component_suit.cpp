#include "audio.hpp"
#include "block_component_suit.hpp"
#include "inventory.hpp"
#include "level_state.hpp"

BlockComponentSuit::BlockComponentSuit( CardSuit suit ) : suit_ ( suit ) {};
BlockComponentSuit::~BlockComponentSuit() {};

void BlockComponentSuit::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const
{
	level_state.inventory().getSuit( suit_ );
	Audio::playSound( Audio::SoundType::ITEM );
};
