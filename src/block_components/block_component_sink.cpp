#include "block_component_sink.hpp"
#include "collision.hpp"
#include "sprite.hpp"

BlockComponentSink::BlockComponentSink() {};

BlockComponentSink::~BlockComponentSink() {};

void BlockComponentSink::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera )
{	
	sprite.fullStopY();
	sprite.addToY( 100 );
};
