#include "audio.hpp"
#include "bullet_sprite.hpp"
#include "chamsby_sprite.hpp"
#include "collision.hpp"
#include "event_system.hpp"
#include "health.hpp"
#include "level_state.hpp"
#include "mezun_math.hpp"
#include "sprite_graphics.hpp"
#include "sprite_system.hpp"

static constexpr int HEALTH_PER_HIT = 4;
static constexpr int NUMBER_OF_HITS = 3;
static constexpr int MAX_HP = NUMBER_OF_HITS * HEALTH_PER_HIT;

ChamsbySprite::ChamsbySprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/chamsby.png", 0, 0, false, false, 0.0, -2, -1, 5, 1 ), x, y, 14, 25, {}, 500, 3000, 500, 4000, Direction::Horizontal::LEFT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::PERMANENT ),
	health_ ( 0 ),
	invincibility_ ( 0 ),
	walk_timer_ (),
	shoot_timer_ (),
	name ( "Lance Chamsby", 0, 0, Text::FontColor::DARK_GRAY ),
	health_timer_ ( 0 ),
	state_ ( ChamsbyState::INTRO ),
	head_box_ ( Unit::PixelsToSubPixels( sdl2::SDLRect{ x - 2, y, 18, 8 } ))
{};

ChamsbySprite::~ChamsbySprite() {};

void ChamsbySprite::customUpdate( LevelState& level_state )
{
	EventSystem& events = level_state.events();
	events.createBossUI();
	switch ( state_ )
	{
		case ( ChamsbyState::INTRO ):
		{
			health_ = MAX_HP;
			state_ = ChamsbyState::ATTACK;
			/*
			events.setPauseHeroOn();
			if ( health_timer_ >= 4 )
			{
				Audio::playSound( Audio::SoundType::SELECT );
				++health_;
				if ( health_ == MAX_HP )
				{
					events.setPauseHeroOff();
					state_ = ChamsbyState::ATTACK;
					Audio::changeSong( "boss" );
				}
				health_timer_ = 0;
			}
			else
			{
				++health_timer_;
			}
			events.changeBossUI( name, health_ );*/
		}
		break;

		case ( ChamsbyState::ATTACK ):
		{
			moveInDirectionX();
			if ( !on_ground_ || is_jumping_ )
			{
				jump();
			}
			else if ( walk_timer_.update() )
			{
				const bool is_high_jump = mezun::randBool();
				jump_start_speed_ = ( is_high_jump ) ? 1000 : 500;
				jump_top_speed_ = ( is_high_jump ) ? 6000 : 4000;
				jump();
			}
		}
		break;

		case ( ChamsbyState::HIT ):
		{
			acceleration_x_ = 0;
			switch ( direction_x_ )
			{
				case ( Direction::Horizontal::LEFT ):
				{
					vx_ = 200;
				}
				break;

				case ( Direction::Horizontal::RIGHT ):
				{
					vx_ = -200;
				}
				break;
			}

			graphics_->visible_ = ( invincibility_ % 4 != 1 );
			if ( invincibility_ > 0 )
			{
				--invincibility_;
			}
			else
			{
				if ( health_ <= 0 )
				{
					state_ = ChamsbyState::DEFEATED;
					events.setPauseHeroOn();
				}
				else
				{
					state_ = ChamsbyState::ATTACK;
				}
			}
		}
		break;

		case( ChamsbyState::DEFEATED ):
		{
			direction_x_ = Direction::Horizontal::RIGHT;
			moveRight();
			if ( xPixels() >= level_state.camera().right() )
			{
				printf( "GONE\n" );
			}
		}
		break;
	}
	flipGraphicsOnRight();
};

void ChamsbySprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	EventSystem& events = level_state.events();
	if ( them.hasType( SpriteType::HERO ) )
	{
		head_box_.x = hit_box_.x - Unit::PixelsToSubPixels( 2 );
		head_box_.y = hit_box_.y;

		Collision head_collision = them.testCollision( head_box_ );

		switch ( state_ )
		{
			case ( ChamsbyState::ATTACK ):
			{
				if ( them.rightSubPixels() < hit_box_.x )
				{
					direction_x_ = Direction::Horizontal::LEFT;
				}
				else if ( them.hit_box_.x > rightSubPixels() )
				{
					direction_x_ = Direction::Horizontal::RIGHT;
				}
				if ( shoot_timer_.update() && them.hit_box_.y < bottomSubPixels() && them.bottomSubPixels() > hit_box_.y )
				{
					level_state.sprites().spawn( std::make_unique<BulletSprite> ( centerXPixels(), centerYPixels(), Direction::horizontalToSimple( direction_x_ ), false ) );
				}

				if ( invincibility_ == 0 )
				{
					//if ( !them.on_ground_ && head_collision.collideAny() )
					if ( false )
					{
						them.bounce();
						Audio::playSound( Audio::SoundType::BOP );
						health_ -= HEALTH_PER_HIT;
						events.changeBossUI( name, health_ );
						invincibility_ = 48;
						state_ = ChamsbyState::HIT;
					}
					else if ( their_collision.collideAny() )
					{
						level_state.health().hurt();
					}
				}
			}
			break;
		}
	}
	else if ( them.hasType( SpriteType::ENEMY ) && invincibility_ == 0 && their_collision.collideAny() )
	{
		health_ -= HEALTH_PER_HIT;
		events.changeBossUI( name, health_ );
		invincibility_ = 48;
		state_ = ChamsbyState::HIT;
	}
};

void ChamsbySprite::render( const Camera& camera ) const
{
	graphics_->render( Unit::SubPixelsToPixels( hit_box_ ), &camera );
	auto r = camera.relativeRect( Unit::SubPixelsToPixels( hit_box_ ) );
	Render::renderRectDebug( r, { 255, 0, 0, 128 } );
	auto hr = camera.relativeRect( Unit::SubPixelsToPixels( head_box_ ) );
	Render::renderRectDebug( hr, { 0, 255, 0, 128 } );
};