#pragma once



#include "counter.hpp"
#include "level.hpp"

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

		std::string levelName( int level ) const;
		
		int recentLevel() const;

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
		
		bool beenToLevel( int level ) const;
		void registerBeenToLevel( int level );

		int fundsShown() const;
		int totalFundsShown() const;
		double percent() const;
		std::string percentShown() const;
		
		void addMcGuffin();
		int McGuffins() const;
		
		void heal();
		void hurt();
		int hp() const;

		void update();
		void save();
		void load();

		// Members
		Counter funds_;
		Counter total_funds_;

		// Upgrades:
		Counter heart_upgrades_;
		bool oxygen_upgrade_;


	private:
		static constexpr int FUNDS_MAX              = 99999;
		static constexpr int TIME_MAX               = ( 60 * 9 ) + 59;
		static constexpr int TOTAL_FUNDS_MAX        = 999999999;
		static constexpr int TOTAL_FUNDS_MIN        = -99999999;
		static constexpr int FUNDS_MAX_DIGITS       = 5;
		static constexpr int TOTAL_FUNDS_MAX_DIGITS = 9;
		static constexpr int FUNDS_SPEED            = 25;
		static constexpr int TOTAL_FUNDS_SPEED      = 100;

		std::vector<bool> been_to_level_;
		std::vector<bool> diamonds_;
		std::vector<Counter> gem_scores_;
		std::vector<Counter> time_scores_;
		Counter funds_shown_;
		Counter total_funds_shown_;
		int recent_level_;
		int mcguffins_;
		Counter hp_;

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