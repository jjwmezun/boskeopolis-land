#include "audio.hpp"
#include "bullet_sprite.hpp"
#include "chamsby_sprite.hpp"
#include "collision.hpp"
#include "event_system.hpp"
#include "health.hpp"
#include "mezun_math.hpp"
#include "sprite_graphics.hpp"
#include "sprite_system.hpp"

static constexpr int HEALTH_PER_HIT = 4;
static constexpr int NUMBER_OF_HITS = 3;
static constexpr int MAX_HP = NUMBER_OF_HITS * HEALTH_PER_HIT;

ChamsbySprite::ChamsbySprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/chamsby.png", 0, 0, false, false, 0.0, false, 2, 1, 5, 1 ), x, y, 14, 25, {}, 500, 3000, 500, 4000, Direction::Horizontal::LEFT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::PERMANENT ),
	health_ ( 0 ),
	invincibility_ ( 0 ),
	walk_timer_ (),
	shoot_timer_ (),
	name ( "Lance Chamsby", 0, 0, Text::FontColor::DARK_GRAY ),
	health_timer_ ( 0 ),
	state_ ( ChamsbyState::INTRO )
{};

ChamsbySprite::~ChamsbySprite() {};

void ChamsbySprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	events.createBossUI();
	switch ( state_ )
	{
		case ( ChamsbyState::INTRO ):
		{
			events.pause_hero_ = true;
			if ( health_timer_ >= 4 )
			{
				Audio::playSound( Audio::SoundType::SELECT );
				++health_;
				if ( health_ == MAX_HP )
				{
					events.pause_hero_ = false;
					state_ = ChamsbyState::ATTACK;
					Audio::changeSong( "boss" );
				}
				health_timer_ = 0;
			}
			else
			{
				++health_timer_;
			}
			events.changeBossUI( name, health_ );
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
		}
		break;
	}
	flipGraphicsOnRight();
};

void ChamsbySprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( them.hasType( SpriteType::HERO ) )
	{
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
					sprites.spawn( std::make_unique<BulletSprite> ( centerXPixels(), centerYPixels(), Direction::horizontalToSimple( direction_x_ ), false ) );
				}

				if ( invincibility_ == 0 )
				{
					if ( them.collideBottomOnly( their_collision, *this ) )
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
						health.hurt();
					}
				}
			}
			break;
		}

		if ( health_ <= 0 && them.on_ground_ )
		{
			events.pause_hero_ = true;
		}
	}
};
