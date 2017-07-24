#include <algorithm>
#include "inventory.hpp"
#include "health.hpp"

Health::Health()
:
	hp_ ( maxHP() ),
	oxygen_meter_ ( maxOxygen() ),
	invincible_ ( false ),
	in_water_ ( false ),
	submerged_in_water_ ( false ),
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

	if ( submerged_in_water_ )
	{
		oxygen_meter_ = std::max( oxygen_meter_ - 1, 0 );
	}
	else
	{
		oxygen_meter_ = std::min( oxygen_meter_ + 2, maxOxygen() );
	}

	submerged_in_water_ = false;
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
};

void Health::fullHeal()
{
	hp_ = maxHP();
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
		}
	}
};

void Health::submerge()
{
	submerged_in_water_ = true;
};

void Health::goInWater()
{
	in_water_ = true;
};

void Health::outOfWater()
{
	in_water_ = false;
};

bool Health::submerged() const
{
	return submerged_in_water_;
};

bool Health::inWater() const
{
	return in_water_;
};

bool Health::drowned() const
{
	return oxygen_meter_ <= 0;
};

double Health::oxygenPercent() const
{
	return double( ( double )( oxygen_meter_ ) / ( double )( maxOxygen() ) );
};

int Health::maxHP()
{
	return START_MAX_HP + Inventory::heartUpgrades();
};

int Health::maxOxygen()
{
	switch( Inventory::haveOxygenUpgrade() )
	{
		case ( true ):
			return STRONGER_OXYGEN_LIMIT;
		break;

		default:
			return NORMAL_OXYGEN_LIMIT;
		break;
	}
};