#include "audio.hpp"
#include <fstream>
#include "inventory.hpp"
#include "level.hpp"
#include "main.hpp"
#include "ow_event.hpp"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/rapidjson.h"

static constexpr int DEFAULT_TILE_SPEED = 16;

OWEvent::OWEvent()
:
    event_tile_speed_ ( DEFAULT_TILE_SPEED ),
    timer_ ( 0 ),
    current_change_ ( 0 ),
    next_level_ ( 0 ),
    changes_ (),
    target_position_ ()
{};

void OWEvent::init( int level, int map_width, bool is_secret )
{
    std::string path;
    if ( level == -2 )
    {
        path = Main::resourcePath() + "events/shop-1.json";
    }
    else
    {
        path = Main::resourcePath() + "events/" + Level::getCodeName( level ) + ( ( is_secret ) ? "-secret.json" : ".json" );
    }
	std::ifstream ifs( path );
	if( !ifs.is_open() )
	{
		throw std::runtime_error( "The o’erworld map data is missing for file “" + path + "”. Please redownload this game & try ’gain." );
	}
	rapidjson::IStreamWrapper ifs_wrap( ifs );
    rapidjson::Document document;
    document.ParseStream( ifs_wrap );
	ifs.close();

    if ( !document.IsObject() )
    {
		throw std::runtime_error( "O’erworld event for level " + Level::getCodeName( level ) + " is missing. Please redownload game." );
    }

    const auto& data = document.GetObject();

    if
    (
        !data.HasMember( "camera_target_x" )
        || !data[ "camera_target_x" ].IsInt()
        || !data.HasMember( "camera_target_y" )
        || !data[ "camera_target_y" ].IsInt()
        || !data.HasMember( "changes" )
        || !data[ "changes" ].IsArray()
    )
    {
		throw std::runtime_error( "O’erworld event for level " + Level::getCodeName( level ) + " has been corrupted. Please redownload game." );
    }

    target_position_.x = ( double )( Unit::BlocksToPixels( data[ "camera_target_x" ].GetInt() ) );
    target_position_.y = ( double )( Unit::BlocksToPixels( data[ "camera_target_y" ].GetInt() ) );

    for ( const auto& change_data : data[ "changes" ].GetArray() )
    {
        if ( !change_data.IsArray() )
        {
		    throw std::runtime_error( "O’erworld event for level " + Level::getCodeName( level ) + " has been corrupted. Please redownload game." );
        }

        std::vector<OWEventTile> change_list;

        for ( const auto& tile_o : change_data.GetArray() )
        {
            if ( !tile_o.IsObject() )
            {
		        throw std::runtime_error( "O’erworld event for level " + Level::getCodeName( level ) + " has been corrupted. Please redownload game." );
            }

            const auto& tile = tile_o.GetObject();

            if
            (
                !tile.HasMember( "layer" )
                || !tile[ "layer" ].IsString()
                || !tile.HasMember( "x" )
                || !tile[ "x" ].IsInt()
                || !tile.HasMember( "y" )
                || !tile[ "y" ].IsInt()
                || !tile.HasMember( "tile" )
                || !tile[ "tile" ].IsInt()
            )
            {
		        throw std::runtime_error( "O’erworld event for level " + Level::getCodeName( level ) + " has been corrupted. Please redownload game." );
            }

            OWEventTile::Layer layer;
            const std::string layer_string = tile[ "layer" ].GetString();
            if ( layer_string == "BG1" )
            {
                layer = OWEventTile::Layer::BG1;
            }
            else if ( layer_string == "BG2" )
            {
                layer = OWEventTile::Layer::BG2;
            }
            else if ( layer_string == "FG1" )
            {
                layer = OWEventTile::Layer::FG1;
            }
            else if ( layer_string == "FG2" )
            {
                layer = OWEventTile::Layer::FG2;
            }
            else
            {
                printf( "%s\n", layer_string.c_str() );
		        throw std::runtime_error( "O’erworld event for level " + Level::getCodeName( level ) + " has been corrupted. Please redownload game." );
            }

            const int position = tile[ "y" ].GetInt() * map_width + tile[ "x" ].GetInt();
            const int tile_index = tile[ "tile" ].GetInt() + 1;
            change_list.emplace_back( layer, position, tile_index );
        }
        changes_.push_back( change_list );
    }

    if ( data.HasMember( "speed" ) && data[ "speed" ].IsInt() )
    {
        event_tile_speed_ = data[ "speed" ].GetInt();
    }

    if ( data.HasMember( "next_level" ) && data[ "next_level" ].IsString() )
    {
        const std::string key = data[ "next_level" ].GetString();
        next_level_ = Level::getIDFromCodeName( key );
    }
    else
    {
        next_level_ = level + 1;
    }
};

OWEvent::MessageBack OWEvent::update( std::vector<int>* bg_tiles, std::vector<int>* fg_tiles )
{
    MessageBack message = MessageBack::__NULL;
    if ( current_change_ == changes_.size() )
    {
        if ( timer_ == 64 )
        {
            return MessageBack::EVENT_OVER;
        }
        else if ( timer_ == 32 )
        {
            message = MessageBack::SHOW_NEXT_LEVEL;
        }
        ++timer_;
    }
    else
    {
        if ( timer_ == event_tile_speed_ )
        {
            Audio::playSound( Audio::SoundType::CHEST_OPEN );
            message = changeTiles( bg_tiles, fg_tiles );
            timer_ = 0;
        }
        else
        {
            ++timer_;
        }
    }
    return message;
};

const DPoint& OWEvent::getTargetPosition() const
{
    return target_position_;
};

OWEvent::MessageBack OWEvent::changeTiles( std::vector<int>* bg_tiles, std::vector<int>* fg_tiles )
{
    MessageBack message = MessageBack::__NULL;
    for ( const OWEventTile& tile : changes_[ current_change_ ] )
    {
        int* tilemap = nullptr;
        switch ( tile.layer_ )
        {
            case( OWEventTile::Layer::BG1 ):
            {
                message = message | MessageBack::BG_TILES_1_CHANGED;
                tilemap = &bg_tiles[ 0 ][ 0 ];
            }
            break;

            case( OWEventTile::Layer::BG2 ):
            {
                message = message | MessageBack::BG_TILES_2_CHANGED;
                tilemap = &bg_tiles[ 1 ][ 0 ];
            }
            break;

            case( OWEventTile::Layer::FG1 ):
            {
                message = message | MessageBack::FG_TILES_1_CHANGED;
                tilemap = &fg_tiles[ 0 ][ 0 ];
            }
            break;

            case( OWEventTile::Layer::FG2 ):
            {
                message = message | MessageBack::FG_TILES_2_CHANGED;
                tilemap = &fg_tiles[ 1 ][ 0 ];
            }
            break;
        }

        tilemap[ tile.position_ ] = tile.tile_;
    }
    ++current_change_;

    return message;
};

void OWEvent::changeAllTiles( std::vector<int>* bg_tiles, std::vector<int>* fg_tiles )
{
    while ( current_change_ < changes_.size() )
    {
        changeTiles( bg_tiles, fg_tiles );
    }
};

int OWEvent::getNextLevel() const
{
    return next_level_;
};