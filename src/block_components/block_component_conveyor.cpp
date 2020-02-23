#include "block_component_conveyor.hpp"
#include "block_system.hpp"
#include "collision.hpp"
#include "event_system.hpp"
#include "level_state.hpp"
#include "sprite.hpp"

BlockComponentConveyor::BlockComponentConveyor( Direction::Horizontal direction, int speed )
:
	x_effect_ ( ( direction == Direction::Horizontal::LEFT ) ? -speed : speed )
{};

BlockComponentConveyor::~BlockComponentConveyor() {};

void BlockComponentConveyor::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const
{
	int x_effect = x_effect_;
	if ( level_state.events().isSwitchOn() )
	{
		x_effect = -x_effect_;
	}

	sdl2::SDLRect block_test = { sprite.hit_box_.x - 2000, sprite.hit_box_.y + 3000, 4000, sprite.hit_box_.h - 6000 };
	if ( x_effect > 0 )
	{
		block_test = { sprite.hit_box_.right() - 2000, sprite.hit_box_.y + 3000, 4000, sprite.hit_box_.h - 6000 };
	}

	if ( !level_state.blocks().blocksInTheWay( block_test, BlockComponent::Type::SOLID ) )
	{
		sprite.addToX( x_effect );
	}

	if ( sprite.hasType( Sprite::SpriteType::HERO ) && collision.collideBottom() )
	{
		level_state.events().setOnConveyorBelt();
	}
};