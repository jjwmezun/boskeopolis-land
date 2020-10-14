#include "block.hpp"
#include "block_type.hpp"
#include "collision.hpp"
#include "level.hpp"
#include "map.hpp"
#include "sprite.hpp"

Block::Block
(
	int x, int y, BlockType* type, int location, int layer, int type_id
)
:
	Object( x, y, Unit::PIXELS_PER_BLOCK, Unit::PIXELS_PER_BLOCK ),
	type_ ( type ),
	type_id_ ( type_id ),
	location_ ( location ),
	layer_ ( layer )
{};

Block::~Block() {};

void Block::render( const Camera& camera ) const
{
	if ( hasType() )
	{
		type_->render( Unit::SubPixelsToPixels( hitBox() ), &camera );
	}
};

void Block::destroy()
{
	type_ = nullptr;
	type_id_ = -1;
};

int Block::location() const
{
	return location_;
};

int Block::layer() const
{
	return layer_;
};

void Block::interact( Sprite& sprite, LevelState& level_state )
{
	if ( hasType() )
	{
		Collision collision = sprite.testBlockCollision( *this );
		type_->interact( collision, sprite, *this, level_state );
	}
};

int Block::typeID() const
{
	return type_id_;
};

bool Block::hasComponentType( BlockComponent::Type type ) const
{
	if ( hasType() )
	{
		return type_->hasComponentType( type );
	}
	return false;
};

bool Block::hasComponentTypeWhetherCollided( BlockComponent::Type type ) const
{
	if ( hasType() )
	{
		return type_->hasComponentTypeWhetherCollided( type );
	}
	return false;
};

bool Block::testForComponentTypeNow( BlockComponent::Type type, const Collision& collision, const Sprite& sprite, const Block& block, LevelState& level_state ) const
{
	if ( hasType() )
	{
		return type_->testForComponentTypeNow( type, collision, sprite, block, level_state );
	}
	return false;
}

bool Block::hasType() const
{
	return type_ != nullptr;
};

const BlockType* Block::type() const
{
	return type_;
};

bool Block::isDestroyed() const
{
	return !hasType();
}

void Block::init( LevelState& level_state )
{
	if ( type_ != nullptr )
	{
		type_->init( *this, level_state );
	}
};
