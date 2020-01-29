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

	bool beenToLevel( int level );
	void levelStart( int level );
	void setCurrentLevel( int level );

	void addFunds( int n );
	void loseFunds( int n );
	int funds();
	void setFunds( int n );
	int fundsShown();
	int totalFundsShown();
	double percent();
	std::string percentShown();
	Clock& clock();
	std::u32string fundsString();

	void addMcGuffin();
	int McGuffins();

	void win();
	void fail();
	void quit();

	bool updateLevelFunds();
	void updateForOverworld();
	void save();
	void load();
	
	int heartUpgrades();
	bool haveOxygenUpgrade();
	
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
};