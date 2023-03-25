#include "arg.hpp"
#include <cmath>
#include "json.hpp"
#include <stdexcept>
#include "tileset.hpp"

namespace BSL
{
    void Tileset::init()
    {
        const JSON json { "assets/tilesets/" + name_ + ".json" };
        const int width = json.getInt( "tilewidth" );
        const JSONArray tiles = json.getArray( "tiles" );
        tiles.forEach( [&]( JSONItem item )
        {
            const JSONObject o = item.asObject();
            int id = o.getInt( "id" );
            unsigned char animation = 0;
            unsigned char x = id % width;
            unsigned char y = std::floor( id / width );
            std::string blocktype = "none";
            ArgList args {};
            const JSONArray props = o.getArray( "properties" );
            props.forEach( [&]( JSONItem item )
            {
                const JSONObject o = item.asObject();
                const std::string name = o.getString( "name" );
                const std::string type = o.getString( "type" );
                if ( name == "animation" )
                {
                    if ( type != "int" )
                    {
                        throw std::runtime_error( "Tileset " + name_ + " has an invalid format: a tile’s animation isn’t an integer." );
                    }
                    animation = ( unsigned char )( o.getInt( "value" ) );
                }
                else if ( name == "x" )
                {
                    if ( type != "int" )
                    {
                        throw std::runtime_error( "Tileset " + name_ + " has an invalid format: a x’s animation isn’t an integer." );
                    }
                    x = ( unsigned char )( o.getInt( "value" ) );
                }
                else if ( name == "y" )
                {
                    if ( type != "int" )
                    {
                        throw std::runtime_error( "Tileset " + name_ + " has an invalid format: a y’s animation isn’t an integer." );
                    }
                    y = ( unsigned char )( o.getInt( "value" ) );
                }
                else if ( name == "type" )
                {
                    if ( type != "string" )
                    {
                        throw std::runtime_error( "Tileset " + name_ + " has an invalid format: a type’s animation isn’t a string." );
                    }
                    blocktype = o.getString( "value" );
                }
                else
                {
                    if ( type == "int" )
                    {
                        args.insert({ name, o.getInt( "value" ) });
                    }
                    else if ( type == "string" )
                    {
                        args.insert({ name, o.getString( "value" ) });
                    }
                }
            });
            while ( data_.size() <= static_cast<size_t>( id ) )
            {
                data_.push_back({});
            }
            data_[ id ] = { x, y, animation };
        });
    }

    const BlockType * Tileset::getBlockType( unsigned int type ) const
    {
        return data_.at( type ).isNull() ? nullptr : &data_.at( type );
    };
}