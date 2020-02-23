#include "block_component_switch_sound.hpp"
#include "audio.hpp"

BlockComponentSwitchSound::BlockComponentSwitchSound() {};
BlockComponentSwitchSound::~BlockComponentSwitchSound() {};

void BlockComponentSwitchSound::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const
{
	Audio::playSound( Audio::SoundType::SWITCH );
};
