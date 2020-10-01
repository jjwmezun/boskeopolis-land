#include "camera.hpp"
#include "clock.hpp"
#include "event_system.hpp"
#include "inventory.hpp"
#include "inventory_level.hpp"
#include "map.hpp"
#include "mezun_helpers.hpp"
#include "render.hpp"
#include "sprite.hpp"
#include "wtext_obj.hpp"

constexpr bool testMultiples( int value )
{
	return value > 1 && value <= InventoryLevel::MAX_BOPS;
};

InventoryLevel::InventoryLevel( Difficulty difficulty, int max_hp, bool oxygen_upgrade )
:
	oxygen_upgrade_ ( oxygen_upgrade ),
	difficulty_ ( difficulty ),
	mcguffins_ ( 0 ),
	bops_ ( 0 ),
	ghost_kills_ ( 0 ),
	funds_ (),
	funds_shown_ (),
	clock_ (),
	graphics_ ( max_hp )
{};

InventoryLevel::~InventoryLevel() {};

void InventoryLevel::update( EventSystem& events, const Health& health )
{
	const bool funds_changed = updateFunds();
	const bool time_changed = clock_.update();
	graphics_.update( events, health, *this, funds_changed, time_changed );
};

void InventoryLevel::init( const Map& lvmap )
{
	graphics_.init( lvmap, *this );
};

void InventoryLevel::render( const EventSystem& events, const Sprite& hero, const Camera& camera )
{
	graphics_.render( events, hero, camera, *this );
};

void InventoryLevel::renderBops( const Sprite& hero, const Camera& camera ) const
{
	graphics_.renderBops( hero, camera, *this );
};

void InventoryLevel::setShowMcGuffins()
{
	graphics_.setShowMcGuffins();
};

void InventoryLevel::changeKillCounter( int count )
{
	graphics_.changeKillCounter( count );
};

void InventoryLevel::forceRerender()
{
	graphics_.forceRerender( *this );
};

bool InventoryLevel::updateFunds()
{
	const int funds_shown_momento = funds_shown_.value();
	if ( funds_shown_ < funds_.value() )
	{
		funds_shown_ += FUNDS_SPEED;
	}
	else if ( funds_shown_ > funds_.value() )
	{
		funds_shown_ -= FUNDS_SPEED;
	}
	return funds_shown_momento != funds_shown_.value();
}

int InventoryLevel::funds() const
{
	return funds_.value();
};

const Clock& InventoryLevel::clock() const
{
	return clock_;
};

int InventoryLevel::mcguffins() const
{
	return mcguffins_;
};

std::u32string InventoryLevel::fundsString() const
{
	return mezun::intToChar32StringWithPadding( funds_shown_(), FUNDS_MAX_DIGITS );
};

bool InventoryLevel::bopsMultiplier() const
{
	return testMultiples( bops_ );
};

int InventoryLevel::howManyBops() const
{
	return bops_;
};

int InventoryLevel::howManyGhostKills() const
{
	return ghost_kills_;
};

bool InventoryLevel::multipleGhostKills() const
{
	return testMultiples( ghost_kills_ );
};

bool InventoryLevel::isHardMode() const
{
	return difficulty_ == Difficulty::HARD;
};

void InventoryLevel::addFunds( int n )
{
	funds_ += n;
};

void InventoryLevel::loseFunds( int n )
{
	funds_ -= n;
};

void InventoryLevel::setFunds( int n )
{
	funds_ = n;
};

void InventoryLevel::addMcGuffin()
{
	++mcguffins_;
};

void InventoryLevel::bop()
{
	++bops_;
	if ( bopsMultiplier() )
	{
		addFundsForMultiplier( bops_ );
	}
};

void InventoryLevel::clearBops()
{
	bops_ = 0;
};

void InventoryLevel::addGhostKill()
{
	++ghost_kills_;
	if ( multipleGhostKills() )
	{
		addFundsForMultiplier( ghost_kills_ );
	}
};

void InventoryLevel::clearGhostKills()
{
	ghost_kills_ = 0;
};

void InventoryLevel::addFundsForMultiplier( int value )
{
	//  2    3    4    5     6      7      8
	// 100, 200, 400, 800, 1,600, 3,200, 6,400
	funds_ += ( int )( 100 * pow( 2, value - 2 ) );
}

Clock& InventoryLevel::clock()
{
	return clock_;
};

void InventoryLevel::setDiamondGotten()
{
	graphics_.reRenderDiamond();
};