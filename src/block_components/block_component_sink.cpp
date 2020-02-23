#include "block_component_sink.hpp"
#include "collision.hpp"
#include "sprite.hpp"

BlockComponentSink::BlockComponentSink() {};

BlockComponentSink::~BlockComponentSink() {};

void BlockComponentSink::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const
{	
	sprite.fullStopY();
	sprite.addToY( 100 );
	
	if ( collision.collideTop() )
	{
		sprite.kill();
	}
};
