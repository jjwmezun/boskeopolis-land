#include "block_component_solid.hpp"
#include "block_type.hpp"
#include "collision.hpp"
#include "sprite.hpp"

BlockComponentSolid::BlockComponentSolid()
:
	BlockComponent( Type::SOLID )
{};

BlockComponentSolid::~BlockComponentSolid() {};

void BlockComponentSolid::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const
{
	sprite.collideStopAny( collision );
};
