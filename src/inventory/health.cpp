#include "audio.hpp"
#include <algorithm>
#include "inventory.hpp"
#include "health.hpp"

Health::Health()
:
	hp_ ( maxHP() ),
	meter_ ( maxOxygen() ),
	invincible_ ( false ),
	lose_meter_ ( false ),
	heater_ ( false ),
	invincibility_timer_ ( 48, false )
{};

void Health::update()
{
	if ( invincible_ )
	{
		invincibility_timer_.update();

		if ( invincibility_timer_.done() )
		{
			invincible_ = false;
		}
	}

	switch ( lose_meter_ )
	{
		case ( true ):
			meter_ = std::max( meter_ - losePoint(), 0 );
		break;

		default:
			meter_ = std::min( meter_ + restorePoint(), maxOxygen() );
		break;
	}
	
	switch ( heater_ )
	{
		case ( true ):
			if ( meter_ <= 0 )
			{
				hurt();
			}
		break;
	}

	lose_meter_ = false;
};

bool Health::flickerOff() const
{
	return invincibility_timer_.counter() % 4 == 1;
};

int Health::hp() const
{
	return hp_;
};

void Health::heal()
{
	++hp_;

	if ( hp_ > maxHP() )
	{
		hp_ = maxHP();
	}
	else
	{
		Audio::playSound( Audio::SoundType::HEAL );
	}
};

void Health::fullHeal()
{
	if ( hp_ < maxHP() )
	{
		hp_ = maxHP();
		Audio::playSound( Audio::SoundType::HEAL );
	}
};

void Health::hurt()
{
	if ( !invincible_ )
	{
		--hp_;

		if ( hp_ > 0 )
		{
			invincible_ = true;
			invincibility_timer_.start();
			Audio::playSound( Audio::SoundType::HURT );
		}
	}
};

void Health::submerge()
{
	lose_meter_ = true;
};

void Health::heatUp()
{
	heater_ = true;
	lose_meter_ = true;
};

bool Health::losingMeter() const
{
	return lose_meter_;
};

bool Health::drowned() const
{
	return !heater_ && meter_ <= 0;
};

bool Health::overheated() const
{
	return heater_ && meter_ <= 0;
};

double Health::oxygenPercent() const
{
	return double( ( double )( meter_ ) / ( double )( maxOxygen() ) );
};

int Health::maxHP()
{
	return START_MAX_HP + Inventory::heartUpgrades();
};

int Health::maxOxygen() const
{
	switch( heater_ )
	{
		case ( true ):
			return HEAT_LIMIT;
		break;

		default:
			switch( Inventory::haveOxygenUpgrade() )
			{
				case ( true ):
					return STRONGER_OXYGEN_LIMIT;
				break;

				default:
					return NORMAL_OXYGEN_LIMIT;
				break;
			}
		break;
	}
};

int Health::losePoint() const
{
	switch( heater_ )
	{
		case ( true ):
			return 10;
		break;

		default:	
			return 1;
		break;
	}
};

int Health::restorePoint() const
{
	switch( heater_ )
	{
		case ( true ):
			return 10;
		break;

		default:	
			return maxOxygen() / 100;
		break;
	}
};