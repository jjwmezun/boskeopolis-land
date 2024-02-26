#include "json.hpp"
#include "level_table.hpp"
#include <regex>
#include <stdexcept>

namespace BSL
{
    std::vector<LevelData> level_table;

    void loadLevelDataTable()
    {
        const JSON json { "assets/levels/list.json" };
        const JSONArray list = json.getArray( "list" );
        list.forEach
        (
            [&] ( const JSONItem item, unsigned int i )
            {
                LevelData level;
                const JSONObject obj = item.asObject();
                const uint_fast8_t cycle = static_cast<uint_fast8_t> ( obj.getInt( "cycle" ) );
                const uint_fast8_t theme = static_cast<uint_fast8_t> ( obj.getInt( "theme" ) );

                if ( cycle > 9 || cycle < 1 )
                {
                    throw std::runtime_error( "Error loading level list. Cycles should ne’er be greater than 9 or less than 1." );
                }

                if ( theme > 16 || theme < 1 )
                {
                    throw std::runtime_error( "Error loading level list. Themes should ne’er be greater than 16 or less than 1." );
                }

                static constexpr char hextable[ 16 ] =
                {
                    '0',
                    '1',
                    '2',
                    '3',
                    '4',
                    '5',
                    '6',
                    '7',
                    '8',
                    '9',
                    'A',
                    'B',
                    'C',
                    'D',
                    'E',
                    'F'
                };
                const char theme_str[ 1 ] = { hextable[ theme - 1 ] };

                // Follow pattern: B-2: [name] for 12th theme o’ 2nd cycle.
                level.name = std::string( theme_str ) + "-" + std::to_string( cycle ) + ": " + obj.getString( "name" );

                // Load gem score.
                level.gemscore = static_cast<uint_fast16_t> ( obj.getInt( "gemscore" ) );

                // Load time score.
                level.timescore = static_cast<uint_fast16_t> ( obj.getInt( "timescore" ) );

                // Load entrance x & y
                if ( obj.hasInt( "entrancex" ) )
                {
                    level.entrancex = static_cast<uint_fast16_t> ( obj.getInt( "entrancex" ) );
                }
                if ( obj.hasInt( "entrancey" ) )
                {
                    level.entrancey = static_cast<uint_fast16_t> ( obj.getInt( "entrancey" ) );
                }

                // Load camera x & y
                if ( obj.hasInt( "camerax" ) )
                {
                    level.camerax = static_cast<uint_fast16_t> ( obj.getInt( "camerax" ) );
                }
                if ( obj.hasInt( "cameray" ) )
                {
                    level.cameray = static_cast<uint_fast16_t> ( obj.getInt( "cameray" ) );
                }

                // Load maps.
                JSONArray maplist = obj.getArray( "maps" );
                level.mapcount = maplist.getLength();
                maplist.forEach
                (
                    [&] ( JSONItem mapitem )
                    {
                        level.maps.push_back( mapitem.asString() );
                    }
                );

                // Load goal.
                if ( obj.hasObject( "goal" ) )
                {
                    const auto goalobj = obj.getObject( "goal" );

                    // Get type.
                    const std::string typestring = goalobj.getStringOptional( "type", "keycane" );
                    if ( typestring == "keycane" )
                    {
                        level.goal.type = GoalType::KEYCANE;
                    }
                    else if ( typestring == "collect" )
                    {
                        level.goal.type = GoalType::COLLECT;
                    }
                    else if ( typestring == "timed" )
                    {
                        level.goal.type = GoalType::TIMED;
                    }

                    // Load extra data.
                    switch ( level.goal.type )
                    {
                        case ( GoalType::COLLECT ):
                        {
                            level.goal.data.collect.amount = goalobj.getInt( "amount" );
                        }
                        break;
                    }

                    // Load message.
                    std::string message;
                    if ( goalobj.hasString( "message" ) )
                    {
                        message = goalobj.getString( "message" );
                    }
                    else
                    {
                        switch ( level.goal.type )
                        {
                            case ( GoalType::COLLECT ):
                            {
                                message = std::regex_replace( "¡Collect %s ₧!", std::regex( "%s" ), std::to_string( level.goal.data.collect.amount ) );
                            }
                            break;
                        }
                    }
                    level.goal.message = static_cast<char *>( calloc( message.size(), 1 ) );
                    strncpy( level.goal.message, message.c_str(), message.size() );
                }

                level_table.push_back( level );
            }
        );
    };
};