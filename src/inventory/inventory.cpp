#include <cassert>
#include "clock.hpp"
#include "inventory.hpp"
#include "inventory_level.hpp"
#include <fstream>
#include "level_data.hpp"
#include "level_list.hpp"
#include "localization.hpp"
#include "localization_language.hpp"
#include "main.hpp"
#include "mezun_exceptions.hpp"
#include "mezun_helpers.hpp"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "title_state.hpp"
#include "wmessage_state.hpp"

CounterT<Unit::TOTAL_FUNDS_MAX, Unit::TOTAL_FUNDS_MIN> Inventory::total_funds_shown_{};
Save Inventory::save_{};

OWTile Inventory::currentSpace() { return save_.data_.current_space_; };

int Inventory::currentLevel() { return ( save_.data_.current_space_.isLevel() ) ? save_.data_.current_space_.getLevelNumber() : -1; };

bool Inventory::levelComplete( int level )
{
	return
		victory( level ) &&
		( !LevelList::hasCard( level ) || haveDiamond( level ) ) &&
		( !LevelList::hasHardMode( level ) || hasCrown( level ) ) &&
		( !LevelList::hasSuits( level ) || hasSuits( level ) ) &&
		( !LevelList::hasSecretGoal( level ) || getSecretGoal( level ) ) &&
		( !LevelList::hasGemScore( level ) || gemChallengeBeaten( level ) ) &&
		( !LevelList::hasTimeScore( level ) || timeChallengeBeaten( level ) );
};

bool Inventory::haveDiamond()
{
	return ( save_.data_.current_space_.isLevel() ) ? save_.data_.diamonds_[ currentLevel() ] : false;
};

bool Inventory::haveDiamond( int level )
{
	return save_.data_.diamonds_[ level ];
};

bool Inventory::hasSuits( int level )
{
	return save_.data_.suits_[ level ];
};

void Inventory::getDiamond()
{
	if ( save_.data_.current_space_.isLevel() )
	{
		save_.data_.diamonds_[ currentLevel() ] = true;
		save();
	}
};

std::u32string Inventory::gemScore( int level )
{
	return ( save_.data_.gem_scores_[ level ] > Unit::DEFAULT_GEM_SCORE )
		? mezun::intToChar32StringWithPadding( save_.data_.gem_scores_[ level ], FUNDS_MAX_DIGITS )
		: U"-----";
};

void Inventory::setGemScore( int level, int value )
{
	if ( value > -1 && value < InventoryLevel::FUNDS_MAX )
	{
		save_.data_.gem_scores_[ level ] = value;
	}
};

void Inventory::winGemScore( int funds )
{
	if ( save_.data_.current_space_.isLevel() && funds >= save_.data_.gem_scores_[ currentLevel() ] )
	{
		setGemScore( currentLevel(), funds );
	}
};

bool Inventory::gemChallengeBeaten( int level )
{
	return save_.data_.gem_scores_[ level ] >= LevelList::gemChallenge( level );
}

std::u32string Inventory::timeScore( int level )
{
	return ( save_.data_.time_scores_[ level ] > Unit::DEFAULT_GEM_SCORE )
		? Clock::timeToString( save_.data_.time_scores_[ level ] )
		: U"----";
};

void Inventory::setTimeScore( int level, int value )
{
	if ( value > -1 && value < TIME_MAX )
	{
		save_.data_.time_scores_[ level ] = value;
	}
};

void Inventory::winTimeScore( const Clock& clock )
{
	if ( save_.data_.current_space_.isLevel() )
	{
		if ( save_.data_.time_scores_[ currentLevel() ] < 0 || clock.totalSeconds() <= save_.data_.time_scores_[ currentLevel() ] )
		{
			setTimeScore( currentLevel(), clock.totalSeconds() );
		}
	}
};

bool Inventory::timeChallengeBeaten( int level )
{
	return save_.data_.time_scores_[ level ] > 0 && save_.data_.time_scores_[ level ] <= LevelList::timeChallenge( level );
}

bool Inventory::victory()
{
	return ( save_.data_.current_space_.isLevel() ) ? save_.data_.victories_[ currentLevel() ] : false;
};

bool Inventory::victory( int level )
{
	return save_.data_.victories_[ level ];
};

bool Inventory::getSecretGoal()
{
	return ( save_.data_.current_space_.isLevel() ) ? save_.data_.secret_goals_[ currentLevel() ] : false;
};

bool Inventory::getSecretGoal( int level )
{
	return save_.data_.secret_goals_[ level ];
};

bool Inventory::beenToLevel( int level )
{
	return save_.data_.been_to_level_[ level ];
};

void Inventory::levelStart( int level )
{
	save_.data_.been_to_level_[ level ] = true;
	save_.data_.current_space_ = OWTile::createLevel( level );
	save();
};

void Inventory::setSpaceAsShop( int value )
{
	save_.data_.current_space_ = OWTile::createShop( value );
};

int Inventory::totalFundsShown()
{
	return total_funds_shown_();
};

int Inventory::totalFunds()
{
	return save_.data_.total_funds_();
};

int Inventory::levelsBeaten()
{
	int n = 0;

	for ( auto b : save_.data_.victories_ )
	{
		if ( b )
		{
			++n;
		}
	}

	return n;
};

int Inventory::diamondsGotten()
{
	int n = 0;

	for ( auto b : save_.data_.diamonds_ )
	{
		if ( b )
		{
			++n;
		}
	}

	return n;
};

int Inventory::gemChallengesWon()
{
	int n = 0;

	for ( int i = 0; i < LevelList::getNumberOfLevels(); ++i )
	{
		if ( gemChallengeBeaten( i ) )
		{
			++n;
		}
	}

	return n;
};

int Inventory::timeChallengesWon()
{
	int n = 0;

	for ( int i = 0; i < LevelList::getNumberOfLevels(); ++i )
	{
		if ( timeChallengeBeaten( i ) )
		{
			++n;
		}
	}

	return n;
};

double Inventory::percent()
{
	int score_target = 0;
	int score_attained = 0;
	for ( const auto& level : LevelList::getLevelDataList() )
	{
		score_target += 20;
		if ( victory( level.id_ ) )
		{
			score_attained += 20;
		}

		if ( LevelList::hasCard( level.id_ ) )
		{
			score_target += 10;
			if ( haveDiamond( level.id_ ) )
			{
				score_attained += 10;
			}
		}

		if ( LevelList::hasGemScore( level.id_ ) )
		{
			score_target += 5;
			if ( gemChallengeBeaten( level.id_ ) )
			{
				score_attained += 5;
			}
		}

		if ( LevelList::hasTimeScore( level.id_ ) )
		{
			score_target += 5;
			if ( timeChallengeBeaten( level.id_ ) )
			{
				score_attained += 5;
			}
		}

		if ( LevelList::hasSecretGoal( level.id_ ) )
		{
			score_target += 20;
			if ( getSecretGoal( level.id_ ) )
			{
				score_attained += 20;
			}
		}
	}

	return ( double )( score_attained ) / ( double )( score_target );
};

std::u32string Inventory::percentShown()
{
	return mezun::merge32Strings( mezun::intToChar32String( ( int )( std::floor( percent() ) ) ), U"%" );
};

void Inventory::updateForOverworld()
{
	if ( total_funds_shown_() < save_.data_.total_funds_() )
	{
		if ( save_.data_.total_funds_() - total_funds_shown_() < TOTAL_FUNDS_SPEED )
		{
			total_funds_shown_ = save_.data_.total_funds_;
		}
		else
		{
			total_funds_shown_ += TOTAL_FUNDS_SPEED;
		}
	}
	else if ( total_funds_shown_() > save_.data_.total_funds_() )
	{
		if ( total_funds_shown_() - save_.data_.total_funds_() < TOTAL_FUNDS_SPEED )
		{
			total_funds_shown_ = save_.data_.total_funds_;
		}
		else
		{
			total_funds_shown_ -= TOTAL_FUNDS_SPEED;
		}
	}
};

void Inventory::updateForShop()
{
	updateForOverworld();
};

void Inventory::save()
{
	if ( !Main::nosave() )
	{
		save_.save();
	}
};

void Inventory::load( Save save )
{
	save_ = save;
	total_funds_shown_ = save.data_.total_funds_;
};

void Inventory::win( const InventoryLevel& level_inventory )
{
	if ( save_.data_.current_space_.isLevel() )
	{
		const int level_id = save_.data_.current_space_.getLevelNumber();
		save_.data_.victories_[ level_id ] = true;
		save_.data_.levels_unlocked_[ LevelList::getNextLevel( level_id ) ] = true;
		generalVictory( level_inventory );
	}
};

void Inventory::fail()
{
	save_.data_.total_funds_ -= PRICE_OF_DEATH;
	save();
};

void Inventory::quit()
{
	if ( !victory() )
	{
		fail();
	}
};

void Inventory::secretGoal( const InventoryLevel& level_inventory )
{
	if ( save_.data_.current_space_.isLevel() )
	{
		save_.data_.secret_goals_[ currentLevel() ] = true;
		generalVictory( level_inventory );
	}
};

void Inventory::generalVictory( const InventoryLevel& level_inventory )
{
	if ( level_inventory.isHardMode() )
	{
		save_.data_.crowns_[ currentLevel() ] = true;
	}
	if ( level_inventory.haveAllSuits() )
	{
		save_.data_.suits_[ currentLevel() ] = true;
	}
	winGemScore( level_inventory.funds() );
	winTimeScore( level_inventory.clock() );
	save_.data_.total_funds_ += level_inventory.funds();
	save();
}

int Inventory::heartUpgrades()
{
	int upgrade_count = 0;
	for ( int i = 0; i < Unit::MAX_HEART_UPGRADES; ++i )
	{
		if ( save_.data_.health_upgrades_[ i ] )
		{
			++upgrade_count;
		}
	}
	return upgrade_count;
};

bool Inventory::haveOxygenUpgrade()
{
	return save_.data_.oxygen_upgrade_;
};

std::u32string Inventory::totalFundsString()
{
	return ( total_funds_shown_ < 0 )
		? mezun::intToChar32StringWithPadding( total_funds_shown_(), TOTAL_FUNDS_MAX_DIGITS )
		: mezun::merge32Strings( U" ", mezun::intToChar32StringWithPadding( total_funds_shown_(), TOTAL_FUNDS_MAX_DIGITS ) );
};

bool Inventory::hasCrown( int level )
{
	return save_.data_.crowns_[ level ];
};

bool Inventory::levelUnlocked( int level )
{
	return ( level > 0 ) ? victory( level - 1 ) : true;
};

std::string Inventory::getPlayerCostume()
{
	return "sprites/autumn-skirt.png";
};

void Inventory::loseTotalFunds( int amount )
{
	save_.data_.total_funds_ -= amount;
};

bool Inventory::hasHPUpgrade( int number )
{
	return save_.data_.health_upgrades_[ number ];
};

void Inventory::giveOxygenUpgrade()
{
	save_.data_.oxygen_upgrade_ = true;
};

void Inventory::giveHPUpgrade( int number )
{
	save_.data_.health_upgrades_[ number ] = true;
};

void Inventory::unlockSpecialLevel( int number )
{
	save_.data_.levels_unlocked_[ LevelList::getSpecialLevelID( number ) ] = true;
};

bool Inventory::specialLevelUnlocked( int number )
{
	return save_.data_.levels_unlocked_[ LevelList::getSpecialLevelID( number ) ];
};