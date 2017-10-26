#include "block_component_message.hpp"
#include "event_system.hpp"

BlockComponentMessage::BlockComponentMessage() {};

BlockComponentMessage::~BlockComponentMessage() {};

void BlockComponentMessage::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks )
{
	events.showMessage();
};
