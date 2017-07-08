#include "sprite.h"
#include "sprite_graphics.h"
#include "sprite_status.h"

SpriteStatus::SpriteStatus( int max_hp, int hp )
:
	max_hp_ ( max_hp ),
	hp_ ( ( ( ( hp <= 0 ) ? max_hp : hp ) >= max_hp ) ? max_hp : hp ),
	dead_ ( false ),
	invincible_ ( false ),
	invincibility_timer_ ( TimerSimple( 48, false ) ),
	oxygen_meter_ ( OXYGEN_LIMIT )
{};

void SpriteStatus::update( Sprite& sprite, SpriteGraphics& graphics )
{
	if ( invincible_ )
	{
		invincibility_timer_.update();

		if ( invincibility_timer_.done() )
		{
			invincible_ = false;
			graphics.visible_ = true;
		}
		// If invincible, make sprite flash by having it invisible for every 4th frame o' invincibility.
		else if ( invincibility_timer_.counter() % 4 == 1 )
		{
			graphics.visible_ = false;
		}
		else
		{
			graphics.visible_ = true;
		}
	}

	if ( sprite.submerged_in_water_ )
	{
		--oxygen_meter_;
	}
	else
	{
		oxygen_meter_ = OXYGEN_LIMIT;
	}
};

void SpriteStatus::reset()
{
	hp_ = max_hp_;
	dead_ = false;
	invincible_ = false;
};

void SpriteStatus::kill()
{
	hp_ = 0;
	dead_ = true;
};

bool SpriteStatus::isDead() const
{
	return dead_;
};

int SpriteStatus::hp() const
{
	return hp_;
};

void SpriteStatus::heal( int amount )
{
	hp_ += amount;

	if ( hp_ > max_hp_ )
		hp_ = max_hp_;
};

void SpriteStatus::fullHeal()
{
	hp_ = max_hp_;
};

void SpriteStatus::hurt( int amount )
{
	if ( !invincible_ )
	{
		hp_ -= amount;

		if ( hp_ <= 0 )
		{
			kill();
		}
		else
		{
			invincible_ = true;
			invincibility_timer_.start();
		}
	}
};

bool SpriteStatus::drowned() const
{
	return oxygen_meter_ <= 0;
};

double SpriteStatus::oxygenPercent() const
{
	return double( (double)oxygen_meter_ / (double)OXYGEN_LIMIT );
};
