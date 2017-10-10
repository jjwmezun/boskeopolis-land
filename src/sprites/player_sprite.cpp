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
#include <iostream>
#include "level.hpp"
#include "map.hpp"
#include "player_sprite.hpp"
#include "racer_sprite.hpp"

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
	if ( input_ == nullptr )
	{
		input_.reset( new InputComponentPlayer() );
	}
};

PlayerSprite::~PlayerSprite() {};

void PlayerSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	heroActions( camera, lvmap, events, sprites, blocks, health );
};

void PlayerSprite::heroActions( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	if ( on_ground_ )
	{
		Inventory::clearBops();
	}

	actions( blocks, events );

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
		}
	}

	if ( events.in_front_of_door_ && input_->up() && !door_lock_ && on_ground_ )
	{
		events.changeMap();
	}
	else if ( input_->up() )
	{
		door_lock_ = true;
	}
	events.in_front_of_door_ = false;

	if ( door_lock_ )
	{
		if ( !input_->up() )
		{
			door_lock_ = false;
		}
	}

	if ( fellInBottomlessPit( lvmap ) )
	{
		kill();
	}
	else if ( health.drowned() )
	{
		kill();
	}

	invincibilityFlicker( health );

	boundaries( camera, lvmap );
	camera.adjust( *this, lvmap );
};

void PlayerSprite::actions( const BlockSystem& blocks, EventSystem& events )
{
	if ( input_->action2() )
	{
		run();
	}
	else
	{
		stopRunning();
	}

	if ( input_->left() && !input_->right() && ( !isDucking() || !onGround() ) )
	{
		moveLeft();
	}

	if ( input_->right() && !input_->left() && ( !isDucking() || !onGround() ) )
	{
		moveRight();
	}

	if ( input_->down() )
	{
		switch ( on_slope_ )
		{
			case ( Direction::Horizontal::LEFT ):
				if ( isDucking() ) unduck( blocks );
				slideLeft();
			break;
			case ( Direction::Horizontal::RIGHT ):
				if ( isDucking() ) unduck( blocks );
				slideRight();
			break;
			case ( Direction::Horizontal::__NULL ):
				duck();
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
				stopDucking();
			else
				stopX();
		}
	}

	if ( slide_jump_ )
	{
		top_speed_ = top_speed_run_;
	}

	
	const sdl2::SDLRect just_above = { hit_box_.x, hit_box_.y - 16000, hit_box_.w, 20000 };

	if ( input_->action1() && !( events.on_conveyor_belt_ && isDucking() && blocks.blocksInTheWay( just_above, BlockComponent::Type::SOLID ) ) )
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
	events.on_conveyor_belt_ = false;

	if ( input_->up() )
	{
		if ( touching_ladder_ )
		{
			grabLadder();
		}
	}

	if ( Input::released( Input::Action::MOVE_UP ) )
	{
		stopLookingUp();
	}

	if ( input_->up() )
	{
		moveUp();
		
		if ( hasMovementType( SpriteMovement::Type::GROUNDED ) )
		{
			if ( onLadder() )
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
			else if ( onGround() )
			{
				lookUp();
			}
		}
	}
	else if ( input_->down() )
	{
		moveDown();

		if ( onLadder() )
		{
			acceleration_y_ = LADDER_SPEED;
		}
	}
	else
	{
		if ( onLadder() )
		{
			stopY();
		}
	}

	/*
	// In case player gets stuck 'tween 2 blocks with just 1 empty block 'tween them vertically.
	// Makes player automatically move to the side till out o' space ala Super Mario Bros. 3.
	if ( collide_top_prev_ && collide_bottom_prev_ && !isDucking() )
	{
		if ( directionX() == Direction::Horizontal::RIGHT )
			vx_ = 1000;
		else
			vx_ = -1000;

		stopX();
	}*/

	if ( !touching_ladder_ )
		releaseLadder();

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

	input_->update();

};

void PlayerSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health )
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
			}
			else if ( my_collision.collideAny() && isSlidingPrev() )
			{
				them.kill();
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

void PlayerSprite::unduck( const BlockSystem& blocks )
{
	const sdl2::SDLRect just_above = { hit_box_.x, hit_box_.y - 16000, hit_box_.w, 20000 };

	if ( !blocks.blocksInTheWay( just_above, BlockComponent::Type::SOLID ) )
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
	}
};

void PlayerSprite::deathAction( Camera& camera )
{
	defaultDeathAction( camera );
};

void PlayerSprite::testVX()
{
	std::cout<<vx_<<std::endl;
};

void PlayerSprite::testFrameRate()
{
	std::cout<<Main::frame()<<std::endl;
};