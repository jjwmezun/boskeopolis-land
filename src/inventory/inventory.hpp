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

	void addMcGuffin();
	int McGuffins();

	void win();
	void fail();
	void quit();

	void heal();
	void hurt();
	int hp();

	void update();
	void save();
	void load();
};