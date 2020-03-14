#pragma once

#include "block_component.hpp"
#include "block_system.hpp"
#include "level_state.hpp"

template<Direction::Simple direction>
class BlockComponentCurrent final : public BlockComponent
{
	public:
		BlockComponentCurrent() {};
		~BlockComponentCurrent() {};

		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const override
		{
			switch( direction )
			{
				case( Direction::Simple::LEFT ):
				case( Direction::Simple::RIGHT ):
				{
					const int LEFT_TEST = ( direction == Direction::Simple::LEFT ) ? sprite.hit_box_.x - HIT_BOX_PADDING : sprite.rightSubPixels() + HIT_BOX_PADDING;
					if ( !level_state.blocks().blocksInTheWay( { LEFT_TEST, sprite.hit_box_.y + 2000, 2000, sprite.hit_box_.h - 4000 }, BlockComponent::Type::SOLID ) )
					{
						sprite.hit_box_.x -= ( direction == Direction::Simple::LEFT ) ? NORMAL_MOVEMENT : -NORMAL_MOVEMENT;

						if ( sprite.isDucking() )
						{
							// Kill the normal slide effect.
							sprite.vx_ = 0;
						}
						else if ( sprite.on_ground_prev_ || sprite.on_ground_ )
						{
							sprite.hit_box_.x -= ( direction == Direction::Simple::LEFT ) ? ON_GROUND_MOVEMENT : -ON_GROUND_MOVEMENT;
						}
						else if ( sprite.collide_bottom_prev_ || sprite.collide_bottom_ )
						{
							sprite.hit_box_.x -= ( direction == Direction::Simple::LEFT ) ? CEILING_MOVEMENT : -CEILING_MOVEMENT;
						}
					}
				}
				break;

				case( Direction::Simple::UP ):
				case( Direction::Simple::DOWN ):
				{
					const int TOP_TEST = ( direction == Direction::Simple::UP ) ? sprite.hit_box_.y - HIT_BOX_PADDING : sprite.bottomSubPixels() + HIT_BOX_PADDING;
					if ( !level_state.blocks().blocksInTheWay( { sprite.hit_box_.x + 2000, TOP_TEST, sprite.hit_box_.w - 4000, 2000 }, BlockComponent::Type::SOLID ) )
					{
						sprite.hit_box_.y -= ( direction == Direction::Simple::UP ) ? NORMAL_MOVEMENT : -NORMAL_MOVEMENT;
						sprite.vy_ = 0;
						if ( sprite.isDucking() )
						{
						}
						else if ( sprite.on_ground_prev_ || sprite.on_ground_ )
						{
							sprite.hit_box_.y -= ( direction == Direction::Simple::UP ) ? ON_GROUND_MOVEMENT : -ON_GROUND_MOVEMENT;
						}
						else if ( sprite.collide_bottom_prev_ || sprite.collide_bottom_ )
						{
							sprite.hit_box_.y -= ( direction == Direction::Simple::UP ) ? CEILING_MOVEMENT : -CEILING_MOVEMENT;
						}

						//sprite.hit_box_.y += ( direction == Direction::Simple::UP ) ? sprite.vy_ : -sprite.vy_;
					}
				}
				break;
			}
		};

	private:
		static constexpr int HIT_BOX_PADDING = 2000;
		static constexpr int NORMAL_MOVEMENT = 910;
		static constexpr int ON_GROUND_MOVEMENT = 75;
		static constexpr int CEILING_MOVEMENT = 1000;
};

inline std::unique_ptr<BlockComponent> generateBlockComponentCurrentUpward()
{
	return std::unique_ptr<BlockComponent> ( new BlockComponentCurrent<Direction::Simple::UP> () );
};

inline std::unique_ptr<BlockComponent> generateBlockComponentCurrentDownward()
{
	return std::unique_ptr<BlockComponent> ( new BlockComponentCurrent<Direction::Simple::DOWN> () );
};

inline std::unique_ptr<BlockComponent> generateBlockComponentCurrentLeftward()
{
	return std::unique_ptr<BlockComponent> ( new BlockComponentCurrent<Direction::Simple::LEFT> () );
};

inline std::unique_ptr<BlockComponent> generateBlockComponentCurrentRightward()
{
	return std::unique_ptr<BlockComponent> ( new BlockComponentCurrent<Direction::Simple::RIGHT> () );
};
