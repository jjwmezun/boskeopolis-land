#include "block.hpp"
#include "block_type.hpp"
#include "collision.hpp"
#include "level.hpp"
#include "map.hpp"
#include "sprite.hpp"
#include <iostream>

Block::Block
(
	int x, int y, BlockType* type, int location, int type_id
)
:
	Object( x, y, Unit::PIXELS_PER_BLOCK, Unit::PIXELS_PER_BLOCK ),
	type_ ( type ),
	location_ ( location ),
	type_id_ ( type_id ),
	destroyed_ ( false )
{};

// DO NOT DELETE BLOCK TYPE. Owned by other class; this holds just a copy.
Block::~Block() {};

Block::Block( Block&& m )
:
	Object( Unit::SubPixelsToPixels( m.hit_box_.x ), Unit::SubPixelsToPixels( m.hit_box_.y ), Unit::PIXELS_PER_BLOCK, Unit::PIXELS_PER_BLOCK ),
	type_ ( m.type_ ),
	location_ ( m.location_ ),
	type_id_ ( m.type_id_ ),
	destroyed_ ( m.destroyed_ )
{};

Block::Block( const Block& c )
:
	Object( Unit::SubPixelsToPixels( c.hit_box_.x ), Unit::SubPixelsToPixels( c.hit_box_.y ), Unit::PIXELS_PER_BLOCK, Unit::PIXELS_PER_BLOCK ),
	type_ ( c.type_ ),
	location_ ( c.location_ ),
	type_id_ ( c.type_id_ ),
	destroyed_ ( c.destroyed_ )
{};

Block& Block::operator=( Block&& m )
{
	hit_box_ = m.hit_box_;
	type_ = m.type_;
	location_ = m.location_;
	type_id_ = m.type_id_;
	destroyed_ = m.destroyed_;
};

Block& Block::operator=( const Block& c )
{
	hit_box_ = c.hit_box_;
	type_ = c.type_;
	location_ = c.location_;
	type_id_ = c.type_id_;
	destroyed_ = c.destroyed_;
};

void Block::render( const Camera& camera, bool priority ) const
{
	if ( hasType() )
	{
		type_->render( camera, *this, priority );
	}
};

void Block::renderAnyPriority( const Camera& camera ) const
{
	if ( hasType() )
	{
		type_->renderAnyPriority( camera, *this );
	}
};

void Block::destroy()
{
	destroyed_ = true;
};

int Block::location() const
{
	return location_;
};

void Block::interact( Sprite& sprite, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks )
{
	if ( hasType() && areNearbyWithAllBlocks( sprite, level.currentMap() ) )
	{
		Collision collision = sprite.testCollision( *this );
		type_->interact( collision, sprite, *this, level, events, camera, health, blocks );
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
};

bool Block::hasType() const
{
	return type_ != nullptr;
};

const BlockType* Block::type() const
{
	return type_;
};

bool Block::areNearbyWithAllBlocks( const Object& other, const Map& lvmap ) const
{
	return
		!lvmap.blocks_work_offscreen_ ||
		(
			other.rightSubPixels() > hit_box_.x - Unit::WINDOW_WIDTH_SUBPIXELS &&
			other.hit_box_.x < rightSubPixels() + Unit::WINDOW_WIDTH_SUBPIXELS
		);
};