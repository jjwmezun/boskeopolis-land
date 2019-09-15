#include "block_component_spark_wall.hpp"
#include "block_type.hpp"
#include "collision.hpp"
#include "sprite.hpp"

BlockComponentSparkWall::BlockComponentSparkWall() {};
BlockComponentSparkWall::~BlockComponentSparkWall() {};

void BlockComponentSparkWall::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites ) const
{
	if ( sprite.hasType( Sprite::SpriteType::SPARK ) )
	{
		sprite.collide_left_ = true;
	}
};
