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
    const std::string path = Main::resourcePath() + "events/" + Level::getCodeName( level ) + ( ( is_secret ) ? "-secret.json" : ".json" );
	std::ifstream ifs( path );
	if( !ifs.is_open() )
	{
		throw std::runtime_error( "The o’erworld map data is missing. Please redownload this game & try ’gain." );
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
            if ( layer_string == "BG" )
            {
                layer = OWEventTile::Layer::BG;
            }
            else if ( layer_string == "FG" )
            {
                layer = OWEventTile::Layer::FG;
            }
            else
            {
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
        next_level_ = Inventory::currentLevel() + 1;
    }
};

OWEvent::MessageBack OWEvent::update( std::vector<int>& bg_tiles, std::vector<int>& fg_tiles )
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

OWEvent::MessageBack OWEvent::changeTiles( std::vector<int>& bg_tiles, std::vector<int>& fg_tiles )
{
    bool fg_change = false;
    bool bg_change = false;
    for ( const OWEventTile& tile : changes_[ current_change_ ] )
    {
        int* tilemap = nullptr;
        switch ( tile.layer_ )
        {
            case( OWEventTile::Layer::BG ):
            {
                bg_change = true;
                tilemap = &bg_tiles[ 0 ];
            }
            break;

            case( OWEventTile::Layer::FG ):
            {
                fg_change = true;
                tilemap = &fg_tiles[ 0 ];
            }
            break;
        }

        tilemap[ tile.position_ ] = tile.tile_;
    }
    ++current_change_;

    return ( bg_change && fg_change )
        ? MessageBack::BOTH_TILES_CHANGED
        : ( bg_change )
            ? MessageBack::BG_TILES_CHANGED
            : MessageBack::FG_TILES_CHANGED;
};

void OWEvent::changeAllTiles( std::vector<int>& bg_tiles, std::vector<int>& fg_tiles )
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