#include "block_component_solid.hpp"
#include "block_type.hpp"
#include "collision.hpp"
#include "sprite.hpp"

BlockComponentSolid::BlockComponentSolid()
:
	BlockComponent( Type::SOLID )
{};

BlockComponentSolid::~BlockComponentSolid() {};

void BlockComponentSolid::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera )
{
	sprite.collideStopAny( collision );
};
