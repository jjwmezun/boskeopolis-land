#include "block.hpp"
#include "block_component.hpp"
#include "block_condition.hpp"
#include "block_type.hpp"
#include "collision.hpp"
#include <iostream>
#include "mezun_math.hpp"
#include "sprite_graphics.hpp"

static constexpr int BLOCK_SIZE = 16;
static constexpr int MINI_BLOCK_SIZE = 8;
static constexpr int NUM_O_MINI_BLOCKS = 4;

BlockType::BlockType
(
	std::unique_ptr<SpriteGraphics> graphics,
	std::vector<std::unique_ptr<BlockComponent>> components,
	std::vector<std::vector<std::unique_ptr<BlockCondition>>> conditions
)
:
	graphics_   ( std::move( graphics ) ),
	components_ ( std::move( components ) ),
	conditions_ ( std::move( conditions ) )
{};

BlockType::~BlockType() {};

void BlockType::interact( Collision& collision, Sprite& sprite, Block& block, LevelState& level_state )
{
	for ( int i = 0; i < components_.size(); ++i )
	{
		if ( testCanInteract( i, collision, sprite, block, level_state ) )
		{
			if ( components_.at( i ) )
			{
				components_.at( i )->interact( collision, sprite, block, *this, level_state );
			}
		}
	}
};

void BlockType::renderAnyPriority( const sdl2::SDLRect& dest, const Camera* camera ) const
{
	if ( graphics_ )
	{
		graphics_->renderAnyPriority( dest, camera );
	}
};

void BlockType::render( const sdl2::SDLRect& dest, bool priority, const Camera* camera ) const
{
	if ( graphics_ )
	{
		graphics_->render( dest, camera, priority );
	}
};

void BlockType::update( EventSystem& events )
{
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

bool BlockType::testForComponentTypeNow( BlockComponent::Type type, const Collision& collision, const Sprite& sprite, const Block& block, LevelState& level_state ) const
{
	for ( int i = 0; i < components_.size(); ++i )
	{
		if ( testCanInteract( i, collision, sprite, block, level_state ) )
		{
			if ( components_.at( i ) )
			{
				if ( components_.at( i )->type() == type )
				{
					return true;
				}
			}
		}
	}
	return false;
}

const SpriteGraphics* BlockType::graphics() const
{
	return graphics_.get();
};

void BlockType::init( Block& block, LevelState& level_state ) const
{
	for ( const auto& component : components_ )
	{
		component->init( block, level_state );
	}
};

bool BlockType::testCanInteract( int i, const Collision& collision, const Sprite& sprite, const Block& block, LevelState& level_state ) const
{
	if ( i < conditions_.size() )
	{
		for ( int j = 0; j < conditions_[ i ].size(); ++j )
		{
			if ( conditions_.at( i ).at( j ) )
			{
				if ( !conditions_.at( i ).at( j )->condition( collision, sprite, block, level_state ) )
				{
					return false;
				}
			}
		}
	}
	return true;
};
