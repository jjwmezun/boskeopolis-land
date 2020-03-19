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
#include "level_state.hpp"
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
		( gfx == nullptr ) ? std::make_unique<SpriteGraphics> ( Inventory::getPlayerCostume(), 0, 0, false, false, 0, false, -1, -2, 2, 4 ) : std::move( gfx ),
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

void PlayerSprite::customUpdate( LevelState& level_state )
{
	if ( !level_state.events().testPauseHero() )
	{
		heroActions( level_state );
		player_gfx_.update( *this, graphics_.get(), level_state.health(), &level_state.events() );
	}
};

// Actions only performed by hero version.
void PlayerSprite::heroActions( LevelState& level_state )
{
	Camera& camera = level_state.camera();
	Map& lvmap = level_state.currentMap();
	Health& health = level_state.health();
	EventSystem& events = level_state.events();
	resetBopsOnLanding( level_state.inventory() );
	actions( level_state.blocks(), events );
	handleLookingUp();
	handleCameraMovement( camera );
	handleWaterEnteringAndExiting( lvmap );
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
	handleLadderBehavior( events );
	adjustJumpSpeed();
	dontDuckWhileSwimming( blocks );
	input_->update();
};

void PlayerSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	playerInteract( my_collision, them, level_state.health(), level_state.events(), level_state.inventory() );
};

void PlayerSprite::playerInteract( Collision& my_collision, Sprite& them, Health& health, EventSystem& events, InventoryLevel& inventory )
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
					inventory.bop();
				}
				Audio::playSound( Audio::SoundType::BOP );
			}
			else if ( my_collision.collideAny() && events.testIsSlidingPreviously() )
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
	if ( input_->up() && !door_lock_ && on_ground_ )
	{
		if ( events.isInFrontOfRegularDoor() )
		{
			events.changeMap();
		}
		else if ( events.isInFrontOfSewerDoor() )
		{
			events.changeMapSewer();
			graphics_->priority_ = true;
			vx_ = 0;
			acceleration_x_ = 0;
			Audio::playSound( Audio::SoundType::SEWER_HOLE );
		}
	}
	else if ( input_->up() )
	{
		door_lock_ = true;
	}
	events.resetInFrontOfDoor();

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

void PlayerSprite::resetBopsOnLanding( InventoryLevel& inventory ) const
{
	if ( on_ground_ )
	{
		inventory.clearBops();
	}
}

void PlayerSprite::handleLadderBehavior( EventSystem& events )
{
	if ( input_->up() )
	{
		if ( events.testTouchingLadder() )
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

	if ( !events.testTouchingLadder() )
	{
		releaseLadder();
	}

	if ( on_ladder_ )
	{
		vy_ = 0;
	}
	events.resetTouchingLadder();
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
				if ( events.testCanClimbDown() )
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

	if ( !events.testIsSliding() )
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

	events.resetCanClimbDown();
	events.resetIsSliding();
};

void PlayerSprite::handleJumpingAndFalling( const BlockSystem& blocks, EventSystem& events )
{
	if ( input_->action1() && !( events.testOnConveyorBelt() && isDucking() && blocksJustAbove( blocks ) ) )
	{
		jump();
		if ( jump_start_ && !jump_end_ )
		{
			on_ladder_ = false;
			if ( events.testIsSliding() )
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
	events.resetOnConveyorBelt();
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
	events.setSlidingOn();
	top_speed_ = top_speed_run_;
	start_speed_ = start_speed_run_;
	moveLeft();
};

void PlayerSprite::slideRight( EventSystem& events )
{
	events.setSlidingOn();
	top_speed_ = top_speed_run_;
	start_speed_ = start_speed_run_;
	moveRight();
};

bool PlayerSprite::isLookingUp() const
{
	return is_looking_up_;
};
