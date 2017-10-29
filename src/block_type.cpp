#include "block.hpp"
#include "block_component.hpp"
#include "block_condition.hpp"
#include "block_type.hpp"
#include "collision.hpp"
#include <iostream>
#include "mezun_math.hpp"
#include "sprite_graphics.hpp"

BlockType::BlockType
(
	std::unique_ptr<SpriteGraphics> graphics,
	std::vector<std::unique_ptr<BlockComponent>> components,
	std::vector<std::vector<std::unique_ptr<BlockCondition>>> conditions
)
:
	graphics_   ( std::move( graphics ) ),
	components_ ( std::move( components ) ),
	conditions_ ( std::move( conditions ) ),
	prev_direction_ ( prev_direction_ = Direction::Clockwise::__NULL )
{};

BlockType::~BlockType() {};

void BlockType::interact( Collision& collision, Sprite& sprite, Block& block, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks )
{
	for ( int i = 0; i < components_.size(); ++i )
	{
		bool can_interact = true;

		if ( i < conditions_.size() )
		{
			for ( int j = 0; j < conditions_[ i ].size(); ++j )
			{
				if ( conditions_.at( i ).at( j ) )
				{
					if ( !conditions_.at( i ).at( j )->condition( collision, sprite, block, events ) )
					{
						can_interact = false;
						break;
					}
				}
			}
		}

		if ( can_interact )
		{
			if ( components_.at( i ) )
			{
				components_.at( i )->interact( collision, sprite, block, *this, level, events, camera, health, blocks );
			}
		}
	}
};

void BlockType::renderAnyPriority( const Camera& camera, const Block& block ) const
{
	if ( graphics_ )
	{
		graphics_->renderAnyPriority( Unit::SubPixelsToPixels( block.hitBox() ), &camera );
	}
};

void BlockType::render( const Camera& camera, const Block& block, bool priority ) const
{
	if ( graphics_ )
	{
		graphics_->render( Unit::SubPixelsToPixels( block.hitBox() ), &camera, priority );
	}
};

void BlockType::update( EventSystem& events )
{
	for ( auto& c : components_ )
	{
		c->update( events, *this );
	}

	if ( graphics_ )
	{
		graphics_->update( events );
	}

};

bool BlockType::hasComponentType( BlockComponent::Type type ) const
{
	for ( auto& t : components_ )
	{
		// True if a'least 1 component has type.
		if ( t->type() == type ) return true;
	}

	// If all don't, false.
	return false;
};

void BlockType::rotate( Direction::Clockwise direction )
{
	if ( graphics_ )
	{
		int amount = 5;

		if ( direction == Direction::Clockwise::COUNTERCLOCKWISE )
		{
			amount *= -1;
		}

		graphics_->rotation_ += amount;
	}

	prev_direction_ = direction;
};

void BlockType::readjust()
{
	if ( graphics_ )
	{
		if ( !mezun::isAtRightAngle( ( int )graphics_->rotation_ ) )
		{
			rotate( prev_direction_ );
		}
	}
};

const SpriteGraphics* BlockType::graphics() const
{
	return graphics_.get();
};