#include "block_component_switch_sound.hpp"
#include "audio.hpp"

BlockComponentSwitchSound::BlockComponentSwitchSound() {};
BlockComponentSwitchSound::~BlockComponentSwitchSound() {};

void BlockComponentSwitchSound::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites ) const
{
	Audio::playSound( Audio::SoundType::SWITCH );
};
