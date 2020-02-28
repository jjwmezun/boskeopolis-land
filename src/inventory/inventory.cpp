#include <cassert>
#include "clock.hpp"
#include "inventory.hpp"
#include "inventory_level.hpp"
#include <fstream>
#include "level.hpp"
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

int Inventory::currentLevel() { std::cout << save_.data_.current_level_() << std::endl; return save_.data_.current_level_(); };

bool Inventory::levelComplete( int level )
{
	return haveDiamond( level ) &&
		victory( level ) &&
		hasCrown( level ) &&
		( !Level::hasSecretGoal( level ) || getSecretGoal( level ) ) &&
		gemChallengeBeaten( level ) &&
		timeChallengeBeaten( level );
};

bool Inventory::haveDiamond()
{
	return save_.data_.diamonds_[ save_.data_.current_level_() ];
};

bool Inventory::haveDiamond( int level )
{
	return save_.data_.diamonds_[ level ];
};

void Inventory::getDiamond()
{
	save_.data_.diamonds_[ save_.data_.current_level_() ] = true;
	save();
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
	if ( funds >= save_.data_.gem_scores_[ save_.data_.current_level_() ] )
	{
		setGemScore( save_.data_.current_level_(), funds );
	}
};

bool Inventory::gemChallengeBeaten( int level )
{
	return save_.data_.gem_scores_[ level ] >= Level::gemChallenge( level );
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
	if ( save_.data_.time_scores_[ save_.data_.current_level_() ] < 0 || clock.totalSeconds() <= save_.data_.time_scores_[ save_.data_.current_level_() ] )
	{
		setTimeScore( save_.data_.current_level_(), clock.totalSeconds() );
	}
};

bool Inventory::timeChallengeBeaten( int level )
{
	return save_.data_.time_scores_[ level ] > 0 && save_.data_.time_scores_[ level ] <= Level::timeChallenge( level );
}

bool Inventory::victory()
{
	return save_.data_.victories_[ save_.data_.current_level_() ];
};

bool Inventory::victory( int level )
{
	return save_.data_.victories_[ level ];
};

bool Inventory::getSecretGoal()
{
	return save_.data_.secret_goals_[ save_.data_.current_level_() ];
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
	save_.data_.current_level_ = level;
	save();
};

void Inventory::setCurrentLevel( int level )
{
	save_.data_.current_level_ = level;
};

int Inventory::totalFundsShown()
{
	return total_funds_shown_();
};

int Inventory::totalFunds()
{
	return save_.data_.total_funds_();
};

double Inventory::percentPerLevel()
{
	return 100.0 / ( double )( Level::NUMBER_OF_LEVELS );
};

double Inventory::percentPerVictory()
{
	return percentPerLevel() * 0.5;
};

double Inventory::percentPerDiamond()
{
	return percentPerLevel() * 0.3;
};

double Inventory::percentPerScore()
{
	return percentPerLevel() * 0.1;
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

	for ( int i = 0; i < Level::NUMBER_OF_LEVELS; ++i )
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

	for ( int i = 0; i < Level::NUMBER_OF_LEVELS; ++i )
	{
		if ( timeChallengeBeaten( i ) )
		{
			++n;
		}
	}

	return n;
};

double Inventory::totalVictoryPercents()
{
	return levelsBeaten() * percentPerVictory();
};

double Inventory::totalDiamondPercents()
{
	return diamondsGotten() * percentPerDiamond();
};

double Inventory::totalGemChallengePercents()
{
	return gemChallengesWon() * percentPerScore();
};

double Inventory::totalTimeChallengePercents()
{
	return timeChallengesWon() * percentPerScore();
};

double Inventory::percent()
{
	// Divide 100 by # o' levels
	// Divide each level percent into 4 types o' completion:
	// * Beaten         -> 50%
	// * Diamond        -> 30%
	// * Gem Challenge  -> 10%
	// * Time Challenge -> 10%

	return totalVictoryPercents() + totalDiamondPercents() + totalGemChallengePercents() + totalTimeChallengePercents();
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
};

void Inventory::win( const InventoryLevel& level_inventory )
{
	save_.data_.victories_[ save_.data_.current_level_() ] = true;
	generalVictory( level_inventory );
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
	save_.data_.secret_goals_[ save_.data_.current_level_() ] = true;
	generalVictory( level_inventory );
};

void Inventory::generalVictory( const InventoryLevel& level_inventory )
{
	if ( level_inventory.isHardMode() )
	{
		save_.data_.crowns_[ save_.data_.current_level_() ] = true;
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

void Inventory::throwSaveCorruptionErrorMessage()
{
	Main::changeState
	(
		WMessageState::generateErrorMessage
		(
			Localization::getCurrentLanguage().getSaveCorruptionErrorMessage( "save0" ),
			WMessageState::Type::CHANGE,
			std::unique_ptr<TitleState> ( new TitleState() )
		)
	);
}
