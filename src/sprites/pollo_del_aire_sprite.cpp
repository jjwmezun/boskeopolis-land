#include "audio.hpp"
#include "collision.hpp"
#include "health.hpp"
#include "inventory.hpp"
#include "pollo_del_aire_sprite.hpp"
#include "sprite_graphics.hpp"

PolloDelAireSprite::PolloDelAireSprite( int x, int y, Direction::Horizontal dir_x, Direction::Vertical dir_y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/pollo_no_noko.png", 0, 32, ( dir_x == Direction::Horizontal::RIGHT ), false, 0, false, -1, -6, 2, 8 ), x, y, 22, 22, {}, 500, 1000, 500, 4000, dir_x, ( dir_y == Direction::Vertical::DOWN ) ? Direction::Vertical::DOWN : Direction::Vertical::UP, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
	vertical_too_ ( dir_y != Direction::Vertical::__NULL ),
	switch_from_ ( Direction::Horizontal::__NULL ),
	animation_counter_ ( 0 )
{};

PolloDelAireSprite::~PolloDelAireSprite() {};

void PolloDelAireSprite::deathAction( Camera& camera, EventSystem& events )
{
	polloDeath( camera, *this );
};

void PolloDelAireSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	if ( direction_x_ == Direction::Horizontal::RIGHT )
	{
		moveRight();
	}
	else
	{
		moveLeft();
	}

	if ( direction_y_ == Direction::Vertical::UP )
	{
		moveUp();
	}
	else
	{
		moveDown();
	}

	const int limit = ( vertical_too_ ) ? ( 5 * 16000 ) : 3000;
	if ( hit_box_.y < original_hit_box_.y - limit || collide_bottom_ )
	{
		direction_y_ = Direction::Vertical::DOWN;
	}
	else if ( hit_box_.y > original_hit_box_.y + limit || collide_top_ )
	{
		direction_y_ = Direction::Vertical::UP;
	}

	if ( ( graphics_->current_frame_x_ == 48 || graphics_->current_frame_x_ == 72 ) && switch_from_ != Direction::Horizontal::__NULL )
	{
		switch ( switch_from_ )
		{
			case ( Direction::Horizontal::LEFT ):
			{
				direction_x_ = Direction::Horizontal::RIGHT;
				graphics_->flip_x_ = true;
			}
			break;
			case ( Direction::Horizontal::RIGHT ):
			{
				direction_x_ = Direction::Horizontal::LEFT;
				graphics_->flip_x_ = false;
			}
			break;
		}

		switch_from_ = Direction::Horizontal::__NULL;
		graphics_->current_frame_x_ = 48;
	}
	else if ( hit_box_.x < original_hit_box_.x - ( 5 * 16000 ) )
	{
		graphics_->current_frame_x_ = 48;
		switch_from_ = Direction::Horizontal::LEFT;
	}
	else if ( hit_box_.x > original_hit_box_.x + ( 5 * 16000 ) )
	{
		graphics_->current_frame_x_ = 48;
		switch_from_ = Direction::Horizontal::RIGHT;
	}
	else if ( collidedRight() || collidedLeft() )
	{
		graphics_->current_frame_x_ = 48;
		switch_from_ = ( collidedLeft() ) ? Direction::Horizontal::LEFT : Direction::Horizontal::RIGHT;
	}
	else
	{
		graphics_->current_frame_x_ = 0;
	}
	
	++animation_counter_;
	if ( animation_counter_ >= 16 ) animation_counter_ = 0;
	graphics_->current_frame_x_ += 24 * floor(animation_counter_ / 8 );
};

void PolloDelAireSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	polloInteract( my_collision, their_collision, them, blocks, sprites, lvmap, health, *this );
};


void PolloDelAireSprite::polloInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, Sprite& me )
{
	if ( !me.isDead() )
	{
		if ( them.hasType( SpriteType::HERO ) && their_collision.collideAny() )
		{
			if ( them.bottomSubPixels() < me.ySubPixels() + 8000 )
			{
				me.kill();
				them.bounce();
				Inventory::bop();
				Audio::playSound( Audio::SoundType::BOP );
			}
			else if ( them.isSlidingPrev() )
			{
				me.kill();
				Audio::playSound( Audio::SoundType::BOP );
			}
			else
			{
				if ( their_collision.overlapXLeft() > 10000 || their_collision.overlapXRight() > 10000 )
				{
					health.hurt();
				}
			}
		}
		else
		{
			if ( my_collision.collideRight() )
			{
				me.collideStopXRight( my_collision.overlapXRight() );
			}
			else if ( my_collision.collideLeft() )
			{
				me.collideStopXLeft( my_collision.overlapXLeft() );
			}
		}
	}
};

void PolloDelAireSprite::reset()
{
	polloReset( *this );
};

void PolloDelAireSprite::polloReset( Sprite& me )
{
	me.is_dead_ = false;
	me.block_interact_ = true;
	me.sprite_interact_ = true;
	me.movement_ = me.getMovement( SpriteMovement::Type::FLOATING );
	me.resetPosition();

	me.hit_box_.h = 22000;
	me.graphics_->x_adjustment_ = -1;
	me.graphics_->y_adjustment_ = -6;
	me.graphics_->w_adjustment_ = 2;
	me.graphics_->h_adjustment_ = 8;
};

void PolloDelAireSprite::polloDeath( Camera& camera, Sprite& me )
{
	me.hit_box_.h = 17000;
	me.graphics_->x_adjustment_ = -2;
	me.graphics_->y_adjustment_ = -9;
	me.graphics_->w_adjustment_ = 4;
	me.graphics_->h_adjustment_ = 11;
	me.graphics_->current_frame_x_ = 96;
	me.graphics_->current_frame_y_ = 36;
	
	me.acceleration_x_ = 0;
	me.vx_ = 0;
	me.block_interact_ = false;
	me.sprite_interact_ = false;
	me.movement_ = me.getMovement( SpriteMovement::Type::GROUNDED );
	// NOTE: Don't finish death.
};