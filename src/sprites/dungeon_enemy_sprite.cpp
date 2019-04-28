#include "dungeon_enemy_sprite.hpp"
#include "collision.hpp"
#include "mezun_math.hpp"
#include "sprite_graphics.hpp"
#include "sprite_system.hpp"
#include "top_down_bullet_sprite.hpp"

static inline int generateWalkDelay()
{
	return mezun::randInt( 320, 60 );
};

static inline int testShoot()
{
	return mezun::testRandomWithinPercentage( 75 );
}

DungeonEnemySprite::DungeonEnemySprite( int x, int y, int layer )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/nut-monk.png", 0, 0, false, false, 0.0, false, 1, 1, 5, 2 ), x, y, 14, 14, { SpriteType::ENEMY }, 250, 1000, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
	walk_delay_ ( generateWalkDelay() ),
	walk_timer_ ( 0 ),
	state_ ( State::WALK ),
	next_state_ ( State::CHANGE )
{
	direction_ = Direction::Simple::DOWN;
	layer_ = layer;
};

DungeonEnemySprite::~DungeonEnemySprite() {};

void DungeonEnemySprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	updateActions( sprites );
	updateGraphics();
};

void DungeonEnemySprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( them.layer_ == layer_ )
	{
		if ( them.hasType( SpriteType::HEROS_BULLET ) && their_collision.collideAny() )
		{
			kill();
		}
	}
};

void DungeonEnemySprite::updateActions( SpriteSystem& sprites )
{
	switch ( state_ )
	{
		case ( State::WALK ):
		{
			moveInDirection();
			if ( walk_timer_ >= walk_delay_ )
			{
				state_ = State::CHANGE;
			}
			else
			{
				if ( collide_top_ || collide_left_ || collide_right_ || collide_bottom_ )
				{
					fullStopX();
					fullStopY();
					if ( collide_top_ )
					{
						direction_ = Direction::Simple::UP;
					}
					else if ( collide_bottom_ )
					{
						direction_ = Direction::Simple::DOWN;
					}
					else if ( collide_left_ )
					{
						direction_ = Direction::Simple::RIGHT;
					}
					else if ( collide_right_ )
					{
						direction_ = Direction::Simple::LEFT;
					}
				}
				++walk_timer_;
			}
		}
		break;

		case ( State::SHOOT ):
		{
			shoot( sprites );
			state_ = State::CHANGE;
		}
		break;

		case ( State::CHANGE ):
		{
			fullStopX();
			fullStopY();
			switch ( next_state_ )
			{
				case ( State::WALK ):
				{
					if ( walk_timer_ >= walk_delay_ + 8 )
					{
						walk_delay_ = generateWalkDelay();
						walk_timer_ = 0;
						direction_ = Direction::randomSimple( false );
						next_state_ = State::CHANGE;
						state_ = State::WALK;
					}
					else
					{
						++walk_timer_;
					}
				}
				break;

				case ( State::SHOOT ):
				{
					if ( walk_timer_ >= walk_delay_ + 32 )
					{
						walk_timer_ = walk_delay_ + 34;
						next_state_ = State::WALK;
						state_ = State::SHOOT;
					}
					else
					{
						++walk_timer_;
					}
				}
				break;

				case ( State::CHANGE ):
				{
					next_state_ = ( testShoot() ) ? State::SHOOT : State::WALK;
				}
				break;
			}
		}
		break;
	}
}

void DungeonEnemySprite::changeDirection()
{
	fullStopX();
	fullStopY();
	direction_ = Direction::randomSimple( false );
}

void DungeonEnemySprite::shoot( SpriteSystem& sprites )
{
	int x = centerXPixels();
	int y = centerYPixels();
	switch ( direction_ )
	{
		case( Direction::Simple::UP ):
		{
			y = yPixels();
		}
		break;
		case( Direction::Simple::RIGHT ):
		{
			x = rightPixels();
		}
		break;
		case( Direction::Simple::DOWN ):
		{
			y = bottomPixels();
		}
		break;
		case( Direction::Simple::LEFT ):
		{
			x = xPixels();
		}
		break;
	}
	sprites.spawn( std::make_unique<TopDownBulletSprite> ( x, y, direction_, SpriteType::ENEMY, layer_ ) );
};

void DungeonEnemySprite::updateGraphics()
{
	graphics_->priority_ = layer_ == 2;
	switch ( direction_ )
	{
		case ( Direction::Simple::UP ):
		{
			graphics_->current_frame_x_ = 19;
			graphics_->flip_x_ = false;
		}
		break;
		case ( Direction::Simple::RIGHT ):
		{
			graphics_->current_frame_x_ = 38;
			graphics_->flip_x_ = true;
		}
		break;
		case ( Direction::Simple::DOWN ):
		{
			graphics_->current_frame_x_ = 0;
			graphics_->flip_x_ = false;
		}
		break;
		case ( Direction::Simple::LEFT ):
		{
			graphics_->current_frame_x_ = 38;
			graphics_->flip_x_ = false;
		}
		break;
	}
}
