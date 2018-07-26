#include "block_component_conveyor.hpp"
#include "block_system.hpp"
#include "collision.hpp"
#include "event_system.hpp"
#include "sprite.hpp"

BlockComponentConveyor::BlockComponentConveyor( Direction::Horizontal direction, int speed )
:
	x_effect_ ( ( direction == Direction::Horizontal::LEFT ) ? -speed : speed )
{};

BlockComponentConveyor::~BlockComponentConveyor() {};

void BlockComponentConveyor::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites ) const
{
	int x_effect = x_effect_;
	if ( events.switchOn() )
	{
		x_effect = -x_effect_;
	}

	sdl2::SDLRect block_test = { sprite.hit_box_.x - 2000, sprite.hit_box_.y + 3000, 4000, sprite.hit_box_.h - 6000 };
	if ( x_effect > 0 )
	{
		block_test = { sprite.hit_box_.right() - 2000, sprite.hit_box_.y + 3000, 4000, sprite.hit_box_.h - 6000 };
	}

	if ( !blocks.blocksInTheWay( block_test, BlockComponent::Type::SOLID ) )
	{
		sprite.addToX( x_effect );
	}

	if ( sprite.hasType( Sprite::SpriteType::HERO ) && collision.collideBottom() )
	{
		events.on_conveyor_belt_ = true;
	}
};