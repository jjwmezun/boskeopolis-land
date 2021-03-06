#include "audio.hpp"
#include "block.hpp"
#include "block_component_lock_box.hpp"
#include "breaking_lock_box_sprite.hpp"
#include "level_state.hpp"
#include "sprite_system.hpp"

BlockComponentLockBox::BlockComponentLockBox() {};
BlockComponentLockBox::~BlockComponentLockBox() {};

void BlockComponentLockBox::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const
{
	level_state.sprites().spawn( std::make_unique<BreakingLockBoxSprite> ( block.xPixels(), block.yPixels(), 0 ) );
	Audio::playSound( Audio::SoundType::UNLOCK );
};
