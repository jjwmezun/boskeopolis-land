#include "spider_sprite.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"

SpiderSprite::SpiderSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/box.png" ), x, y, 16, 16, { SpriteType::ENEMY }, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING ),
	direction_ ( Direction::Simple::__NULL )
{};

SpiderSprite::~SpiderSprite() {};

void SpiderSprite::customUpdate( LevelState& level_state )
{
	hit_box_.x -= 800;

	switch ( direction_ )
	{
		case ( Direction::Simple::__NULL ):
			if ( collide_left_ )
			{
				direction_ = Direction::Simple::UP;
			}
			else if ( collide_bottom_ )
			{
				direction_ = Direction::Simple::RIGHT;
			}
			else if ( collide_top_ )
			{
				direction_ = Direction::Simple::LEFT;
			}
			else
			{
				direction_ = Direction::Simple::DOWN;
			}
		break;
		case ( Direction::Simple::UP ):
		{
			moveUp();
			moveLeft();
			if ( collide_bottom_ )
			{
				direction_ = Direction::Simple::RIGHT;
			}
			else if ( !collide_left_ )
			{
				fullStopY();
				fullStopX();
				moveLeft();
				moveDown();
				direction_ = Direction::Simple::LEFT;
			}
		}
		break;
		case ( Direction::Simple::RIGHT ):
			moveRight();
			if ( collide_right_ )
			{
				direction_ = Direction::Simple::DOWN;
			}
		break;
		case ( Direction::Simple::DOWN ):
			moveDown();
			if ( collide_top_ )
			{
				direction_ = Direction::Simple::LEFT;
			}
		break;
		case ( Direction::Simple::LEFT ):
			moveLeft();
			if ( collide_left_ )
			{
				direction_ = Direction::Simple::UP;
			}
			else if ( !collide_top_ )
			{
				fullStopY();
				fullStopX();
				direction_ = Direction::Simple::DOWN;
			}
		break;
	}

	debugCollision();
};

void SpiderSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
};

void SpiderSprite::debugCollision()
{/*
	std::cout<<"================="<<std::endl;
	std::cout<<"TOP: "<<collide_bottom_<<std::endl;
	std::cout<<"RIT: "<<collide_right_<<std::endl;
	std::cout<<"BOT: "<<collide_top_<<std::endl;
	std::cout<<"LEF: "<<collide_left_<<std::endl;
	std::cout<<"PREV_TOP: "<<collide_bottom_prev_<<std::endl;
	std::cout<<"PREV_RIT: "<<collide_right_prev_<<std::endl;
	std::cout<<"PREV_BOT: "<<collide_top_prev_<<std::endl;
	std::cout<<"PREV_LEF: "<<collide_left_prev_<<std::endl;
	std::cout<<"DIRECTION: "<<Direction::simpleString( direction_ )<<std::endl;
	std::cout<<"================="<<std::endl;*/
}
