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
	return value > 1;
};

InventoryLevel::InventoryLevel( Difficulty difficulty, int max_hp, bool oxygen_upgrade, bool live_update_gfx )
:
	oxygen_upgrade_ ( oxygen_upgrade ),
	difficulty_ ( difficulty ),
	mcguffins_ ( 0 ),
	bops_ ( 0 ),
	ghost_kills_ ( 0 ),
	suits_ ( { false, false, false, false } ),
	funds_ (),
	funds_shown_ (),
	clock_ (),
	graphics_ ( max_hp, live_update_gfx )
{};

InventoryLevel::~InventoryLevel() {};

void InventoryLevel::update( EventSystem& events, const Health& health )
{
	const bool funds_changed = updateFunds();
	const bool time_changed = clock_.update();
	graphics_.update( events, health, *this, funds_changed, time_changed );
};

void InventoryLevel::init( const Level& level )
{
	graphics_.init( level, *this );
};

void InventoryLevel::render( const EventSystem& events, const Sprite& hero, const Camera& camera )
{
	graphics_.render( events, hero, camera, *this );
};

void InventoryLevel::renderBops( const Sprite& hero, const Camera& camera ) const
{
	graphics_.renderBops( hero, camera, *this );
};

void InventoryLevel::setShowCounter( InventoryLevelGraphics::Icon icon, int start_count )
{
	graphics_.setShowCounter( icon, start_count );
};

void InventoryLevel::changeCounter( int count )
{
	graphics_.changeCounter( count );
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
		if ( funds_shown_ > funds_.value() )
		{
			funds_shown_ = funds_.value();
		}
	}
	else if ( funds_shown_ > funds_.value() )
	{
		funds_shown_ -= FUNDS_SPEED;
		if ( funds_shown_ < funds_.value() )
		{
			funds_shown_ = funds_.value();
		}
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

void InventoryLevel::setFundsImmediatelyShown( int n )
{
	funds_ = funds_shown_ = n;
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
	funds_ += ( int )( 100 * pow( 2, std::min( MAX_BOPS, value ) - 2 ) );
}

Clock& InventoryLevel::clock()
{
	return clock_;
};

void InventoryLevel::setDiamondGotten()
{
	graphics_.reRenderDiamond();
};

bool InventoryLevel::haveSuit( CardSuit suit ) const
{
	return suits_[ ( int )( suit ) ];
};

void InventoryLevel::getSuit( CardSuit suit )
{
	suits_[ ( int )( suit ) ] = true;
	graphics_.setSuitGotten( suit );
};