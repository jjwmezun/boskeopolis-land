#include "block_component_message.hpp"
#include "event_system.hpp"

BlockComponentMessage::BlockComponentMessage() {};

BlockComponentMessage::~BlockComponentMessage() {};

void BlockComponentMessage::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const
{
	events.showMessage();
};
