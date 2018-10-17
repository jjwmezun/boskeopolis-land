#include "audio.hpp"
#include <algorithm>
#include "inventory.hpp"
#include "health.hpp"

static constexpr int NORMAL_OXYGEN_STATUS = -1;
static constexpr int GAINING_OXYGEN = -2;

Health::Health()
:
	hp_ ( maxHP() ),
	heater_ ( false ),
	meter_ ( maxOxygen() ),
	invincible_ ( false ),
	lose_meter_amount_ ( NORMAL_OXYGEN_STATUS ),
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

	switch ( losingMeter() )
	{
		case ( true ):
			meter_ = std::max( meter_ - ( losePoint() * lose_meter_amount_ ), 0 );
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

	// If gaining oxygen, don't turn back to normal, 'less you've already hit max.
	if ( lose_meter_amount_ != GAINING_OXYGEN || meter_ >= maxOxygen())
	{
		lose_meter_amount_ = NORMAL_OXYGEN_STATUS;
	}
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

void Health::submerge( int amount )
{
	if ( lose_meter_amount_ != GAINING_OXYGEN )
	{
		lose_meter_amount_ = amount;
	}
};

void Health::refillOxygen()
{
	lose_meter_amount_ = GAINING_OXYGEN;
};

void Health::heatUp()
{
	heater_ = true;
	lose_meter_amount_ = 1;
};

bool Health::losingMeter() const
{
	return lose_meter_amount_ > NORMAL_OXYGEN_STATUS;
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

bool Health::hasFullHealth() const
{
	return hp_ >= maxHP();
};
