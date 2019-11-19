#include <cassert>
#include "collision.hpp"
#include "event_system.hpp"
#include "map.hpp"
#include "pedestrian_sprite.hpp"
#include "sprite_graphics.hpp"

static constexpr int NUMBER_OF_MOVEABLE_BLOCK_TYPES = 10;
static constexpr int MOVEABLE_BLOCK_TYPES[ NUMBER_OF_MOVEABLE_BLOCK_TYPES ] =
{
	100,
	66,
	106,
	107,
	108,
	109,
	93,
	112,
	77,
	74
};
static bool testMoveableBlockType( int value );

PedestrianSprite::PedestrianSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/pedestrian.png", 0, 0, false, false, 0.0, false, -1, -1, 2, 2 ), x + 6, y + 5, 2, 5, {}, 250, 250, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY, true, false ),
	animation_ ()
{
	direction_ = Direction::Simple::RIGHT;
};

PedestrianSprite::~PedestrianSprite() {};

void PedestrianSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	moveInDirection();
	if ( isAtTransitionPoint() )
	{
		const int current_block_x = Unit::SubPixelsToBlocks( hit_box_.x );
		const int current_block_y = Unit::SubPixelsToBlocks( hit_box_.y );
		const int current_block_n = lvmap.indexFromXAndY( current_block_x, current_block_y );

		const int block_types[ 4 ] =
		{
			lvmap.block( current_block_n - lvmap.widthBlocks() ), // UP
			lvmap.block( current_block_n + 1                   ), // RIGHT
			lvmap.block( current_block_n + lvmap.widthBlocks() ), // DOWN
			lvmap.block( current_block_n - 1 				   )  // LEFT
		};

		int usable_blocks[ 4 ] = { -1, -1, -1, -1 };
		int usable_blocks_count = 0;
		const int opposite_direction_index = ( int )( oppositeSimple( direction_ ) ) - 1;
		assert( opposite_direction_index >= 0 && opposite_direction_index < 4 );

		for ( int i = 0; i < 4; ++i )
		{
			if ( i != opposite_direction_index && testMoveableBlockType( block_types[ i ] ) )
			{
				usable_blocks[ usable_blocks_count ] = i;
				++usable_blocks_count;
			}
		}

		if ( usable_blocks_count == 0 )
		{
			direction_ = ( Direction::Simple )( opposite_direction_index + 1 );
		}
		else
		{
			const int random_direction = mezun::randInCArray( usable_blocks, usable_blocks_count );
			assert( random_direction < 4 && random_direction >= 0 );
			direction_ = ( Direction::Simple )( random_direction + 1 );
		}

		fullStopX();
		fullStopY();
		moveInDirection();
	}

	animation_.update();
	graphics_->current_frame_x_ = animation_.get() * 4;
};

void PedestrianSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( my_collision.collideAny() )
	{
		events.fail();
	}
};

bool PedestrianSprite::isAtTransitionPoint() const
{
	return ( Direction::SimpleIsHorizontal( direction_ ) )
		? hit_box_.x % Unit::SUBPIXELS_PER_BLOCK == 6000
		: hit_box_.y % Unit::SUBPIXELS_PER_BLOCK == 5000;
}

static bool testMoveableBlockType( int value )
{
	for ( int i = 0; i < NUMBER_OF_MOVEABLE_BLOCK_TYPES; ++i )
	{
		if ( MOVEABLE_BLOCK_TYPES[ i ] == value )
		{
			return true;
		}
	}
	return false;
}