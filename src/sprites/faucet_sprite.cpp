#include "collision.hpp"
#include "event_system.hpp"
#include "faucet_sprite.hpp"
#include "level_state.hpp"
#include "sprite_graphics.hpp"

static constexpr int NUM_O_HITS = 3;
static constexpr int INVINCIBILITY_FLICKER_SPEED = 4;
static constexpr int INVINCIBILITY_TIME = 45;
static constexpr int SLIDE_LOCK_TIME = 5;

FaucetSprite::FaucetSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/faucet.png", 256, 0 ), x, y, 16, 16, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT, false, false, true, 0.0 ),
	gfx_component_ (),
	hits_ ( 0 ),
	invincibility_ ( 0 ),
	slide_lock_ ( 0 )
{};

FaucetSprite::~FaucetSprite() {};

void FaucetSprite::customUpdate( LevelState& level_state )
{
	if ( hits_ >= NUM_O_HITS && invincibility_ <= 0 )
	{
		level_state.events().win();
	}
	else if ( invincibility_ > 0 )
	{
		--invincibility_;
	}

	graphics_->current_frame_y_ = hits_;

	if ( invincibility_ % INVINCIBILITY_FLICKER_SPEED == 1 ) // 1 chosen 'cause least likely for invincibility to end with 1 as remainder.
	{
		graphics_->visible_ = false;
	}
	else
	{
		graphics_->visible_ = true;
	}
	gfx_component_.update( *this, graphics_.get() );
};

void FaucetSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( their_collision.collideAny() )
	{
		them.collideStopAny( their_collision );

		if ( them.hasType( SpriteType::HERO ) )
		{
			if ( level_state.events().testIsSlidingPreviously() )
			{
				if ( slide_lock_ <= 0 && invincibility_ <= 0 )
				{
					++hits_;
					invincibility_ = INVINCIBILITY_TIME;
				}
				slide_lock_ = SLIDE_LOCK_TIME;
			}

			if ( slide_lock_ > 0 )
			{
				slide_lock_ = SLIDE_LOCK_TIME;
			}
		}
	}
	else if ( them.hasType( SpriteType::HERO ) )
	{
		--slide_lock_;
	}
};

void FaucetSprite::render( Camera& camera, bool priority )
{
	gfx_component_.render( graphics_.get(), &camera, priority, xPixels(), yPixels() );
};

int FaucetSprite::getHits() const
{
	return hits_;
};
