#include "block_component_bouncy.hpp"
#include "collision.hpp"
#include "sprite.hpp"

BlockComponentBouncy::BlockComponentBouncy( int strength ) : strength_ ( strength ) {};

BlockComponentBouncy::~BlockComponentBouncy() {};

void BlockComponentBouncy::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health )
{
	if ( collision.collideBottom() )
	{
		sprite.bounce( strength_ );
	}
	else if ( collision.collideLeft() )
	{
		sprite.bounceLeft( collision.overlapXLeft() );
	}
	else if ( collision.collideRight() )
	{
		sprite.bounceRight( collision.overlapXRight() );
	}
	else if ( collision.collideTop() )
	{
		sprite.bounceDownward( collision.overlapYTop() );
	}
};
