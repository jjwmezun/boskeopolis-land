#include "block.hpp"
#include "block_type.hpp"
#include "collision.hpp"
#include "level.hpp"
#include "map.hpp"
#include "sprite.hpp"

Block::Block
(
	int x, int y, BlockType* type, int location, int type_id
)
:
	Object( x, y, Unit::PIXELS_PER_BLOCK, Unit::PIXELS_PER_BLOCK ),
	type_ ( type ),
	location_ ( location ),
	type_id_ ( type_id )
{};

Block::~Block() {};

void Block::render( const Camera& camera, bool priority ) const
{
	if ( hasType() )
	{
		type_->render( Unit::SubPixelsToPixels( hitBox() ), priority, &camera );
	}
};

void Block::renderAnyPriority( const Camera& camera ) const
{
	if ( hasType() )
	{
		type_->renderAnyPriority( Unit::SubPixelsToPixels( hitBox() ), &camera );
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

void Block::interact( Sprite& sprite, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites )
{
	if ( hasType() )
	{
		Collision collision = sprite.testCollision( *this );
		type_->interact( collision, sprite, *this, level, events, camera, health, blocks, sprites );
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

void Block::init( Map& lvmap )
{
	if ( type_ != nullptr )
	{
		type_->init( *this, lvmap );
	}
};
