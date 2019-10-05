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
	int top_speed,
	DuckData duck_data,
	UnDuckData unduck_data
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
	door_lock_ ( true ),
	duck_data_ ( duck_data ),
	unduck_data_ ( unduck_data )
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
	if ( !events.pause_hero_ )
	{
		heroActions( camera, lvmap, events, sprites, blocks, health );
		player_gfx_.update( *this, graphics_.get(), &events );
	}
};

// Actions only performed by hero version.
void PlayerSprite::heroActions( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	resetBopsOnLanding();
	actions( blocks, events );
	handleLookingUp();
	handleCameraMovement( camera );
	handleWaterEnteringAndExiting( lvmap );
	handleDoorBehavior( events );
	handleChasmBehavior( lvmap, events );
	handleDrowningBehavior( health );
	invincibilityFlicker( health );
	boundaries( camera, lvmap );
	camera.adjust( *this, lvmap );
	events.is_sliding_prev_ = events.is_sliding_;
	events.is_sliding_ = false;
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
				if ( movementType() != SpriteMovement::Type::SWIMMING )
				{
					Inventory::bop();
				}
				Audio::playSound( Audio::SoundType::BOP );
			}
			else if ( my_collision.collideAny() && events.is_sliding_prev_ )
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

void PlayerSprite::handleWaterEnteringAndExiting( const Map& lvmap )
{
	if ( lvmap.watery_ )
	{
		if ( !hasMovementType( SpriteMovement::Type::SWIMMING ) )
		{
			changeMovement( SpriteMovement::Type::SWIMMING );
		}
	}
	else
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
	is_looking_up_ = ( onGround() && input_->up() );
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
		tryUnduck( blocks );
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

void PlayerSprite::handleDuckingAndSliding( const BlockSystem& blocks, EventSystem& events )
{
	if ( input_->down() )
	{
		switch ( on_slope_ )
		{
			case ( Direction::Horizontal::__NULL ):
			{
				if ( events.can_climb_down_ )
				{
					if ( isDucking() )
					{
						tryUnduck( blocks );
					}
					on_ladder_ = true;
				}
				else
				{
					duck( duck_data_ );
				}
			}
			break;
			case ( Direction::Horizontal::LEFT ):
			{
				if ( isDucking() )
				{
					tryUnduck( blocks );
				}
				slideLeft( events );
			}
			break;
			case ( Direction::Horizontal::RIGHT ):
			{
				if ( isDucking() )
				{
					tryUnduck( blocks );
				}
				slideRight( events );
			}
			break;
		}
	}
	else
	{
		tryUnduck( blocks );
	}

	if ( !events.is_sliding_ )
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

	events.can_climb_down_ = false;
};

void PlayerSprite::handleJumpingAndFalling( const BlockSystem& blocks, const EventSystem& events )
{
	if ( input_->action1() && !( events.on_conveyor_belt_ && isDucking() && blocksJustAbove( blocks ) ) )
	{
		jump();
		if ( jump_start_ && !jump_end_ )
		{
			on_ladder_ = false;
			if ( events.is_sliding_ )
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

void PlayerSprite::tryUnduck( const BlockSystem& blocks )
{
	if ( !blocksJustAbove( blocks ) )
	{
		unduck( unduck_data_ );
	}
};

void PlayerSprite::deathAction( const Camera& camera, EventSystem& events, const Map& lvmap )
{
	graphics_->current_frame_x_ = 16;
	graphics_->current_frame_y_ = 26;
	unduck( unduck_data_ );
	graphics_->priority_ = true;
	defaultDeathAction( camera );
	events.playDeathSoundIfNotAlreadyPlaying();
};

bool PlayerSprite::canJump() const
{
	return ( onGroundPadding() || on_ladder_ ) && !jump_lock_;
};

void PlayerSprite::slideLeft( EventSystem& events )
{
	events.is_sliding_ = true;
	top_speed_ = top_speed_run_;
	start_speed_ = start_speed_run_;
	moveLeft();
};

void PlayerSprite::slideRight( EventSystem& events )
{
	events.is_sliding_ = true;
	top_speed_ = top_speed_run_;
	start_speed_ = start_speed_run_;
	moveRight();
};

bool PlayerSprite::isLookingUp() const
{
	return is_looking_up_;
};
