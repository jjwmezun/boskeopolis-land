#include "audio.hpp"
#include "block.hpp"
#include "block_system.hpp"
#include "camera.hpp"
#include "collision.hpp"
#include "main.hpp"
#include "event_system.hpp"
#include <fstream>
#include "health.hpp"
#include "input.hpp"
#include "input_component_player.hpp"
#include "input_component_sequence.hpp"
#include "inventory_level.hpp"
#include "inventory.hpp"
#include "level.hpp"
#include "map.hpp"
#include "player_sprite.hpp"

PlayerSprite::PlayerSprite
(
	int x,
	int y,
	int jump_start_speed,
	int jump_top_speed,
	std::unique_ptr<InputComponent> input,
	std::unique_ptr<SpriteGraphics>&& gfx,
	SpriteType type,
	int start_speed,
	int top_speed
)
:
	Sprite
	(
		std::move( gfx ),
		x,
		y,
		14,
		23,
		{ type },
		start_speed,
		top_speed,
		jump_start_speed,
		jump_top_speed,
		Direction::Horizontal::RIGHT,
		Direction::Vertical::__NULL,
		nullptr,
		SpriteMovement::Type::GROUNDED,
		CameraMovement::PERMANENT,
		false,
		true,
		true,
		false,
		.2
	),
	input_ ( std::move( input ) ),
	door_lock_ ( true )
{
	// Be absolutely sure input component is set.
	if ( input_ == nullptr )
	{
		input_.reset( new InputComponentPlayer() );
	}
};

PlayerSprite::~PlayerSprite() {};

void PlayerSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	heroActions( camera, lvmap, events, sprites, blocks, health );
	player_gfx_.update( *this, graphics_.get() );
};

// Actions only performed by hero version.
void PlayerSprite::heroActions( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	resetBopsOnLanding();
	actions( blocks, events );
	handleLookingUp();
	handleCameraMovement( camera );
	handleWaterEnteringAndExiting();
	handleDoorBehavior( events );
	handleChasmBehavior( lvmap, events );
	handleDrowningBehavior( health );
	invincibilityFlicker( health );
	boundaries( camera, lvmap );
	camera.adjust( *this, lvmap );
};

// Actions performed by all player sprites, including Dagny.
void PlayerSprite::actions( const BlockSystem& blocks, EventSystem& events )
{
	handleRunning();
	handleWalking();
	handleDuckingAndSliding( blocks, events );
	handleJumpingAndFalling( blocks, events );
	events.on_conveyor_belt_ = false;
	handleLadderBehavior( events );
	adjustJumpSpeed();
	dontDuckWhileSwimming( blocks );
	input_->update();
};

void PlayerSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( them.hasType( SpriteType::ENEMY ) )
	{
		if ( them.hasType( SpriteType::BOPPABLE ) )
		{
			if ( collideBottomOnly( my_collision, them ) )
			{
				them.kill();
				bounce();
				Inventory::bop();
				Audio::playSound( Audio::SoundType::BOP );
			}
			else if ( my_collision.collideAny() && isSlidingPrev() )
			{
				them.kill();
				Audio::playSound( Audio::SoundType::BOP );
			}
			else if ( my_collision.collideAny() && !them.isDead() )
			{
				health.hurt();
			}
		}
		else
		{
			if ( my_collision.collideAny() && !them.isDead() )
			{
				health.hurt();
			}
		}
	}
};

void PlayerSprite::handleCameraMovement( Camera& camera )
{
	if ( input_->cLeft() )
	{
		camera.moveLeft();
	}
	if ( input_->cRight() )
	{
		camera.moveRight();
	}
	if ( input_->cUp() )
	{
		camera.moveUp();
	}
	if ( input_->cDown() )
	{
		camera.moveDown();
	}
};

void PlayerSprite::handleDoorBehavior( EventSystem& events )
{
	if ( events.in_front_of_door_ > 0 && input_->up() && !door_lock_ && on_ground_ )
	{
		switch ( events.in_front_of_door_ )
		{
			case ( 1 ):
				events.changeMap();
			break;
			case ( 2 ):
				events.changeMapSewer();
				graphics_->priority_ = true;
				vx_ = 0;
				acceleration_x_ = 0;
				Audio::playSound( Audio::SoundType::SEWER_HOLE );
			break;
		}
	}
	else if ( input_->up() )
	{
		door_lock_ = true;
	}
	events.in_front_of_door_ = 0;

	if ( door_lock_ )
	{
		if ( !input_->up() )
		{
			door_lock_ = false;
		}
	}
};

void PlayerSprite::handleChasmBehavior( const Map& lvmap, EventSystem& events )
{
	if ( fellInBottomlessPit( lvmap ) )
	{
		if ( lvmap.warp_on_fall_ )
		{
			events.changeMap();
		}
		else
		{
			kill();
		}
	}
};

void PlayerSprite::handleDrowningBehavior( const Health& health )
{
	if ( health.drowned() )
	{
		kill();
	}
}

void PlayerSprite::handleWaterEnteringAndExiting()
{
	if ( !is_dead_ && in_water_ )
	{
		changeMovement( SpriteMovement::Type::SWIMMING );
		in_water_ = false;
	}
	else if ( hasMovementType( SpriteMovement::Type::SWIMMING ) )
	{
		changeMovement( SpriteMovement::Type::GROUNDED );

		if ( input_->action1() )
		{
			bounce();
			Audio::playSound( Audio::SoundType::JUMP );
		}
	}
};

void PlayerSprite::duck()
{
	// Can continue ducking while in air, but only start duck on ground.
	if ( isDucking() || onGround() )
	{
		// Hacky way to make player warp to the right position after height changes.
		if ( !isDucking() )
		{
			hit_box_.y += Unit::PixelsToSubPixels( 9 );
			graphics_->y_adjustment_ = -6;
			graphics_->h_adjustment_ = 8;
		}

		is_ducking_ = true;

		hit_box_.h = original_hit_box_.h - Unit::PixelsToSubPixels( 11 );
	}
};

void PlayerSprite::resetBopsOnLanding() const
{
	if ( on_ground_ )
	{
		Inventory::clearBops();
	}
}

void PlayerSprite::handleLadderBehavior( EventSystem& events )
{
	if ( input_->up() )
	{
		if ( events.touching_ladder_ )
		{
			grabLadder();
		}

		if ( on_ladder_ && hasMovementType( SpriteMovement::Type::GROUNDED ) )
		{
			if ( isJumping() )
			{
				acceleration_y_ = -LADDER_SPEED * 2;
			}
			else
			{
				acceleration_y_ = -LADDER_SPEED;
			}
		}
	}
	else if ( on_ladder_ )
	{
		if ( input_->down() )
		{
			acceleration_y_ = LADDER_SPEED;
		}
		else
		{
			stopY();
		}
	}

	if ( !events.touching_ladder_ )
	{
		releaseLadder();
	}

	if ( on_ladder_ )
	{
		vy_ = 0;
	}
	events.touching_ladder_prev_ = events.touching_ladder_;
	events.touching_ladder_ = false;
};

void PlayerSprite::handleLookingUp()
{
	if ( onGround() && input_->up() )
	{
		lookUp();
	}
	else
	{
		stopLookingUp();
	}
}

void PlayerSprite::adjustJumpSpeed()
{
	if ( isDucking() )
	{
		jump_top_speed_ = JUMP_DUCK_TOP_SPEED;
	}
	else
	{
		jump_top_speed_ = jump_top_speed_normal_;
	}

	if ( abs( vx_ ) > top_speed_walk_ )
	{
		jump_top_speed_ *= 1.1;
	}
};

void PlayerSprite::dontDuckWhileSwimming( const BlockSystem& blocks )
{
	if ( is_ducking_ && !on_ground_ && hasMovementType( SpriteMovement::Type::SWIMMING ) )
	{
		unduck( blocks );
	}
}

void PlayerSprite::handleRunning()
{
	if ( input_->action2() )
	{
		run();
	}
	else
	{
		stopRunning();
	}
};

void PlayerSprite::handleWalking()
{
	if ( input_->left() && !input_->right() && ( !isDucking() || !onGround() ) )
	{
		moveLeft();
	}

	if ( input_->right() && !input_->left() && ( !isDucking() || !onGround() ) )
	{
		moveRight();
	}
};

void PlayerSprite::handleDuckingAndSliding( const BlockSystem& blocks, const EventSystem& events )
{
	if ( input_->down() )
	{
		switch ( on_slope_ )
		{
			case ( Direction::Horizontal::__NULL ):
			{
				if ( events.can_climb_down_ )
				{
					on_ladder_ = true;
				}
				else
				{
					duck();
				}
			}
			break;
			case ( Direction::Horizontal::LEFT ):
			{
				if ( isDucking() )
				{
					unduck( blocks );
				}
				slideLeft();
			}
			break;
			case ( Direction::Horizontal::RIGHT ):
			{
				if ( isDucking() )
				{
					unduck( blocks );
				}
				slideRight();
			}
			break;
		}
	}
	else
	{
		unduck( blocks );
	}

	if ( !is_sliding_ )
	{
		if ( !input_->right() && !input_->left())
		{
			if ( isDucking() )
			{
				stopDucking();
			}
			else
			{
				stopX();
			}
		}
	}

	if ( slide_jump_ )
	{
		top_speed_ = top_speed_run_;
	}
};

void PlayerSprite::handleJumpingAndFalling( const BlockSystem& blocks, const EventSystem& events )
{
	if ( input_->action1() && !( events.on_conveyor_belt_ && isDucking() && blocksJustAbove( blocks ) ) )
	{
		jump();

		if ( jump_start_ && !jump_end_ )
		{
			on_ladder_ = false;

			if ( is_sliding_ )
			{
				slide_jump_ = true;
				vx_ *= 5;
			}

			if ( hasMovementType( SpriteMovement::Type::GROUNDED ) && ( on_ground_prev_ || onGroundPadding() ) )
			{
				Audio::playSound( Audio::SoundType::JUMP );
			}
		}

		if ( hasMovementType( SpriteMovement::Type::SWIMMING ) && !jump_lock_ )
		{
			Audio::playSound( Audio::SoundType::SWIM );
		}

		jump_lock_ = true;

		slowFall();

		if ( is_bouncing_ )
		{
			bounce();
		}
	}
	else
	{
		fastFall();

		if ( is_bouncing_prev_ )
		{
			is_bouncing_ = false;
		}
		jump_lock_ = false;
	}
};

void PlayerSprite::forceUnduck()
{
	// Hacky way to keep player from falling through ground after gaining height from unducking.
	if ( isDucking() )
	{
		hit_box_.y -= Unit::PixelsToSubPixels( 10 );
		graphics_->y_adjustment_ = -2;
		graphics_->h_adjustment_ = 3;
	}

	is_ducking_ = false;
	hit_box_.h = original_hit_box_.h - Unit::PixelsToSubPixels( 1 );
};

void PlayerSprite::unduck( const BlockSystem& blocks )
{
	if ( !blocksJustAbove( blocks ) )
	{
		forceUnduck();
	}
};

void PlayerSprite::deathAction( Camera& camera, EventSystem& events )
{
	graphics_->current_frame_x_ = 16;
	graphics_->current_frame_y_ = 26;
	forceUnduck();
	graphics_->priority_ = true;
	defaultDeathAction( camera );
	events.playDeathSoundIfNotAlreadyPlaying();
};

bool PlayerSprite::canJump() const
{
	return ( onGroundPadding() || on_ladder_ ) && !jump_lock_;
};
