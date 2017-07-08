#pragma once

class Graphics;

#include "counter.h"
#include "level.h"

class Inventory
{
	public:

		static constexpr bool DEFAULT_VICTORY    = false;
		static constexpr bool DEFAULT_DIAMOND    = false;
		static constexpr int  DEFAULT_GEM_SCORE  = -1;
		static constexpr int  DEFAULT_TIME_SCORE = -1;
		static constexpr int  PRICE_OF_DEATH     = 2500;

		// Methods
		Inventory();
		Inventory( const Inventory& c );

		bool levelComplete( int level ) const;

		bool haveDiamond( int level ) const;
		void getDiamond( int level );

		std::string gemScore( int level ) const;
		void setGemScore( int level );
		bool gemChallengeBeaten( int level ) const;

		std::string timeScore( int level ) const;
		void setTimeScore( int level, int time );
		bool timeChallengeBeaten( int level ) const;

		std::vector<bool> victories_;
		bool victory( int level ) const;

		int fundsShown() const;
		int totalFundsShown() const;
		double percent() const;
		std::string percentShown() const;

		void update();
		void save();
		void load();


		// Members
		Counter funds_;
		Counter total_funds_;

		// Upgrades:
		int heart_upgrades_ = 0;
		bool oxygen_upgrade_ = false;


	private:
		static constexpr int FUNDS_MAX              = 99999;
		static constexpr int TIME_MAX               = ( 60 * 9 ) + 59;
		static constexpr int TOTAL_FUNDS_MAX        = 999999999;
		static constexpr int TOTAL_FUNDS_MIN        = -99999999;
		static constexpr int FUNDS_MAX_DIGITS       = 5;
		static constexpr int TOTAL_FUNDS_MAX_DIGITS = 9;
		static constexpr int FUNDS_SPEED            = 25;
		static constexpr int TOTAL_FUNDS_SPEED      = 100;

		std::vector<bool> diamonds_;
		std::vector<Counter> gem_scores_;
		std::vector<Counter> time_scores_;
		Counter funds_shown_;
		Counter total_funds_shown_;
		
		double percentPerLevel() const;
		double percentPerVictory() const;
		double percentPerDiamond() const;
		double percentPerScore() const;
		
		int levelsBeaten() const;
		int diamondsGotten() const;
		int gemChallengesWon() const;
		int timeChallengesWon() const;
		
		double totalVictoryPercents() const;
		double totalDiamondPercents() const;
		double totalGemChallengePercents() const;
		double totalTimeChallengePercents() const;

		void saveBinary();
		void loadBinary();
};