#include "bump_under_block_sprite.hpp"
#include "collision.hpp"
#include "sprite.hpp"

BumpUnderBlockSprite::BumpUnderBlockSprite()
:
	delay_ (),
	ready_ ( true )
{};

void BumpUnderBlockSprite::update( Sprite& sprite )
{
	if ( !ready_ )
	{
		if ( delay_.done() )
		{
			ready_ = true;
			delay_.stop();
		}
		else if ( delay_.on() )
		{
			delay_.update();
		}
		else
		{
			delay_.start();
		}
	}

	if ( sprite.isUpsideDown() )
	{
		if ( sprite.hit_box_.y > sprite.original_hit_box_.y )
		{
			sprite.hit_box_.y -= 500;
		}
		if ( sprite.hit_box_.y < sprite.original_hit_box_.y )
		{
			sprite.hit_box_.y = sprite.original_hit_box_.y;
		}
	}
	else
	{
		if ( sprite.hit_box_.y < sprite.original_hit_box_.y )
		{
			sprite.hit_box_.y += 500;
		}
		if ( sprite.hit_box_.y > sprite.original_hit_box_.y )
		{
			sprite.hit_box_.y = sprite.original_hit_box_.y;
		}
	}
};

bool BumpUnderBlockSprite::testHit( Sprite& me, Sprite& them, const Collision& collision )
{
	int hit = false;
	if ( collision.collideAny() )
	{
		them.collideStopAny( collision );
		if ( me.isUpsideDown() )
		{
			if ( collision.collideBottom() )
			{
				if ( ready_ )
				{
					hit = true;
					ready_ = false;
				}
				me.hit_box_.y += collision.overlapYBottom() / 2;
			}
		}
		else if ( collision.collideTop() )
		{
			if ( ready_ )
			{
				hit = true;
				ready_ = false;
			}
			me.hit_box_.y -= collision.overlapYTop() / 2;
		}
	}
	return hit;
};
