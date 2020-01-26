#include <cassert>
#include "clock.hpp"
#include "counter.hpp"
#include "inventory.hpp"
#include <fstream>
#include "level.hpp"
#include "main.hpp"
#include "mezun_exceptions.hpp"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "text.hpp"

namespace Inventory
{
	// Private Function Declarations
	double percentPerLevel();
	double percentPerVictory();
	double percentPerDiamond();
	double percentPerScore();

	int levelsBeaten();
	int diamondsGotten();
	int gemChallengesWon();
	int timeChallengesWon();

	void setGemScore( int level, int value );
	void setTimeScore( int level, int value );
	void winGemScore();
	void winTimeScore();

	double totalVictoryPercents();
	double totalDiamondPercents();
	double totalGemChallengePercents();
	double totalTimeChallengePercents();

	void saveBinary();
	void loadBinary();

	bool testMultiples( int value );
	void addFundsForMultiplier( int value );


	// Private Variables
	static constexpr bool DEFAULT_VICTORY        = false;
	static constexpr bool DEFAULT_DIAMOND        = false;
	static constexpr int  DEFAULT_GEM_SCORE      = -1;
	static constexpr int  DEFAULT_TIME_SCORE     = -1;
	static constexpr int  PRICE_OF_DEATH         = 2500;
	static constexpr int  FUNDS_MAX              = 99999;
	static constexpr int  TIME_MAX               = ( 60 * 9 ) + 59;
	static constexpr int  TOTAL_FUNDS_MAX        = 999999999;
	static constexpr int  TOTAL_FUNDS_MIN        = -99999999;
	static constexpr int  FUNDS_MAX_DIGITS       = 5;
	static constexpr int  TOTAL_FUNDS_MAX_DIGITS = 9;
	static constexpr int  FUNDS_SPEED            = 25;
	static constexpr int  TOTAL_FUNDS_SPEED      = 100;
	static constexpr int  MAX_HEART_UPGRADES     = 3;
	static constexpr int  MAX_BOPS               = 8;

	static int mcguffins_ = 0;
	static bool oxygen_upgrade_ = false;
	static Counter funds_ = Counter( 0, FUNDS_MAX, 0 );
	static Counter total_funds_ = Counter( 0, TOTAL_FUNDS_MAX, TOTAL_FUNDS_MIN );
	static Counter funds_shown_ = Counter( 0, FUNDS_MAX, 0 );
	static Counter total_funds_shown_ = Counter( 0, TOTAL_FUNDS_MAX, TOTAL_FUNDS_MIN );
	static Counter current_level_ = Counter( -1, Level::NUMBER_OF_LEVELS, -1 );
	static Counter heart_upgrades_ = Counter( 0, MAX_HEART_UPGRADES );

	static bool been_to_level_[ Level::NUMBER_OF_LEVELS ];
	static bool victories_[ Level::NUMBER_OF_LEVELS ];
	static bool diamonds_[ Level::NUMBER_OF_LEVELS ];
	static int gem_scores_[ Level::NUMBER_OF_LEVELS ];
	static int time_scores_[ Level::NUMBER_OF_LEVELS ];

	static Clock clock_ = Clock();

	static int bops_ = 0;
	static int ghost_kills_ = 0;


	// Function Implementations
	void reset()
	{
		funds_ = 0;
		total_funds_ = 0;
		clock_.reset();

		for ( int i = 0; i < Level::NUMBER_OF_LEVELS; ++i )
		{
			been_to_level_[ i ] = victories_[ i ] = diamonds_[ i ] = false;
			gem_scores_[ i ] = DEFAULT_GEM_SCORE;
			time_scores_[ i ] = DEFAULT_TIME_SCORE;
		}

		funds_shown_ = 0;
		total_funds_shown_ = 0;
		current_level_ = -1;
		heart_upgrades_ = 0;
		mcguffins_ = 0;
		oxygen_upgrade_ = false;
		bops_ = 0;
	};

	std::string levelName( int level )
	{
		const std::string name = Level::NameOLevel( level );

		if ( name != "" )
		{
			if ( beenToLevel( level ) )
			{
				return Level::NameOLevel( level );
			}
			else
			{
				char question[ 33 ] = { '\0' };
				for ( int i = 0; i < name.length(); ++i )
				{
					question[ i ] = '?';
				}
				return std::string( question );
			}
		}
		else
		{
			return "";
		}
	};

	int currentLevel() { return current_level_(); };

	bool levelComplete( int level )
	{
		return haveDiamond( level ) &&
			victory( level ) &&
			gemChallengeBeaten( level ) &&
			timeChallengeBeaten( level );
	};

	bool haveDiamond()
	{
		return diamonds_[ current_level_() ];
	};

	bool haveDiamond( int level )
	{
		return diamonds_[ level ];
	};

	void getDiamond()
	{
		diamonds_[ current_level_() ] = true;
		save();
	};

	std::string gemScore( int level )
	{
		if ( gem_scores_[ level ] > DEFAULT_GEM_SCORE )
		{
			return Text::formatNumDigitPadding( gem_scores_[ level ], FUNDS_MAX_DIGITS );
		}
		else
		{
			return "-----";
		}
	};

	void setGemScore( int level, int value )
	{
		if ( value > -1 && value < FUNDS_MAX )
		{
			gem_scores_[ level ] = value;
		}
	};

	void winGemScore()
	{
		if ( funds_ >= gem_scores_[ current_level_() ] )
		{
			setGemScore( current_level_(), funds_() );
		}
	};

	bool gemChallengeBeaten( int level )
	{
		return gem_scores_[ level ] >= Level::gemChallenge( level );
	}

	std::string timeScore( int level )
	{
		if ( time_scores_[ level ] > DEFAULT_GEM_SCORE )
		{
			return Clock::timeToString( time_scores_[ level ] );
		}
		else
		{
			return "----";
		}
	};

	void setTimeScore( int level, int value )
	{
		if ( value > -1 && value < TIME_MAX )
		{
			time_scores_[ level ] = value;
		}
	};

	void winTimeScore()
	{
		if ( time_scores_[ current_level_() ] < 0 || clock_.totalSeconds() <= time_scores_[ current_level_() ] )
		{
			setTimeScore( current_level_(), clock_.totalSeconds() );
		}
	};

	bool timeChallengeBeaten( int level )
	{
		return time_scores_[ level ] > 0 && time_scores_[ level ] <= Level::timeChallenge( level );
	}

	bool victory()
	{
		return victories_[ current_level_() ];
	};

	bool victory( int level )
	{
		return victories_[ level ];
	};

	bool beenToLevel( int level )
	{
		return been_to_level_[ level ];
	};

	void levelStart( int level )
	{
		been_to_level_[ level ] = true;
		setCurrentLevel( level );
		mcguffins_ = 0;
		funds_ = 0;
		funds_shown_ = 0;
		bops_ = 0;
		clock_.reset();
		save();
	};

	void setCurrentLevel( int level )
	{
		current_level_ = level;
	};

	int fundsShown()
	{
		return funds_shown_();
	};

	int totalFundsShown()
	{
		return total_funds_shown_();
	};

	double percentPerLevel()
	{
		return 100.0 / ( double )( Level::NUMBER_OF_LEVELS );
	};

	double percentPerVictory()
	{
		return percentPerLevel() * 0.5;
	};

	double percentPerDiamond()
	{
		return percentPerLevel() * 0.3;
	};

	double percentPerScore()
	{
		return percentPerLevel() * 0.1;
	};

	int levelsBeaten()
	{
		int n = 0;

		for ( auto b : victories_ )
		{
			if ( b )
			{
				++n;
			}
		}

		return n;
	};

	int diamondsGotten()
	{
		int n = 0;

		for ( auto b : diamonds_ )
		{
			if ( b )
			{
				++n;
			}
		}

		return n;
	};

	int gemChallengesWon()
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

	int timeChallengesWon()
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

	double totalVictoryPercents()
	{
		return levelsBeaten() * percentPerLevel();
	};

	double totalDiamondPercents()
	{
		return diamondsGotten() * percentPerDiamond();
	};

	double totalGemChallengePercents()
	{
		return gemChallengesWon() * percentPerScore();
	};

	double totalTimeChallengePercents()
	{
		return timeChallengesWon() * percentPerScore();
	};

	double percent()
	{
		// Divide 100 by # o' levels
		// Divide each level percent into 4 types o' completion:
		// * Beaten         -> 50%
		// * Diamond        -> 30%
		// * Gem Challenge  -> 10%
		// * Time Challenge -> 10%

		return totalVictoryPercents() + totalDiamondPercents() + totalGemChallengePercents() + totalTimeChallengePercents();
	};

	std::string percentShown()
	{
		return std::to_string( ( int )( std::floor( percent() ) ) ) + "%";
	};

	void update()
	{
		if ( total_funds_shown_ < total_funds_ )
		{
			if ( total_funds_ - total_funds_shown_ < TOTAL_FUNDS_SPEED )
				total_funds_shown_ = total_funds_;
			else
				total_funds_shown_ += TOTAL_FUNDS_SPEED;
		}
		else if ( total_funds_shown_ > total_funds_ )
		{
			if ( total_funds_shown_ - total_funds_ < TOTAL_FUNDS_SPEED )
				total_funds_shown_ = total_funds_;
			else
				total_funds_shown_ -= TOTAL_FUNDS_SPEED;
		}

		if ( funds_shown_ < funds_ )
		{
			funds_shown_ += FUNDS_SPEED;
		}
		else if ( funds_shown_ > funds_ )
		{
			funds_shown_ -= FUNDS_SPEED;
		}
		clock_.update();
	};

	void save()
	{
		if ( !Main::nosave() )
		{
			saveBinary();
		}
	};

	void saveBinary()
	{
		std::ofstream binofs( Main::savePath(), std::ios::out | std::ios::binary );

			// Save total gems as 1st 4 bytes.
				int32_t total_funds_block = total_funds_();
				binofs.write( (char*)&total_funds_block, sizeof( int32_t ) );

			// Align level victories & diamonds in 1 straight list o' bools,
			// victories followed straight after diamonds.
			// Then pack them into chars (single bytes) & save these.
				std::vector<bool> bools;

				for ( int vi = 0; vi < Level::NUMBER_OF_LEVELS; ++vi )
				{
					bools.push_back( victories_[ vi ] );
				}

				for ( int li = 0; li < Level::NUMBER_OF_LEVELS; ++li )
				{
					bools.push_back( been_to_level_[ li ] );
				}

				for ( int di = 0; di < Level::NUMBER_OF_LEVELS; ++di )
				{
					bools.push_back( diamonds_[ di ] );
				}

				// Packs 8 bools into single byte.
				// Though this saves a few measely bytes,
				// the true reason for this is simply to make editing the save file a li'l harder.
				for ( int bi = 0; bi < bools.size(); bi += 8 )
				{
					unsigned char c = 0;

					for ( int bit = 0; bit < 8; ++bit )
					{
						int i = bi + bit;

						if ( i < bools.size() )
						{
							if ( bools.at( i ) )
							{
								c |= 1 << bit;
							}
						}
					}

					binofs.write( (char*)&c, sizeof( unsigned char ) );
				}

			// Save gem scores as 4-byte ints each.
				for ( int gi = 0; gi < Level::NUMBER_OF_LEVELS; ++gi )
				{
					int32_t gb = gem_scores_[ gi ];
					binofs.write( (char*)&gb, sizeof( gb ) );
				}

			// Save time scores as 4-byte ints each.
				for ( int ti = 0; ti < Level::NUMBER_OF_LEVELS; ++ti )
				{
					int32_t tb = time_scores_[ ti ];
					binofs.write( (char*)&tb, sizeof( tb ) );
				}

			// Save last level entered.
				int32_t rl = current_level_();
				binofs.write( (char*)&rl, sizeof( rl ) );

		binofs.close();
	};

	void load()
	{
		loadBinary();
	}


	void loadBinary()
	{
		// Sorry this code is messy: I'm still working on it.
		// Also note that trying to load a save from a time when there were fewer levels than now will cause the program to crash due to the assertions.
		// Since levels are added & rearranged not in the order I add them, adding levels will mess up save files, anyway,
		// so there's no elegant fix to this.

		// It'd be better to try understanding this function as a whole than trying to litter it with confusing comments.
		// Basically, this game saves & loads 4 types o' data:
		// * Total gem count
		// * Levels beaten
		// * Levels where you collected a diamond.
		// * Gem high scores for each level.
		// * Time (low) scores for each level.
		//
		// This method starts by getting the save binary as 1 big clump o' data & gets its size.
		// It then goes clumps o' data it expects to extract from the save data,
		// marked off by "current_block_start" & "current_block_end".
		// For each new clump, "current_block_start" is set to the previous "current_block_end",
		// while "current_block_end" is set to where I calculate the new clump to end.

		std::ifstream binifs ( Main::savePath(), std::ios::in | std::ios::binary | std::ios::ate );

			if ( binifs.is_open() )
			{
				int binsize = binifs.tellg();

				char* bindata = new char [ binsize ];
				binifs.seekg ( 0, std::ios::beg );
				binifs.read ( bindata, binsize );


				// TOTAL GEM CLUMP ( 0 - 4 bytes )
					int current_block_start = 0;
					int current_block_end = sizeof( int32_t );

					assert( binsize >= current_block_end );

					int32_t total_funds_block;
					std::memcpy( &total_funds_block, bindata, sizeof( int32_t ) );
					total_funds_ = total_funds_block;
					total_funds_shown_ = total_funds_;


				// VICTORIES, BEEN_TO_LEVEL, & DIAMONDS CLUMP.
					current_block_start = current_block_end;
					// Space for 2x bools for levels (victories & diamonds).
					// 8 bool (bits) fit in a byte--hence dividing by 8, rounding up to ensure there's always the minimum space needed.
					// C++ oddly makes int / int output an auto-floored int rather than a double, so a val needs to be forced as a double.
					int blocks_for_bools = ( int )ceil( ( ( double )( ( Level::NUMBER_OF_LEVELS * 3 ) ) ) / 8 );
					current_block_end += blocks_for_bools;

					assert( binsize >= current_block_end );

					for ( int i = 0; i < blocks_for_bools; ++i )
					{
						unsigned char c;
						std::memcpy( &c, &bindata[ 4 + i ], sizeof( unsigned char ) );

						for ( int bit = 0; bit < 8; ++bit )
						{
							int b = ( i * 8 ) + bit;
							bool val = ( c & ( 1 << bit ) ) != 0;

							if ( b < Level::NUMBER_OF_LEVELS )
							{
								victories_[ b ] = val;
							}
							else if ( b < Level::NUMBER_OF_LEVELS * 2 )
							{
								been_to_level_[ b - Level::NUMBER_OF_LEVELS ] = val;
							}
							else if ( b < Level::NUMBER_OF_LEVELS * 3 )
							{
								diamonds_[ b - ( Level::NUMBER_OF_LEVELS * 2 ) ] = val;
							}
							else
							{
								break;
							}
						}
					}


				// GEM SCORE CLUMP
					current_block_start = current_block_end;
					const int NUM_O_GEM_SCORE_BLOCKS = sizeof( int32_t ) * Level::NUMBER_OF_LEVELS;
					current_block_end += NUM_O_GEM_SCORE_BLOCKS;

					assert( binsize >= current_block_end );

					for ( int gi = 0; gi < Level::NUMBER_OF_LEVELS; ++gi )
					{
						int32_t gblock;
						std::memcpy( &gblock, &bindata[ current_block_start + ( gi * sizeof( int32_t ) ) ], sizeof( int32_t ) );
						setGemScore( gi, gblock );
					}


				// TIME SCORE CLUMP
					current_block_start = current_block_end;
					const int NUM_O_TIME_SCORE_BLOCKS = sizeof( int32_t ) * Level::NUMBER_OF_LEVELS;
					current_block_end += NUM_O_TIME_SCORE_BLOCKS;

					assert( binsize >= current_block_end );

					for ( int ti = 0; ti < Level::NUMBER_OF_LEVELS; ++ti )
					{
						int32_t tblock;
						std::memcpy( &tblock, &bindata[ current_block_start + ( ti * sizeof( int32_t ) ) ], sizeof( int32_t ) );
						setTimeScore( ti, tblock );
					}

				// RECENT LEVEL
					current_block_start = current_block_end;
					current_block_end += sizeof( int32_t );

					assert( binsize >= current_block_end );

					int32_t recent_level;
					std::memcpy( &recent_level, &bindata[ current_block_start ], sizeof( int32_t ) );
					assert( recent_level >= 0 && recent_level < Level::NUMBER_OF_LEVELS );
					current_level_ = recent_level;


				// Clean up time.
					delete[] bindata;
					binifs.close();
			}
			else
			{
				std::cout<<"ERROR: Could not load saves."<<std::endl;
			}

		binifs.close();
	};

	void addMcGuffin()
	{
		++mcguffins_;
	};

	int McGuffins() { return mcguffins_; };

	void win()
	{
		victories_[ current_level_() ] = true;
		winGemScore();
		winTimeScore();

		total_funds_ += funds_;
		funds_ = 0;

		save();
	};

	void fail()
	{
		total_funds_ -= PRICE_OF_DEATH;
		save();
	};

	void quit()
	{
		if ( !victory() )
		{
			fail();
		}
	};

	void addFunds( int n )
	{
		funds_ += n;
	};

	void loseFunds( int n )
	{
		funds_ -= n;
	};

	int funds()
	{
		return funds_();
	};

	void setFunds( int n )
	{
		funds_ = n;
	};

	Clock& clock()
	{
		return clock_;
	};

	int heartUpgrades()
	{
		return heart_upgrades_();
	};

	bool haveOxygenUpgrade()
	{
		return oxygen_upgrade_;
	};

	void bop()
	{
		++bops_;
		if ( bopsMultiplier() )
		{
			addFundsForMultiplier( bops_ );
		}
	};

	bool bopsMultiplier()
	{
		return testMultiples( bops_ );
	};

	void clearBops()
	{
		bops_ = 0;
	};

	int howManyBops()
	{
		return bops_;
	};

	void addGhostKill()
	{
		++ghost_kills_;
		if ( multipleGhostKills() )
		{
			addFundsForMultiplier( ghost_kills_ );
		}
	};

	void clearGhostKills()
	{
		ghost_kills_ = 0;
	};

	int howManyGhostKills()
	{
		return ghost_kills_;
	};

	bool multipleGhostKills()
	{
		return testMultiples( ghost_kills_ );
	};

	bool testMultiples( int value )
	{
		return value > 1 && value <= MAX_BOPS;
	};

	void addFundsForMultiplier( int value )
	{
		//  2    3    4    5     6      7      8
		// 100, 200, 400, 800, 1,600, 3,200, 6,400
		addFunds( ( int )( 100 * pow( 2, value - 2 ) ) );
	}
};
