#include "block_component_sink.hpp"
#include "collision.hpp"
#include "sprite.hpp"

BlockComponentSink::BlockComponentSink() {};

BlockComponentSink::~BlockComponentSink() {};

void BlockComponentSink::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites ) const
{	
	sprite.fullStopY();
	sprite.addToY( 100 );
	
	if ( collision.collideTop() )
	{
		sprite.kill();
	}
};
