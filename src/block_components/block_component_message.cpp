#include "block_component_message.hpp"
#include "event_system.hpp"
#include "level_state.hpp"

BlockComponentMessage::BlockComponentMessage() {};

BlockComponentMessage::~BlockComponentMessage() {};

void BlockComponentMessage::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const
{
	level_state.events().showMessage();
};
