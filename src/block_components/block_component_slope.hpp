#pragma once

#include "block.hpp"
#include "block_component.hpp"
#include "collision.hpp"
#include "direction.hpp"
#include <memory>
#include "sprite.hpp"

constexpr int getYBaselineFromX( int relative_height, int slope_width, bool steep )
{
	// Bottomost pixel o' lowest slope is 1, so we always start with 1000 subpixels.
	//
	// Since whole slope is 1 block high, but a fraction o' slope width wide,
	// add a fraction o' a block's y pixels by slope width for every slope block
	// beyond the 1st.
	return ( steep )
		? 0
		: 1000 + ( ( int )( floor( Unit::SUBPIXELS_PER_BLOCK / slope_width ) ) * ( relative_height - 1 ) );
};

template <bool steep, Direction::Horizontal direction, int relative, int slope_width>
inline bool testSpritePositionWithinBlocksXThreshold( const Sprite& sprite, const Block& block )
{
	const int change = ( int )( floor( Unit::SUBPIXELS_PER_BLOCK / slope_width ) ) * ( relative - 1 );
	return ( steep )
		?
			(
				( direction == Direction::Horizontal::LEFT )
				? sprite.centerXSubPixels() >= block.leftSubPixels() + change && sprite.centerXSubPixels() <= block.rightSubPixels()
				: sprite.centerXSubPixels() >= block.leftSubPixels() && sprite.centerXSubPixels() <= block.rightSubPixels() - change
			)
		: sprite.centerXSubPixels() >= block.leftSubPixels() && sprite.centerXSubPixels() <= block.rightSubPixels();
};

template <Direction::Horizontal direction>
inline int getYAdjustFromSpritesRelativeXPosition( const Sprite& sprite, const Block& block )
{
	// For left slope, calculate # o' pixels left o' left end;
	// for right slope, calculate # o' pixels from right end.
	return ( direction == Direction::Horizontal::LEFT )
		? sprite.centerXSubPixels() - block.leftSubPixels()
		: block.rightSubPixels() - sprite.centerXSubPixels();
};

template <Direction::Horizontal direction, int slope_width, int relative, bool steep>
inline int getSlopeYChangeFromX( const Sprite& sprite, const Block& block )
{
	return
		( steep )
			? std::min
			(
				( getYAdjustFromSpritesRelativeXPosition<direction> ( sprite, block ) - Unit::PixelsToSubPixels( relative ) ) * slope_width,
				16000
			)
			: getYAdjustFromSpritesRelativeXPosition<direction> ( sprite, block ) / slope_width;
};

template <Direction::Horizontal direction, int slope_width, int extra_push, int relative_height, int resistance_percentage, bool steep>
class BlockComponentSlope : public BlockComponent
{
	public:
		BlockComponentSlope() {};
		~BlockComponentSlope() {};
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites ) const override
		{
			if ( collision.collideAny() )
			{
				// Prevent high slopes from being clipped from below 'bove low ceilings.
				if ( ( relative_height == 4 || relative_height == 3 ) && collision.collideTop() )
				{
					return;
				}

				if ( testSpritePositionWithinBlocksXThreshold<steep, direction, relative_height, slope_width> ( sprite, block ) )
				{
					sprite.on_slope_ = direction;
					sprite.collideStopYBottom( 0 );
					if ( sprite.vy_ >= 0 )
					{
						stopSpriteAtSlopeTop( sprite, block );
						adjustSpritesSpeed( sprite );
					}
				}
			}
		};

	private:
		void stopSpriteAtSlopeTop( Sprite& sprite, const Block& block ) const
		{
			// If slope is below slope top o' current x position,
			// set its y position to slope top.
			sprite.changeY( std::min( sprite.ySubPixels(), getSlopeTopFromX( sprite, block ) ) );
		};

		int getSlopeTopFromX( const Sprite& sprite, const Block& block ) const
		{
			return
				block.bottomSubPixels() -
				getYBaselineFromX( relative_height, slope_width, steep ) -
				getSlopeYChangeFromX<direction, slope_width, relative_height, steep> ( sprite, block ) -
				sprite.heightSubPixels();
		}

		void adjustSpritesSpeed( Sprite& sprite ) const
		{
			switch ( direction )
			{
				case ( Direction::Horizontal::LEFT ):
				{
					if ( sprite.vx_ >= 0 )
					{
						sprite.vx_ /= getResistance();
					}
					else if ( sprite.vx_ <= 0 )
					{
						sprite.vx_ *= getResistance();
					}

					sprite.vx_ -= extra_push;
				}
				break;

				case ( Direction::Horizontal::RIGHT ):
				{
					if ( sprite.vx_ <= 0 )
					{
						sprite.vx_ /= getResistance();
					}
					else if ( sprite.vx_ >= 0 )
					{
						sprite.vx_ *= getResistance();
					}

					sprite.vx_ += extra_push;
				}
				break;
			}
		}

		double getResistance() const
		{
			//
			// 'Cause this # will be multiplied or divided by, 1 is the
			// normalizing factor ( no resistance ), with the percentage
			// added to that factor.
			//
			return 1.0 + ( ( double )( resistance_percentage ) / 100.0 );
		}
};

template <Direction::Horizontal direction, int slope_width, int relative_height>
class BlockComponentCeilingSlope : public BlockComponent
{
	public:
		BlockComponentCeilingSlope() {};
		~BlockComponentCeilingSlope() {};

		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites ) const
		{
			if ( collision.collideAny() )
			{
				if ( testSpritePositionWithinBlocksXThreshold<false, direction, relative_height, slope_width> ( sprite, block ) )
				{
					if ( sprite.vy_ <= 0 )
					{
						stopSpriteAtSlopeBottom( sprite, block );
					}
				}
			}
		};

	private:
		void stopSpriteAtSlopeBottom( Sprite& sprite, const Block& block ) const
		{
			const int slope_bottom = getSlopeBottomFromX( sprite, block );
			if ( sprite.hit_box_.y <= slope_bottom )
			{
				sprite.hit_box_.y = slope_bottom;
				sprite.collideStopYTop( 0 );
			}
		};

		int getSlopeBottomFromX( const Sprite& sprite, const Block& block ) const
		{
			return
				block.hit_box_.y +
				getYBaselineFromX( getMirrorOfRelativeHeight(), slope_width, false ) +
				getSlopeYChangeFromX<direction, slope_width, relative_height, false> ( sprite, block );
		};

		int getMirrorOfRelativeHeight() const
		{
			return slope_width - relative_height + 1;
		}
};

template<Direction::Horizontal direction, int relative_height>
std::unique_ptr<BlockComponent> generateBlockComponentMediumSlope()
{
	return std::unique_ptr<BlockComponent> ( new BlockComponentSlope<direction, 2, 3, relative_height, 8, false> () );
};

template<Direction::Horizontal direction>
std::unique_ptr<BlockComponent> generateBlockComponentSteepSlope()
{
	return std::unique_ptr<BlockComponent> ( new BlockComponentSlope<direction, 1, 8, 1, 16, false> () );
};

template<Direction::Horizontal direction, int relative_height>
std::unique_ptr<BlockComponent> generateBlockComponentShallowSlope()
{
	return std::unique_ptr<BlockComponent> ( new BlockComponentSlope<direction, 4, 1, relative_height, 4, false> () );
};

template<Direction::Horizontal direction, int relative>
std::unique_ptr<BlockComponent> generateBlockComponentVerySteepSlope()
{
	return std::unique_ptr<BlockComponent> ( new BlockComponentSlope<direction, 2, 8, relative, 16, true> () );
};

inline std::unique_ptr<BlockComponent> generateBlockComponentMediumSlopeLeftLow()
{
	return generateBlockComponentMediumSlope<Direction::Horizontal::LEFT, 1>();
};

inline std::unique_ptr<BlockComponent> generateBlockComponentMediumSlopeLeftHigh()
{
	return generateBlockComponentMediumSlope<Direction::Horizontal::LEFT, 2>();
};

inline std::unique_ptr<BlockComponent> generateBlockComponentMediumSlopeRightLow()
{
	return generateBlockComponentMediumSlope<Direction::Horizontal::RIGHT, 1>();
};

inline std::unique_ptr<BlockComponent> generateBlockComponentMediumSlopeRightHigh()
{
	return generateBlockComponentMediumSlope<Direction::Horizontal::RIGHT, 2>();
};

inline std::unique_ptr<BlockComponent> generateBlockComponentSteepSlopeLeft()
{
	return generateBlockComponentSteepSlope<Direction::Horizontal::LEFT>();
};

inline std::unique_ptr<BlockComponent> generateBlockComponentSteepSlopeRight()
{
	return generateBlockComponentSteepSlope<Direction::Horizontal::RIGHT>();
};

inline std::unique_ptr<BlockComponent> generateBlockComponentShallowSlopeLeftLowest()
{
	return generateBlockComponentShallowSlope<Direction::Horizontal::LEFT, 1>();
};

inline std::unique_ptr<BlockComponent> generateBlockComponentShallowSlopeLeftLower()
{
	return generateBlockComponentShallowSlope<Direction::Horizontal::LEFT, 2>();
};

inline std::unique_ptr<BlockComponent> generateBlockComponentShallowSlopeLeftHigher()
{
	return generateBlockComponentShallowSlope<Direction::Horizontal::LEFT, 3>();
};

inline std::unique_ptr<BlockComponent> generateBlockComponentShallowSlopeLeftHighest()
{
	return generateBlockComponentShallowSlope<Direction::Horizontal::LEFT, 4>();
};

inline std::unique_ptr<BlockComponent> generateBlockComponentShallowSlopeRightLowest()
{
	return generateBlockComponentShallowSlope<Direction::Horizontal::RIGHT, 1>();
};

inline std::unique_ptr<BlockComponent> generateBlockComponentShallowSlopeRightLower()
{
	return generateBlockComponentShallowSlope<Direction::Horizontal::RIGHT, 2>();
};

inline std::unique_ptr<BlockComponent> generateBlockComponentShallowSlopeRightHigher()
{
	return generateBlockComponentShallowSlope<Direction::Horizontal::RIGHT, 3>();
};

inline std::unique_ptr<BlockComponent> generateBlockComponentShallowSlopeRightHighest()
{
	return generateBlockComponentShallowSlope<Direction::Horizontal::RIGHT, 4>();
};

inline std::unique_ptr<BlockComponent> generateBlockComponentVerySteepSlopeLeftLow()
{
	return generateBlockComponentVerySteepSlope<Direction::Horizontal::LEFT, 1>();
};

inline std::unique_ptr<BlockComponent> generateBlockComponentVerySteepSlopeLeftHigh()
{
	return generateBlockComponentVerySteepSlope<Direction::Horizontal::LEFT, 2>();
};

inline std::unique_ptr<BlockComponent> generateBlockComponentVerySteepSlopeRightLow()
{
	return generateBlockComponentVerySteepSlope<Direction::Horizontal::RIGHT, 1>();
};

inline std::unique_ptr<BlockComponent> generateBlockComponentVerySteepSlopeRightHigh()
{
	return generateBlockComponentVerySteepSlope<Direction::Horizontal::RIGHT, 2>();
};

inline std::unique_ptr<BlockComponent> generateBlockComponentCeilingSteepSlopeLeft()
{
	return std::unique_ptr<BlockComponent> ( new BlockComponentCeilingSlope<Direction::Horizontal::LEFT, 1, 1> () );
};

inline std::unique_ptr<BlockComponent> generateBlockComponentCeilingSteepSlopeRight()
{
	return std::unique_ptr<BlockComponent> ( new BlockComponentCeilingSlope<Direction::Horizontal::RIGHT, 1, 1> () );
};

inline std::unique_ptr<BlockComponent> generateBlockComponentCeilingMediumSlopeLeftLow()
{
	return std::unique_ptr<BlockComponent> ( new BlockComponentCeilingSlope<Direction::Horizontal::LEFT, 2, 1> () );
};

inline std::unique_ptr<BlockComponent> generateBlockComponentCeilingMediumSlopeLeftHigh()
{
	return std::unique_ptr<BlockComponent> ( new BlockComponentCeilingSlope<Direction::Horizontal::LEFT, 2, 2> () );
};

inline std::unique_ptr<BlockComponent> generateBlockComponentCeilingMediumSlopeRightLow()
{
	return std::unique_ptr<BlockComponent> ( new BlockComponentCeilingSlope<Direction::Horizontal::RIGHT, 2, 1> () );
};

inline std::unique_ptr<BlockComponent> generateBlockComponentCeilingMediumSlopeRightHigh()
{
	return std::unique_ptr<BlockComponent> ( new BlockComponentCeilingSlope<Direction::Horizontal::RIGHT, 2, 2> () );
};

inline std::unique_ptr<BlockComponent> generateBlockComponentCeilingShallowSlopeLeftLowest()
{
	return std::unique_ptr<BlockComponent> ( new BlockComponentCeilingSlope<Direction::Horizontal::LEFT, 4, 1> () );
};

inline std::unique_ptr<BlockComponent> generateBlockComponentCeilingShallowSlopeLeftLower()
{
	return std::unique_ptr<BlockComponent> ( new BlockComponentCeilingSlope<Direction::Horizontal::LEFT, 4, 2> () );
};

inline std::unique_ptr<BlockComponent> generateBlockComponentCeilingShallowSlopeLeftHigher()
{
	return std::unique_ptr<BlockComponent> ( new BlockComponentCeilingSlope<Direction::Horizontal::LEFT, 4, 3> () );
};

inline std::unique_ptr<BlockComponent> generateBlockComponentCeilingShallowSlopeLeftHighest()
{
	return std::unique_ptr<BlockComponent> ( new BlockComponentCeilingSlope<Direction::Horizontal::LEFT, 4, 4> () );
};

inline std::unique_ptr<BlockComponent> generateBlockComponentCeilingShallowSlopeRightLowest()
{
	return std::unique_ptr<BlockComponent> ( new BlockComponentCeilingSlope<Direction::Horizontal::RIGHT, 4, 1> () );
};

inline std::unique_ptr<BlockComponent> generateBlockComponentCeilingShallowSlopeRightLower()
{
	return std::unique_ptr<BlockComponent> ( new BlockComponentCeilingSlope<Direction::Horizontal::RIGHT, 4, 2> () );
};

inline std::unique_ptr<BlockComponent> generateBlockComponentCeilingShallowSlopeRightHigher()
{
	return std::unique_ptr<BlockComponent> ( new BlockComponentCeilingSlope<Direction::Horizontal::RIGHT, 4, 3> () );
};

inline std::unique_ptr<BlockComponent> generateBlockComponentCeilingShallowSlopeRightHighest()
{
	return std::unique_ptr<BlockComponent> ( new BlockComponentCeilingSlope<Direction::Horizontal::RIGHT, 4, 4> () );
};
