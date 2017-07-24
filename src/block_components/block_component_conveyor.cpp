#include "block_component_conveyor.hpp"
#include "sprite.hpp"

BlockComponentConveyor::BlockComponentConveyor( Direction::Horizontal direction, int speed )
:
	x_effect_ ( ( direction == Direction::Horizontal::LEFT ) ? -speed : speed )
{};

BlockComponentConveyor::~BlockComponentConveyor() {};

void BlockComponentConveyor::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health )
{
	sprite.addToX( x_effect_ );
};
