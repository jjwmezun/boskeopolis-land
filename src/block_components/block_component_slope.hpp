#pragma once

#include "block.hpp"
#include "block_component.hpp"
#include "collision.hpp"
#include "direction.hpp"
#include <memory>
#include "sprite.hpp"

constexpr int getYBaselineFromX( int relative_height, int slope_width )
{
	// Bottomost pixel o' lowest slope is 1, so we always start with 1000 subpixels.
	//
	// Since whole slope is 1 block high, but a fraction o' slope width wide,
	// add a fraction o' a block's y pixels by slope width for every slope block
	// beyond the 1st.
	return 1000 + ( ( int )( floor( Unit::SUBPIXELS_PER_BLOCK / slope_width ) ) * ( relative_height - 1 ) );
};

inline bool testSpritePositionWithinBlocksXThreshold( const Sprite& sprite, const Block& block )
{
	return sprite.centerXSubPixels() >= block.leftSubPixels() && sprite.centerXSubPixels() <= block.rightSubPixels();
};

template <Direction::Horizontal direction, int slope_width, int extra_push, int relative_height, int resistance_percentage>
class BlockComponentSlope : public BlockComponent
{
	public:
		BlockComponentSlope() {};
		~BlockComponentSlope() {};
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites ) const override
		{
			if ( collision.collideAny() )
			{
				sprite.on_slope_ = direction;
				if ( testSpritePositionWithinBlocksXThreshold( sprite, block ) )
				{
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
				getYBaselineFromX( relative_height, slope_width ) -
				( getYAdjustFromSpritesRelativeXPosition( sprite, block ) / slope_width ) -
				sprite.heightSubPixels();
		}

		int getYAdjustFromSpritesRelativeXPosition( const Sprite& sprite, const Block& block ) const
		{
			// For left slope, calculate # o' pixels left o' left end;
			// for right slope, calculate # o' pixels from right end.
			return ( direction == Direction::Horizontal::LEFT )
				? sprite.centerXSubPixels() - block.leftSubPixels()
				: block.rightSubPixels() - sprite.centerXSubPixels();
		};

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

template<Direction::Horizontal direction, int relative_height>
std::unique_ptr<BlockComponent> generateBlockComponentMediumSlope()
{
	return std::unique_ptr<BlockComponent> ( new BlockComponentSlope<direction, 2, 3, relative_height, 8> () );
};

template<Direction::Horizontal direction>
std::unique_ptr<BlockComponent> generateBlockComponentSteepSlope()
{
	return std::unique_ptr<BlockComponent> ( new BlockComponentSlope<direction, 1, 8, 1, 16> () );
};

template<Direction::Horizontal direction, int relative_height>
std::unique_ptr<BlockComponent> generateBlockComponentShallowSlope()
{
	return std::unique_ptr<BlockComponent> ( new BlockComponentSlope<direction, 4, 1, relative_height, 4> () );
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
