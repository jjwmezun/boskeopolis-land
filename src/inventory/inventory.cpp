#include <cassert>
#include "clock.hpp"
#include "counter.hpp"
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
#include "text.hpp"
#include "title_state.hpp"
#include "wmessage_state.hpp"

namespace Inventory
{
	// Private Function Declarations
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

	static void saveBinary();

	static void generalVictory( const InventoryLevel& level_inventory );

	static void throwSaveCorruptionErrorMessage();

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

	static bool oxygen_upgrade_ = false;
	static int current_save_ = 0;
	static Counter total_funds_ = Counter( 0, TOTAL_FUNDS_MAX, TOTAL_FUNDS_MIN );
	static Counter total_funds_shown_ = Counter( 0, TOTAL_FUNDS_MAX, TOTAL_FUNDS_MIN );
	static Counter current_level_ = Counter( -1, Level::NUMBER_OF_LEVELS, -1 );
	static bool health_upgrades_[ MAX_HEART_UPGRADES ] = { false, false, false };
	static bool been_to_level_[ Level::NUMBER_OF_LEVELS ];
	static bool victories_[ Level::NUMBER_OF_LEVELS ];
	static bool secret_goals_[ Level::NUMBER_OF_LEVELS ];
	static bool diamonds_[ Level::NUMBER_OF_LEVELS ];
	static bool crowns_[ Level::NUMBER_OF_LEVELS ];
	static bool suits_[ Level::NUMBER_OF_LEVELS ];
	static int gem_scores_[ Level::NUMBER_OF_LEVELS ];
	static int time_scores_[ Level::NUMBER_OF_LEVELS ];


	// Function Implementations
	void reset()
	{
		total_funds_ = 0;
		for ( int i = 0; i < Level::NUMBER_OF_LEVELS; ++i )
		{
			been_to_level_[ i ] = victories_[ i ] = secret_goals_[ i ] = diamonds_[ i ] = false;
			gem_scores_[ i ] = DEFAULT_GEM_SCORE;
			time_scores_[ i ] = DEFAULT_TIME_SCORE;
		}
		total_funds_shown_ = 0;
		current_level_ = -1;
		oxygen_upgrade_ = false;
		for ( int i = 0; i < MAX_HEART_UPGRADES; ++i )
		{
			health_upgrades_[ i ] = false;
		}
		current_save_ = 0;
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
			hasCrown( level ) &&
			( !Level::hasSecretGoal( level ) || getSecretGoal( level ) ) &&
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

	void winGemScore( int funds )
	{
		if ( funds >= gem_scores_[ current_level_() ] )
		{
			setGemScore( current_level_(), funds );
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
			return Clock::timeToString2( time_scores_[ level ] );
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

	void winTimeScore( const Clock& clock )
	{
		if ( time_scores_[ current_level_() ] < 0 || clock.totalSeconds() <= time_scores_[ current_level_() ] )
		{
			setTimeScore( current_level_(), clock.totalSeconds() );
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

	bool getSecretGoal()
	{
		return secret_goals_[ current_level_() ];
	};

	bool getSecretGoal( int level )
	{
		return secret_goals_[ level ];
	};

	bool beenToLevel( int level )
	{
		return been_to_level_[ level ];
	};

	void levelStart( int level )
	{
		been_to_level_[ level ] = true;
		current_level_ = level;
		save();
	};

	void setCurrentLevel( int level )
	{
		current_level_ = level;
	};

	int totalFundsShown()
	{
		return total_funds_shown_();
	};

	int totalFunds()
	{
		return total_funds_();
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
		return levelsBeaten() * percentPerVictory();
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

	std::u32string percentShown()
	{
		return mezun::merge32Strings( mezun::intToChar32String( ( int )( std::floor( percent() ) ) ), U"%" );
	};

	void updateForOverworld()
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
	};

	void updateForShop()
	{
		updateForOverworld();
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
		std::ofstream binofs( Main::savePath( current_save_ ), std::ios::out | std::ios::binary );

			// Save total gems as 1st 4 bytes.
				int32_t total_funds_block = total_funds_();
				binofs.write( ( char* )( &total_funds_block ), sizeof( int32_t ) );

			// Align level victories & diamonds in 1 straight list o' bools,
			// victories followed straight after diamonds.
			// Then pack them into chars (single bytes) & save these.
				std::vector<bool> bools;

				for ( int vi = 0; vi < Level::NUMBER_OF_LEVELS; ++vi )
				{
					bools.push_back( victories_[ vi ] );
				}

				for ( int si = 0; si < Level::NUMBER_OF_LEVELS; ++si )
				{
					bools.push_back( secret_goals_[ si ] );
				}

				for ( int li = 0; li < Level::NUMBER_OF_LEVELS; ++li )
				{
					bools.push_back( been_to_level_[ li ] );
				}

				for ( int di = 0; di < Level::NUMBER_OF_LEVELS; ++di )
				{
					bools.push_back( diamonds_[ di ] );
				}

				for ( int hmi = 0; hmi < Level::NUMBER_OF_LEVELS; ++hmi )
				{
					bools.push_back( crowns_[ hmi ] );
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

					binofs.write( ( char* )( &c ), sizeof( unsigned char ) );
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

	bool load()
	{
		// Sorry this code is messy: I'm still working on it.
		// Also note that trying to load a save from a time when there were fewer levels than now will cause the program to crash due to the assertions.
		// Since levels are added & rearranged not in the order I add them, adding levels will mess up save files, anyway,
		// so there's no elegant fix to this.

		// It'd be better to try understanding this function as a whole than trying to litter it with confusing comments.
		// Basically, this game saves & loads 6 types o' data:
		// * Total gem count
		// * Levels beaten
		// * Levels played ( but not necessarily beaten )
		// * Levels where you collected a diamond.
		// * Levels where you collected a crown ( beat it in hard mode )
		// * Gem high scores for each level.
		// * Time (low) scores for each level.
		//
		// This method starts by getting the save binary as 1 big clump o' data & gets its size.
		// It then goes clumps o' data it expects to extract from the save data,
		// marked off by "current_block_start" & "current_block_end".
		// For each new clump, "current_block_start" is set to the previous "current_block_end",
		// while "current_block_end" is set to where I calculate the new clump to end.

		bool load_success = true;
		std::ifstream binifs ( Main::savePath( current_save_ ), std::ios::in | std::ios::binary | std::ios::ate );

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
					// Space for 4x bools for levels (victories, been-tos, diamonds, & crowns).
					// 8 bool (bits) fit in a byte--hence dividing by 8, rounding up to ensure there's always the minimum space needed.
					// C++ oddly makes int / int output an auto-floored int rather than a double, so a val needs to be forced as a double.
					int blocks_for_bools = ( int )ceil( ( ( double )( ( Level::NUMBER_OF_LEVELS * 5 ) ) ) / 8 );
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
								secret_goals_[ b - Level::NUMBER_OF_LEVELS ] = val;
							}
							else if ( b < Level::NUMBER_OF_LEVELS * 3 )
							{
								been_to_level_[ b - ( Level::NUMBER_OF_LEVELS * 2 ) ] = val;
							}
							else if ( b < Level::NUMBER_OF_LEVELS * 4 )
							{
								diamonds_[ b - ( Level::NUMBER_OF_LEVELS * 3 ) ] = val;
							}
							else if ( b < Level::NUMBER_OF_LEVELS * 5 )
							{
								crowns_[ b - ( Level::NUMBER_OF_LEVELS * 4 ) ] = val;
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

					if ( binsize < current_block_end )
					{
						throwSaveCorruptionErrorMessage();
						load_success = false;
						goto LOAD_CLEAN_UP;
					}

					for ( int gi = 0; gi < Level::NUMBER_OF_LEVELS; ++gi )
					{
						int32_t gblock;
						std::memcpy( &gblock, &bindata[ current_block_start + ( gi * sizeof( int32_t ) ) ], sizeof( int32_t ) );
						setGemScore( gi, gblock );
					}


				// TIME SCORE CLUMP
				{
					current_block_start = current_block_end;
					const int NUM_O_TIME_SCORE_BLOCKS = sizeof( int32_t ) * Level::NUMBER_OF_LEVELS;
					current_block_end += NUM_O_TIME_SCORE_BLOCKS;

					if ( binsize < current_block_end )
					{
						throwSaveCorruptionErrorMessage();
						load_success = false;
						goto LOAD_CLEAN_UP;
					}

					for ( int ti = 0; ti < Level::NUMBER_OF_LEVELS; ++ti )
					{
						int32_t tblock;
						std::memcpy( &tblock, &bindata[ current_block_start + ( ti * sizeof( int32_t ) ) ], sizeof( int32_t ) );
						setTimeScore( ti, tblock );
					}
				}

				// RECENT LEVEL
					current_block_start = current_block_end;
					current_block_end += sizeof( int32_t );

					if ( binsize < current_block_end )
					{
						throwSaveCorruptionErrorMessage();
						load_success = false;
						goto LOAD_CLEAN_UP;
					}

					int32_t recent_level;
					std::memcpy( &recent_level, &bindata[ current_block_start ], sizeof( int32_t ) );
					assert( recent_level >= 0 && recent_level < Level::NUMBER_OF_LEVELS );
					current_level_ = recent_level;


				LOAD_CLEAN_UP:
					delete[] bindata;
					binifs.close();
			}
			else
			{
				std::cout<<"ERROR: Could not load saves."<<std::endl;
			}

		binifs.close();
		return load_success;
	};

	void win( const InventoryLevel& level_inventory )
	{
		victories_[ current_level_() ] = true;
		generalVictory( level_inventory );
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

	void secretGoal( const InventoryLevel& level_inventory )
	{
		secret_goals_[ current_level_() ] = true;
		generalVictory( level_inventory );
	};

	void generalVictory( const InventoryLevel& level_inventory )
	{
		if ( level_inventory.isHardMode() )
		{
			crowns_[ current_level_() ] = true;
		}
		winGemScore( level_inventory.funds() );
		winTimeScore( level_inventory.clock() );
		total_funds_ += level_inventory.funds();
		save();
	}

	int heartUpgrades()
	{
		int upgrade_count = 0;
		for ( int i = 0; i < MAX_HEART_UPGRADES; ++i )
		{
			if ( health_upgrades_[ i ] )
			{
				++upgrade_count;
			}
		}
		return upgrade_count;
	};

	bool haveOxygenUpgrade()
	{
		return oxygen_upgrade_;
	};

	std::u32string totalFundsString()
	{
		return ( total_funds_shown_ < 0 )
			? mezun::intToChar32StringWithPadding( total_funds_shown_(), TOTAL_FUNDS_MAX_DIGITS )
			: mezun::merge32Strings( U" ", mezun::intToChar32StringWithPadding( total_funds_shown_(), TOTAL_FUNDS_MAX_DIGITS ) );
	};

	bool hasCrown( int level )
	{
		return crowns_[ level ];
	};

	bool levelUnlocked( int level )
	{
		return ( level > 0 ) ? victory( level - 1 ) : true;
	};

	std::string getPlayerCostume()
	{
		return "sprites/autumn-skirt.png";
	};

	void loseTotalFunds( int amount )
	{
		total_funds_ -= amount;
	};

	bool hasHPUpgrade( int number )
	{
		return health_upgrades_[ number ];
	};

	void giveOxygenUpgrade()
	{
		oxygen_upgrade_ = true;
	};

	void giveHPUpgrade( int number )
	{
		health_upgrades_[ number ] = true;
	};

	void throwSaveCorruptionErrorMessage()
	{
		Main::changeState
		(
			WMessageState::generateErrorMessage
			(
				Localization::getCurrentLanguage().getSaveCorruptionErrorMessage(),
				WMessageState::Type::CHANGE,
				std::unique_ptr<TitleState> ( new TitleState() )
			)
		);
	}
};
