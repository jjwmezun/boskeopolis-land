#pragma once

class InventoryLevel;

#include <string>

namespace Inventory
{
	static constexpr int DEFAULT_GEM_SCORE  = -1;
	static constexpr int DEFAULT_TIME_SCORE = -1;

	void reset();
	std::string levelName( int level );
	int currentLevel();
	bool levelComplete( int level );
	bool haveDiamond();
	bool haveDiamond( int level );
	void getDiamond();
	std::string gemScore( int level );
	bool gemChallengeBeaten( int level );
	std::string timeScore( int level );
	bool timeChallengeBeaten( int level );
	bool victory();
	bool victory( int level );
	bool getSecretGoal();
	bool getSecretGoal( int level );
	bool levelUnlocked( int level );
	bool beenToLevel( int level );
	void levelStart( int level );
	void setCurrentLevel( int level );
	int totalFundsShown();
	int totalFunds();
	double percent();
	std::u32string percentShown();
	std::u32string totalFundsString();
	void loseTotalFunds( int amount );
	void win( const InventoryLevel& level_inventory );
	void fail();
	void quit();
	void secretGoal( const InventoryLevel& level_inventory );
	void updateForOverworld();
	void updateForShop();
	void save();
	bool load();
	int heartUpgrades();
	bool haveOxygenUpgrade();
	bool hasHPUpgrade( int number );
	void giveOxygenUpgrade();
	void giveHPUpgrade( int number );
	bool hasCrown( int level );
	std::string getPlayerCostume();
};