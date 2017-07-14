#include "clock.h"
#include "game.h"
#include "inventory.h"
#include <fstream>
#include "level.h"
#include "mezun_exceptions.h"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "text.h"

Inventory::Inventory()
:
	funds_ ( Counter( 0, FUNDS_MAX, 0 ) ),
	total_funds_ ( Counter( 0, TOTAL_FUNDS_MAX, TOTAL_FUNDS_MIN ) ),
	funds_shown_ ( Counter( 0, FUNDS_MAX, 0 ) ),
	total_funds_shown_ ( Counter( 0, TOTAL_FUNDS_MAX, TOTAL_FUNDS_MIN ) ),
	recent_level_ ( NULL )
{

	if ( victories_.empty() )
	{
		for ( int i = 0; i < Level::NUM_O_LEVELS; ++i )
		{
			been_to_level_.push_back( false );
			victories_.push_back( DEFAULT_VICTORY );
			diamonds_.push_back( DEFAULT_DIAMOND );
			gem_scores_.push_back( { DEFAULT_GEM_SCORE, FUNDS_MAX, -1 } );
			time_scores_.push_back( { DEFAULT_TIME_SCORE, TIME_MAX, -1 } );
		}
	}

};

Inventory::Inventory( const Inventory& c )
:
	funds_ ( c.funds_ ),
	total_funds_ ( c.total_funds_ ),
	been_to_level_ ( c.been_to_level_ ),
	victories_ ( c.victories_ ),
	diamonds_ ( c.diamonds_ ),
	gem_scores_ ( c.gem_scores_ ),
	time_scores_ ( c.time_scores_ ),
	funds_shown_ ( { 0, FUNDS_MAX, 0 } ),
	total_funds_shown_ ( c.total_funds_shown_ ),
	recent_level_ ( c.recent_level_ )
{};

std::string Inventory::levelName( int level ) const
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
			return "????????????????????";
		}
	}
	else
	{
		return "";
	}
};
		
int Inventory::recentLevel() const { return recent_level_; };

bool Inventory::levelComplete( int level ) const
{
	return haveDiamond( level ) &&
		victory( level ) &&
		gemChallengeBeaten( level ) &&
		timeChallengeBeaten( level );
};

bool Inventory::haveDiamond( int level ) const
{
	return diamonds_.at( level );
};

void Inventory::getDiamond( int level )
{
	diamonds_.at( level ) = true;
	save();
};

std::string Inventory::gemScore( int level ) const
{
	if ( gem_scores_.at( level ) > DEFAULT_GEM_SCORE )
	{
		return Text::formatNumDigitPadding( gem_scores_.at( level )(), FUNDS_MAX_DIGITS );
	}
	else
	{
		return "-----";
	}
};

void Inventory::setGemScore( int level )
{
	if ( funds_ >= gem_scores_.at( level ) )
	{
		gem_scores_.at( level ) = funds_;
	}
};

bool Inventory::gemChallengeBeaten( int level ) const
{
	return gem_scores_.at( level )() >= Level::gemChallenge( level );
}

std::string Inventory::timeScore( int level ) const
{
	if ( time_scores_.at( level ) > DEFAULT_GEM_SCORE )
	{
		return Clock::timeToString( time_scores_.at( level )() );
	}
	else
	{
		return "----";
	}
};

void Inventory::setTimeScore( int level, int time )
{
	if ( time_scores_.at( level ) < 0 || time <= time_scores_.at( level )() )
	{
		time_scores_.at( level ) = time;
	}
};

bool Inventory::timeChallengeBeaten( int level ) const
{
	return time_scores_.at( level )() > 0 && time_scores_.at( level )() <= Level::timeChallenge( level );
}

bool Inventory::victory( int level ) const
{
	return victories_.at( level );
};
		
bool Inventory::beenToLevel( int level ) const
{
	return been_to_level_.at( level );
};

void Inventory::registerBeenToLevel( int level )
{
	been_to_level_.at( level ) = true;
	recent_level_ = level;
	save();
};

int Inventory::fundsShown() const
{
	return funds_shown_();
};

int Inventory::totalFundsShown() const
{
	return total_funds_shown_();
};
		
double Inventory::percentPerLevel() const
{
	return 100.0 / ( double )( Level::realLevelNum() );
};

double Inventory::percentPerVictory() const
{
	return percentPerLevel() * 0.5;
};

double Inventory::percentPerDiamond() const
{
	return percentPerLevel() * 0.3;
};

double Inventory::percentPerScore() const
{
	return percentPerLevel() * 0.1;
};

int Inventory::levelsBeaten() const
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

int Inventory::diamondsGotten() const
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

int Inventory::gemChallengesWon() const
{
	int n = 0;

	for ( int i = 0; i < Level::NUM_O_LEVELS; ++i )
	{
		if ( gemChallengeBeaten( i ) )
		{
			++n;
		}
	}

	return n;
};

int Inventory::timeChallengesWon() const
{
	int n = 0;

	for ( int i = 0; i < Level::NUM_O_LEVELS; ++i )
	{
		if ( timeChallengeBeaten( i ) )
		{
			++n;
		}
	}

	return n;
};
		
double Inventory::totalVictoryPercents() const
{
	return levelsBeaten() * percentPerLevel();
};

double Inventory::totalDiamondPercents() const
{
	return diamondsGotten() * percentPerDiamond();
};

double Inventory::totalGemChallengePercents() const
{
	return gemChallengesWon() * percentPerScore();
};

double Inventory::totalTimeChallengePercents() const
{
	return timeChallengesWon() * percentPerScore();
};

double Inventory::percent() const
{
	// Divide 100 by # o' levels
	// Divide each level percent into 4 types o' completion:
	// * Beaten         -> 50%
	// * Diamond        -> 30%
	// * Gem Challenge  -> 10%
	// * Time Challenge -> 10%
	
	return totalVictoryPercents() + totalDiamondPercents() + totalGemChallengePercents() + totalTimeChallengePercents();
};

std::string Inventory::percentShown() const
{
	return Text::stringifyNum( percent() ) + "%";
};

void Inventory::update()
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

};

void Inventory::save()
{
	if ( Game::savingAllowed() )
	{
		saveBinary();
	}
};

void Inventory::saveBinary()
{
	std::ofstream binofs( Game::savePath(), std::ios::out | std::ios::binary );

		// Save total gems as 1st 4 bytes.
			int32_t total_funds_block = total_funds_();
			binofs.write( (char*)&total_funds_block, sizeof( int32_t ) );

		// Align level victories & diamonds in 1 straight list o' bools,
		// victories followed straight after diamonds.
		// Then pack them into chars (single bytes) & save these.
			std::vector<bool> bools;

			for ( int vi = 0; vi < victories_.size(); ++vi )
			{
				bool vb = victories_.at( vi );
				bools.push_back( vb );
			}

			for ( int li = 0; li < been_to_level_.size(); ++li )
			{
				bool lb = been_to_level_.at( li );
				bools.push_back( lb );
			}

			for ( int di = 0; di < diamonds_.size(); ++di )
			{
				bool db = diamonds_.at( di );
				bools.push_back( db );
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
						if ( bools.at ( i ) )
						{
							c |= 1 << bit;
						}
					}
				}

				binofs.write( (char*)&c, sizeof( unsigned char ) );
			}

		// Save gem scores as 4-byte ints each.
			for ( int gi = 0; gi < gem_scores_.size(); ++gi )
			{
				int32_t gb = gem_scores_.at( gi )();
				binofs.write( (char*)&gb, sizeof( gb ) );
			}

		// Save time scores as 4-byte ints each.
			for ( int ti = 0; ti < time_scores_.size(); ++ti )
			{
				int32_t tb = time_scores_.at( ti )();
				binofs.write( (char*)&tb, sizeof( tb ) );
			}
	
		// Save last level entered.
			int32_t rl = recent_level_;
			binofs.write( (char*)&rl, sizeof( rl ) );

	binofs.close();
};

void Inventory::load()
{
	loadBinary();
}


void Inventory::loadBinary()
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

	std::ifstream binifs ( Game::savePath(), std::ios::in | std::ios::binary | std::ios::ate );

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
				int blocks_for_bools = ( int )ceil( ( ( double )( ( Level::NUM_O_LEVELS * 3 ) ) ) / 8 );
				current_block_end += blocks_for_bools;

				assert( binsize >= current_block_end );

				std::vector<bool> v;
				std::vector<bool> l;
				std::vector<bool> d;

				for ( int i = 0; i < blocks_for_bools; ++i )
				{
					unsigned char c;
					std::memcpy( &c, &bindata[ 4 + i ], sizeof( unsigned char ) );

					for ( int bit = 0; bit < 8; ++bit )
					{
						int b = ( i * 8 ) + bit;
						bool val = ( c & ( 1 << bit ) ) != 0;

						if ( b < Level::NUM_O_LEVELS )
						{
							v.push_back( val );
						}
						else if ( b < Level::NUM_O_LEVELS * 2 )
						{
							l.push_back( val );
						}
						else if ( b < Level::NUM_O_LEVELS * 3 )
						{
							d.push_back( val );
						}
						else
						{
							break;
						}
					}
				}

				victories_ = v;
				been_to_level_ = l;
				diamonds_ = d;


			// GEM SCORE CLUMP
				current_block_start = current_block_end;
				const int NUM_O_GEM_SCORE_BLOCKS = sizeof( int32_t ) * Level::NUM_O_LEVELS;
				current_block_end += NUM_O_GEM_SCORE_BLOCKS;

				assert( binsize >= current_block_end );

				std::vector<Counter> glist;

				for ( int gi = 0; gi < Level::NUM_O_LEVELS; ++gi )
				{
					int32_t gblock;
					std::memcpy( &gblock, &bindata[ current_block_start + ( gi * sizeof( int32_t ) ) ], sizeof( int32_t ) );
					glist.push_back( { gblock, FUNDS_MAX, -1 } );
				}

				gem_scores_ = glist;


			// TIME SCORE CLUMP
				current_block_start = current_block_end;
				const int NUM_O_TIME_SCORE_BLOCKS = sizeof( int32_t ) * Level::NUM_O_LEVELS;
				current_block_end += NUM_O_TIME_SCORE_BLOCKS;

				try
				{
					mezun::testSaveSize( binsize, current_block_end );
				}
				catch( const mezun::InvalidSaveSizeException& e )
				{
					throw;
				}

				std::vector<Counter> tlist;

				for ( int ti = 0; ti < Level::NUM_O_LEVELS; ++ti )
				{
					int32_t tblock;
					std::memcpy( &tblock, &bindata[ current_block_start + ( ti * sizeof( int32_t ) ) ], sizeof( int32_t ) );
					tlist.push_back( { tblock, TIME_MAX, -1 } );
				}

				time_scores_ = tlist;
			
			// RECENT LEVEL
				current_block_start = current_block_end;
				current_block_end += sizeof( int32_t );

				try
				{
					mezun::testSaveSize( binsize, current_block_end );
				}
				catch( const mezun::InvalidSaveSizeException& e )
				{
					throw;
				}
			
				int32_t recent_level;
				std::memcpy( &recent_level, &bindata[ current_block_start ], sizeof( int32_t ) );
				recent_level_ = recent_level;


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