#include "audio.hpp"
#include "block_component_key.hpp"
#include "event_system.hpp"
#include "level_state.hpp"

BlockComponentKey::BlockComponentKey() {};
BlockComponentKey::~BlockComponentKey() {};

void BlockComponentKey::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const
{
	level_state.events().getKey();
	Audio::playSound( Audio::SoundType::ITEM );
};
