#pragma once

class Clock;

#include <string>

namespace Inventory
{
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

	void addFunds( int n );
	void loseFunds( int n );
	int funds();
	void setFunds( int n );
	int fundsShown();
	int totalFundsShown();
	int totalFunds();
	double percent();
	std::string percentShown();
	Clock& clock();
	std::u32string fundsString();
	std::u32string totalFundsString();
	void loseTotalFunds( int amount );

	void addMcGuffin();
	int McGuffins();

	void win();
	void fail();
	void quit();
	void secretGoal();

	bool updateLevelFunds();
	void updateForOverworld();
	void updateForShop();
	void save();
	void load();
	
	int heartUpgrades();
	bool haveOxygenUpgrade();
	bool hasHPUpgrade( int number );
	void giveOxygenUpgrade();
	void giveHPUpgrade( int number );
	
	void bop();
	void clearBops();
	int howManyBops();
	bool bopsMultiplier();
	void addGhostKill();
	void clearGhostKills();
	int howManyGhostKills();
	bool multipleGhostKills();

	bool isHardMode();
	void setDifficultyNormal();
	void setDifficultyHard();
	bool hasCrown( int level );

	std::string getPlayerCostume();
};