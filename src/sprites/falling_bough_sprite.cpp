#include "block_system.hpp"
#include "camera.hpp"
#include "collision.hpp"
#include "falling_bough_sprite.hpp"
#include "level_state.hpp"
#include "sprite_graphics.hpp"
#include "sprite_system.hpp"

FallingBoughSprite::FallingBoughSprite( int x, int y, Direction::Horizontal direction, bool fall )
:
	Sprite( std::make_unique<SpriteGraphics> ( "tilesets/woods.png", 104, 32, ( direction == Direction::Horizontal::RIGHT ) ), ( direction == Direction::Horizontal::RIGHT ) ? x : x - 8, y, 24, 8, { SpriteType::FALLING_BOUGH }, 10, 1400, 0, 0, direction, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY, false, false ),
	fall_ ( fall ),
	falling_timer_ ()
{};
FallingBoughSprite::~FallingBoughSprite() {};

void FallingBoughSprite::customUpdate( LevelState& level_state )
{
	if ( falling_timer_.on() )
	{
		falling_timer_.update();
	}
	if ( falling_timer_.done() )
	{
		changeMovement( SpriteMovement::Type::GROUNDED );
	}
	hit_box_.x = original_hit_box_.x;
};

void FallingBoughSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( them.hasType( Sprite::SpriteType::HERO ) )
	{
		const Collision collision = them.testBlockCollision( *this );
		if ( collision.collideBottom() && them.prevBottomSubPixels() <= topSubPixels() + 1000  )
		{
			them.collideStopYBottom( collision.overlapYBottom() );

			// Keep player on platform ( so it doesn’t fall under player & cause them to be unable to jump )
			// but only if not colliding with any solid blocks or other sprites, so it doesn’t force
			// player thru other sprites.
			if ( !level_state.blocks().blocksInTheWay( hit_box_, BlockComponent::Type::SOLID ) )
			{
				bool collide = false;
				for ( auto& sprite : level_state.sprites().getSpritesList() )
				{
					// Skip self.
					if ( sprite.get() == this )
					{
						continue;
					}

					const Collision scol = sprite->testCollision( *this );
					if ( scol.collideAny() )
					{
						collide = true;
						break;
					}
				}

				if ( !collide )
				{
					them.addToY( vy_ );
				}
			}

			if ( fall_ && !falling_timer_.on() )
			{
				falling_timer_.start();
			}
		}
	}
};

void FallingBoughSprite::reset()
{
	hit_box_.y = original_hit_box_.y;
	changeMovement( SpriteMovement::Type::FLOATING );
	vy_ = 0;
	acceleration_y_ = 0;
	falling_timer_.stop();
};

void FallingBoughSprite::render( const Camera& camera ) const
{
	if ( graphics_ != nullptr )
	{
		graphics_->render( Unit::SubPixelsToPixels( hit_box_ ), &camera );
	}
};
