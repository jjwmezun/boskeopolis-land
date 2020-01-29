#include "audio.hpp"
#include <algorithm>
#include "inventory.hpp"
#include "health.hpp"
#include "main.hpp"

static constexpr int NORMAL_OXYGEN_STATUS = -1;
static constexpr int GAINING_OXYGEN = -2;
static constexpr int NORMAL_OXYGEN_LIMIT = 720;
static constexpr int STRONGER_OXYGEN_LIMIT = 720 + 360;
static constexpr int HEAT_LIMIT = 500;
static constexpr int START_MAX_HP = 2;

Health::Health()
:
	hp_ ( maxHP() ),
	heater_ ( false ),
	meter_ ( maxOxygen() ),
	lose_meter_amount_ ( NORMAL_OXYGEN_STATUS ),
	invincibility_timer_ ()
{};

void Health::update()
{
	if ( invincibility_timer_.on() )
	{
		invincibility_timer_.update();
	}

	meter_ = ( losingMeter() )
		? std::max( meter_ - ( losePoint() * lose_meter_amount_ ), 0 )
		: std::min( meter_ + restorePoint(), maxOxygen() );

	if ( heater_ && meter_ <= 0 )
	{
		hurt();
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

int Health::getInvincibilityCounter() const
{
	return invincibility_timer_.counter();
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
	if ( !Main::noharm() && !invincibility_timer_.on() )
	{
		--hp_;
		if ( hp_ > 0 )
		{
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
	return ( Inventory::isHardMode() ) ? 1 : START_MAX_HP + Inventory::heartUpgrades();
};

int Health::maxOxygen() const
{
	return ( heater_ )
		? HEAT_LIMIT
		: ( Inventory::haveOxygenUpgrade() )
			? STRONGER_OXYGEN_LIMIT
			: NORMAL_OXYGEN_LIMIT;
};

int Health::losePoint() const
{
	return ( heater_ ) ? 10 : 1;
};

int Health::restorePoint() const
{
	return ( heater_ ) ? 10 : ( int )( ( float )( maxOxygen() ) / 100 );
};

bool Health::hasFullHealth() const
{
	return hp_ >= maxHP();
};
