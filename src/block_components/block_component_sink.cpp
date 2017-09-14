#include "block_component_sink.hpp"
#include "collision.hpp"
#include "sprite.hpp"

BlockComponentSink::BlockComponentSink() {};

BlockComponentSink::~BlockComponentSink() {};

void BlockComponentSink::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health )
{	
	sprite.fullStopY();
	sprite.addToY( 100 );
	
	if ( collision.collideTop() )
	{
		sprite.kill();
	}
};
