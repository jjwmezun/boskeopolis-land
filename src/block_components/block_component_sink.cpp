#include "block_component_sink.h"
#include "collision.h"
#include "sprite.h"

BlockComponentSink::BlockComponentSink() {};

BlockComponentSink::~BlockComponentSink() {};

void BlockComponentSink::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera )
{	
	sprite.fullStopY();
	sprite.addToY( 100 );
};
