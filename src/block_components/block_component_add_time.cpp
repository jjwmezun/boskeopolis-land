#include "audio.hpp"
#include "block_component_add_time.hpp"
#include "inventory.hpp"
#include "level_state.hpp"

BlockComponentAddTime::BlockComponentAddTime( int seconds ) : seconds_ ( seconds ) {};
BlockComponentAddTime::~BlockComponentAddTime() {};

void BlockComponentAddTime::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const
{
	level_state.inventory().clock().addTime( seconds_ );
	Audio::playSound( Audio::SoundType::HEAL );
};
