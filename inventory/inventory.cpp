




// Name
//===================================
//
// Inventory
//


// DEPENDENCIES
//===================================

    #include "clock.h"
    #include "game.h"
    #include "inventory.h"
    #include <fstream>
    #include "rapidjson/document.h"
    #include "rapidjson/istreamwrapper.h"
    #include "text.h"


// STATIC PROPERTIES
//===================================

// METHODS
//===================================

    Inventory::Inventory()
    :
        funds_ ( Counter( 0, FUNDS_MAX, 0 ) ),
        total_funds_ ( Counter( 0, TOTAL_FUNDS_MAX, TOTAL_FUNDS_MIN ) ),
        //victories_ ( { DEFAULT_VICTORY } ),
        //diamonds_ ( { DEFAULT_DIAMOND } ),
        //gem_scores_ ( { DEFAULT_GEM_SCORE-1 } ),
        //time_scores_ ( { DEFAULT_TIME_SCORE } ),
		funds_shown_ ( Counter( 0, FUNDS_MAX, 0 ) ),
		total_funds_shown_ ( Counter( 0, TOTAL_FUNDS_MAX, TOTAL_FUNDS_MIN ) )
	{
        /*
        // DUMB WAY TO INIT IF NOT ALREADY INIT, 'CAUSE STD::ARRAY IS FUCKING STUPID.
        if ( gem_scores_.at( 0 ) == DEFAULT_GEM_SCORE-1 )
        {
            for ( auto& v : victories_ )
            {
                v = DEFAULT_VICTORY;
            }

            for ( auto& d : diamonds_ )
            {
                d = DEFAULT_DIAMOND;
            }

            for ( auto& g : gem_scores_ )
            {
                g = DEFAULT_GEM_SCORE;
            }

            for ( auto& t : time_scores_ )
            {
                t = DEFAULT_TIME_SCORE;
            }
        }*/

        if ( victories_.empty() )
        {
            for ( int i = 0; i < Level::NUM_O_LEVELS; ++i )
            {
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
        victories_ ( c.victories_ ),
        diamonds_ ( c.diamonds_ ),
        gem_scores_ ( c.gem_scores_ ),
        time_scores_ ( c.time_scores_ ),
		funds_shown_ ( { 0, FUNDS_MAX, 0 } ),
		total_funds_shown_ ( c.total_funds_shown_ )
    {};

	bool Inventory::haveDiamond( Level::LevelName level ) const
	{
		return diamonds_.at( level );
	};

	void Inventory::getDiamond( Level::LevelName level )
	{
		diamonds_.at( level ) = true;
		save();
	};

	std::string Inventory::gemScore( Level::LevelName level ) const
	{
		if ( gem_scores_.at( level ) > DEFAULT_GEM_SCORE )
		{
			return Text::formatNumDigitPadding( gem_scores_.at( level )(), FUNDS_MAX_DIGITS );
		}
		else
		{
			return "-";
		}
	};

	void Inventory::setGemScore( Level::LevelName level )
	{
		if ( funds_ >= gem_scores_.at( level ) )
		{
            gem_scores_.at( level ) = funds_;
		}
	};

	std::string Inventory::timeScore( Level::LevelName level ) const
	{
		if ( time_scores_.at( level ) > DEFAULT_GEM_SCORE )
		{
			return Clock::timeToString( time_scores_.at( level )() );
		}
		else
		{
			return "-";
		}
	};

	void Inventory::setTimeScore( Level::LevelName level, int time )
	{
		if ( time_scores_.at( level ) < 0 || time <= time_scores_.at( level )() )
		{
			time_scores_.at( level ) = time;
		}
	};

    bool Inventory::victory( Level::LevelName level ) const
    {
        return victories_.at( level );
    };

	int Inventory::fundsShown() const
	{
		return funds_shown_();
	};

	int Inventory::totalFundsShown() const
	{
		return total_funds_shown_();
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
            //saveJSON();
            saveBinary();
        }
	};

    void Inventory::saveJSON()
    {
        std::ofstream ofs( Game::savePath() );
        ofs << "{";

            ofs << "\"total_funds\":";
            ofs << total_funds_();
            ofs << ",";

            ofs << "\"victories\":";
            ofs << "[";

            for ( int vi = 0; vi < victories_.size(); ++vi )
            {
                if ( vi != 0 )
                {
                    ofs << ",";
                }

                ofs << victories_.at( vi );
            }

            ofs << "],";

            ofs << "\"diamonds\":";
            ofs << "[";

            for ( int di = 0; di < diamonds_.size(); ++di )
            {
                if ( di != 0 )
                {
                    ofs << ",";
                }

                ofs << diamonds_.at( di );
            }

            ofs << "],";

            ofs << "\"gem_scores\":";
            ofs << "[";

            for ( int gi = 0; gi < gem_scores_.size(); ++gi )
            {
                if ( gi != 0 )
                {
                    ofs << ",";
                }

                ofs << gem_scores_.at( gi )();
            }

            ofs << "],";

            ofs << "\"time_scores\":";
            ofs << "[";

            for ( int ti = 0; ti < time_scores_.size(); ++ti )
            {
                if ( ti != 0 )
                {
                    ofs << ",";
                }

                ofs << time_scores_.at( ti )();
            }

            ofs << "]";

        ofs << "}";
        ofs.close();
    };

    void Inventory::saveBinary()
    {
        std::ofstream binofs( Game::savePath(), std::ios::out | std::ios::binary );

            int32_t total_funds_block = total_funds_();
            binofs.write( (char*)&total_funds_block, sizeof( int32_t ) );

            std::vector<bool> bools;

            for ( int vi = 0; vi < victories_.size(); ++vi )
            {
                bool vb = victories_.at( vi );
                bools.push_back( vb );
                //binofs.write( (char*)&vb, sizeof( vb ) );
            }

            for ( int di = 0; di < diamonds_.size(); ++di )
            {
                bool db = diamonds_.at( di );
                bools.push_back( db );
                //binofs.write( (char*)&db, sizeof( db ) );
            }

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

            for ( int gi = 0; gi < gem_scores_.size(); ++gi )
            {
                int32_t gb = gem_scores_.at( gi )();
                binofs.write( (char*)&gb, sizeof( gb ) );
            }

            for ( int ti = 0; ti < time_scores_.size(); ++ti )
            {
                int32_t tb = time_scores_.at( ti )();
                binofs.write( (char*)&tb, sizeof( tb ) );
            }

        binofs.close();
    };

    void Inventory::load()
    {
        //loadJSON();
        loadBinary();
    }

	void Inventory::loadJSON()
	{
        std::ifstream ifs( Game::savePath() );

        assert( ifs.is_open() );

        rapidjson::IStreamWrapper isw( ifs );
        rapidjson::Document d;
        d.ParseStream( isw );

        if( d.IsObject() )
        {
            assert ( d.HasMember( "total_funds" ) );
            assert ( d.HasMember( "victories" ) );
            assert ( d.HasMember( "gem_scores" ) );
            assert ( d.HasMember( "time_scores" ) );
            assert ( d[ "total_funds" ].IsInt() );
            assert ( d[ "victories" ].IsArray() );
            assert ( d[ "gem_scores" ].IsArray() );
            assert ( d[ "time_scores" ].IsArray() );

            total_funds_ = d[ "total_funds" ].GetInt();

            int vi = 0;
            for ( auto& v : d[ "victories" ].GetArray() )
            {
                victories_.at( vi ) = ( v.GetInt() == 1 ) ? true : false;
                ++vi;
            }

            int di = 0;
            for ( auto& dm : d[ "diamonds" ].GetArray() )
            {
                diamonds_.at( di ) = ( dm.GetInt() == 1 ) ? true : false;
                ++di;
            }

            int gi = 0;
            for ( auto& g : d[ "gem_scores" ].GetArray() )
            {
                gem_scores_.at( gi ) = g.GetInt();
                ++gi;
            }

            int ti = 0;
            for ( auto& t : d[ "time_scores" ].GetArray() )
            {
                time_scores_.at( ti ) = t.GetInt();
                ++ti;
            }
        }

        ifs.close();
	};


    void Inventory::loadBinary()
    {
        std::ifstream binifs ( Game::savePath(), std::ios::in | std::ios::binary | std::ios::ate );

            if ( binifs.is_open() )
            {
                std::streampos binsize = binifs.tellg();

                char* bindata = new char [ binsize ];
                binifs.seekg ( 0, std::ios::beg );
                binifs.read ( bindata, binsize );

                int current_block_start = 0;
                int current_block_end = sizeof( int32_t );


                assert( binsize >= current_block_end );

                int32_t* total_funds_block = new int32_t;
                std::memcpy( total_funds_block, bindata, sizeof( int32_t ) );
                int tf = *total_funds_block;
                delete total_funds_block;
                total_funds_ = tf;


                current_block_start = current_block_end;
                int blocks_for_bools = floor( ( Level::NUM_O_LEVELS * 2 ) / 8 ) + 1;
                current_block_end += blocks_for_bools;

                assert( binsize >= current_block_end );

                std::vector<bool> v;
                std::vector<bool> d;

                for ( int i = 0; i < blocks_for_bools; ++i )
                {
                    unsigned char c;
                    std::memcpy( &c, &bindata[ 4 + i ], sizeof( unsigned char ) );
                    //std::cout<<(int)c<<std::endl;

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
                            d.push_back( val );
                        }
                        else
                        {
                            break;
                        }
                    }
                }

//                std::cout<<std::endl;
//                std::cout<<"VICTORIES:"<<std::endl;
//                std::cout<<std::endl;
//
//                    for ( auto vi : v )
//                    {
//                        std::cout<<vi<<",";
//                    }

//                std::cout<<std::endl;
//                std::cout<<"DIAMONDS:"<<std::endl;
//                std::cout<<std::endl;
//
//                    for ( auto di : d )
//                    {
//                        std::cout<<di<<",";
//                    }

                    victories_ = v;
                    diamonds_ = d;


                current_block_start = current_block_end;
                const int NUM_O_GEM_SCORE_BLOCKS = sizeof( int32_t ) * Level::NUM_O_LEVELS;
                current_block_end += NUM_O_GEM_SCORE_BLOCKS;

                assert( binsize >= current_block_end );

                std::vector<Counter> glist;

                for ( int gi = 0; gi < Level::NUM_O_LEVELS; ++gi )
                {
                    int32_t* gblock = new int32_t;
                    std::memcpy( gblock, &bindata[ current_block_start + ( gi * sizeof( int32_t ) ) ], sizeof( int32_t ) );
                    int gdata = *gblock;
                    delete gblock;
                    glist.push_back( { gdata, FUNDS_MAX, -1 } );
                }

//                std::cout<<std::endl;
//                std::cout<<"GEM_SCORES:"<<std::endl;
//                std::cout<<std::endl;
//
//                    for ( auto gem : glist )
//                    {
//                        std::cout<<gem()<<",";
//                    }

                gem_scores_ = glist;

                current_block_start = current_block_end;
                const int NUM_O_TIME_SCORE_BLOCKS = sizeof( int32_t ) * Level::NUM_O_LEVELS;
                current_block_end += NUM_O_TIME_SCORE_BLOCKS;

                assert( binsize >= current_block_end );

                std::vector<Counter> tlist;

                for ( int ti = 0; ti < Level::NUM_O_LEVELS; ++ti )
                {
                    int32_t* tblock = new int32_t;
                    std::memcpy( tblock, &bindata[ current_block_start + ( ti * sizeof( int32_t ) ) ], sizeof( int32_t ) );
                    int tdata = *tblock;
                    delete tblock;
                    tlist.push_back( { tdata, TIME_MAX, -1 } );
                }

//                std::cout<<std::endl;
//                std::cout<<"TIME_SCORES:"<<std::endl;
//                std::cout<<std::endl;
//
//                    for ( auto time : tlist )
//                    {
//                        std::cout<<time()<<",";
//                    }

                time_scores_ = tlist;

                delete[] bindata;

            }
            else
            {
                std::cout<<"ERROR: Could not load saves."<<std::endl;
            }

        binifs.close();
    };
