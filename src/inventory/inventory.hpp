#pragma once

class Clock;
class InventoryLevel;

#include "counter_t.hpp"
#include "save.hpp"
#include <string>
#include "unit.hpp"

class Inventory final
{
	public:
		static OWTile currentSpace();
		static int currentLevel();
		static bool levelComplete( int level );
		static bool haveDiamond();
		static bool haveDiamond( int level );
		static void getDiamond();
		static std::u32string gemScore( int level );
		static bool gemChallengeBeaten( int level );
		static std::u32string timeScore( int level );
		static bool timeChallengeBeaten( int level );
		static bool victory();
		static bool victory( int level );
		static bool getSecretGoal();
		static bool getSecretGoal( int level );
		static bool levelUnlocked( int level );
		static bool beenToLevel( int level );
		static void levelStart( int level );
		static void setSpaceAsShop( int value );
		static int totalFundsShown();
		static int totalFunds();
		static double percent();
		static std::u32string percentShown();
		static std::u32string totalFundsString();
		static void loseTotalFunds( int amount );
		static void win( const InventoryLevel& level_inventory );
		static void fail();
		static void quit();
		static void secretGoal( const InventoryLevel& level_inventory );
		static void updateForOverworld();
		static void updateForShop();
		static void save();
		static void load( Save save );
		static int heartUpgrades();
		static bool haveOxygenUpgrade();
		static bool hasHPUpgrade( int number );
		static void giveOxygenUpgrade();
		static void giveHPUpgrade( int number );
		static void unlockSpecialLevel( int number );
		static bool specialLevelUnlocked( int number );
		static bool hasCrown( int level );
		static std::string getPlayerCostume();

	private:
		static double percentPerLevel();
		static double percentPerVictory();
		static double percentPerDiamond();
		static double percentPerScore();
		static int levelsBeaten();
		static int diamondsGotten();
		static int gemChallengesWon();
		static int timeChallengesWon();
		static void setGemScore( int level, int value );
		static void setTimeScore( int level, int value );
		static void winGemScore( int funds );
		static void winTimeScore( const Clock& clock );
		static double totalVictoryPercents();
		static double totalDiamondPercents();
		static double totalGemChallengePercents();
		static double totalTimeChallengePercents();
		static void generalVictory( const InventoryLevel& level_inventory );

		static constexpr bool DEFAULT_VICTORY        = false;
		static constexpr bool DEFAULT_DIAMOND        = false;
		static constexpr int  PRICE_OF_DEATH         = 2500;
		static constexpr int  TIME_MAX               = ( 60 * 9 ) + 59;
		static constexpr int  FUNDS_MAX_DIGITS       = 5;
		static constexpr int  TOTAL_FUNDS_MAX_DIGITS = 9;
		static constexpr int  FUNDS_SPEED            = 25;
		static constexpr int  TOTAL_FUNDS_SPEED      = 100;
		static constexpr int  MAX_BOPS               = 8;

		static CounterT<Unit::TOTAL_FUNDS_MAX, Unit::TOTAL_FUNDS_MIN> total_funds_shown_;
		static Save save_;
};