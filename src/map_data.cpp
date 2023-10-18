#include "config.hpp"
#include "game.hpp"
#include "json.hpp"
#include "map_data.hpp"
#include "utility.hpp"

namespace BSL
{
    MapData::MapData( const std::string & slug )
    :
        slug_ ( slug ),
        width_ ( 0 ),
        height_ ( 0 ),
        loaded_ ( false )
    {};

    void MapData::init( const Game & game )
    {
        // Skip if already loaded.
        if ( loaded_ )
        {
            return;
        }

        // Get tile data from JSON file.
        JSON json { "assets/maps/" + slug_ + ".json" };
        width_ = json.getInt( "width" );
        height_ = json.getInt( "height" );
        JSONArray l = json.getArray( "layers" );

        // Get map tile layers from JSON file.
        unsigned int ii = 0;
        layers_ = JSONMap<MapTileLayer>
        (
            l,
            [&, &ii]( const JSONItem & i )
            {
                MapTileLayer layer;
                layer.type = MapTileLayer::Type::__NULL;
                layer.scrollx = 0.0f;
                layer.scrolly = 0.0f;
                layer.offsetx = 0.0f;
                layer.offsety = 0.0f;
                JSONObject o = i.asObject();

                std::string type = o.getString( "type" );

                if ( type == "imagelayer" )
                {
                    layer.type = MapTileLayer::Type::IMAGE;
                    layer.misc.image.texture = nullptr;
                    layer.misc.image.tilex = false;
                    layer.misc.image.tiley = false;

                    std::string image = strReplace( o.getString( "image" ), "..\/graphics\/", "" );
                    layer.misc.image.texture = static_cast<char *>( calloc( strlen( image.c_str() ) + 1, sizeof( char ) ) );
                    strcpy( layer.misc.image.texture, image.c_str() );

                    if ( o.hasBool( "repeatx" ) )
                    {
                        layer.misc.image.tilex = o.getBool( "repeatx" );
                    }

                    if ( o.hasBool( "repeaty" ) )
                    {
                        layer.misc.image.tiley = o.getBool( "repeaty" );
                    }

                    if ( o.hasFloat( "parallaxx" ) )
                    {
                        layer.scrollx = o.getFloat( "parallaxx" );
                    }
                    else if ( o.hasInt( "parallaxx" ) )
                    {
                        layer.scrollx = static_cast<float> ( o.getInt( "parallaxx" ) );
                    }

                    if ( o.hasFloat( "parallaxy" ) )
                    {
                        layer.scrolly = o.getFloat( "parallaxy" );
                    }
                    else if ( o.hasInt( "parallaxy" ) )
                    {
                        layer.scrolly = static_cast<float> ( o.getInt( "parallaxy" ) );
                    }

                    if ( o.hasFloat( "offsetx" ) )
                    {
                        layer.offsetx = o.getFloat( "offsetx" );
                    }
                    else if ( o.hasInt( "offsetx" ) )
                    {
                        layer.offsetx = static_cast<float> ( o.getInt( "offsetx" ) );
                    }

                    if ( o.hasFloat( "offsety" ) )
                    {
                        layer.offsety = o.getFloat( "offsety" );
                    }
                    else if ( o.hasInt( "offsety" ) )
                    {
                        layer.offsety = static_cast<float> ( o.getInt( "offsety" ) );
                    }
                }
                else if ( type == "objectgroup" )
                {
                    std::string name = o.getString( "name" );
                    layer.type = BSL::findInMap<MapTileLayer::Type>
                    (
                        {
                            { "warp", MapTileLayer::Type::WARP },
                            { "gradient", MapTileLayer::Type::GRADIENT },
                            { "rain", MapTileLayer::Type::RAIN },
                            { "constellation", MapTileLayer::Type::CONSTELLATION },
                            { "palchange", MapTileLayer::Type::PALCHANGE }
                        },
                        name,
                        "Map " + slug_ + " has invalid layer type " + name
                    );

                    switch ( layer.type )
                    {
                        case ( MapTileLayer::Type::WARP ):
                        {
                            if ( !o.hasArray( "objects" ) )
                            {
                                break;
                            }
                            const JSONArray objs = o.getArray( "objects" );
                            objs.forEach
                            (
                                [ & ]( const JSONItem & oi )
                                {
                                    Warp w;
                                    const auto oo = oi.asObject();
                                    if ( oo.hasFloat( "x" ) )
                                    {
                                        w.coords.x = oo.getFloat( "x" );
                                    }
                                    else if ( oo.hasInt( "x" ) )
                                    {
                                        w.coords.x = static_cast<float> ( oo.getInt( "x" ) );
                                    }
                                    if ( oo.hasFloat( "y" ) )
                                    {
                                        w.coords.y = oo.getFloat( "y" );
                                    }
                                    else if ( oo.hasInt( "y" ) )
                                    {
                                        w.coords.y = static_cast<float> ( oo.getInt( "y" ) );
                                    }
                                    if ( oo.hasFloat( "width" ) )
                                    {
                                        w.coords.w = oo.getFloat( "width" );
                                    }
                                    else if ( oo.hasInt( "width" ) )
                                    {
                                        w.coords.w = static_cast<float> ( oo.getInt( "width" ) );
                                    }
                                    if ( oo.hasFloat( "height" ) )
                                    {
                                        w.coords.h = oo.getFloat( "height" );
                                    }
                                    else if ( oo.hasInt( "height" ) )
                                    {
                                        w.coords.h = static_cast<float> ( oo.getInt( "height" ) );
                                    }
                                    if ( oo.hasArray( "properties" ) )
                                    {
                                        const JSONArray props = oo.getArray( "properties" );
                                        props.forEach
                                        (
                                            [ &w ]( const JSONItem & pi )
                                            {
                                                const auto po = pi.asObject();
                                                if ( !po.hasString( "name" ) )
                                                {
                                                    return;
                                                }
                                                const std::string name = po.getString( "name" );
                                                if ( name == "map" )
                                                {
                                                    if ( po.hasInt( "value" ) )
                                                    {
                                                        w.map = static_cast<unsigned int> ( po.getInt( "value" ) );
                                                    }
                                                    else if ( po.hasFloat( "value" ) )
                                                    {
                                                        w.map = static_cast<unsigned int> ( po.getFloat( "value" ) );
                                                    }
                                                }
                                                else if ( name == "entrance_x" )
                                                {
                                                    if ( po.hasFloat( "value" ) )
                                                    {
                                                        w.entrance_x = static_cast<float>( blocksToPixels( po.getFloat( "value" ) ) );
                                                    }
                                                    else if ( po.hasInt( "value" ) )
                                                    {
                                                        w.entrance_x = static_cast<float>( blocksToPixels( po.getInt( "value" ) ) );
                                                    }
                                                }
                                                else if ( name == "entrance_y" )
                                                {
                                                    if ( po.hasFloat( "value" ) )
                                                    {
                                                        w.entrance_y = static_cast<float>( blocksToPixels( po.getFloat( "value" ) ) );
                                                    }
                                                    else if ( po.hasInt( "value" ) )
                                                    {
                                                        w.entrance_y = static_cast<float>( blocksToPixels( po.getInt( "value" ) ) );
                                                    }
                                                }
                                                else if ( name == "camera_x" )
                                                {
                                                    if ( po.hasFloat( "value" ) )
                                                    {
                                                        w.camera_x = po.getFloat( "value" );
                                                    }
                                                    else if ( po.hasInt( "value" ) )
                                                    {
                                                        w.camera_x = static_cast<float> ( po.getInt( "value" ) );
                                                    }
                                                }
                                                else if ( name == "camera_y" )
                                                {
                                                    if ( po.hasFloat( "value" ) )
                                                    {
                                                        w.camera_y = po.getFloat( "value" );
                                                    }
                                                    else if ( po.hasInt( "value" ) )
                                                    {
                                                        w.camera_y = static_cast<float> ( po.getInt( "value" ) );
                                                    }
                                                }
                                            }
                                        );
                                    }
                                    warps_.push_back( w );
                                }
                            );
                        }
                        break;
                        case ( MapTileLayer::Type::GRADIENT ):
                        {
                            layer.misc.gradient.dir = Dir::XY::UP;
                            layer.misc.gradient.start = 0;
                            layer.misc.gradient.end = 0;

                            if ( o.hasArray( "properties" ) )
                            {
                                const JSONArray props = o.getArray( "properties" );
                                props.forEach
                                (
                                    [ &layer ]( const JSONItem & di )
                                    {
                                        const JSONObject io = di.asObject();
                                        const std::string name = io.getString( "name" );
                                        if ( name == "direction" )
                                        {
                                            std::string dir = io.getString( "value" );
                                            layer.misc.gradient.dir = Dir::getXYFromString( dir );
                                        }
                                        else if ( name == "start" )
                                        {
                                            if ( io.hasFloat( "value" ) )
                                            {
                                                layer.misc.gradient.start = static_cast<unsigned int> ( io.getFloat( "value" ) );
                                            }
                                            else
                                            {
                                                layer.misc.gradient.start = static_cast<unsigned int> ( io.getInt( "value" ) );
                                            }
                                        }
                                        else if ( name == "end" )
                                        {
                                            if ( io.hasFloat( "value" ) )
                                            {
                                                layer.misc.gradient.end = static_cast<unsigned int> ( io.getFloat( "value" ) );
                                            }
                                            else
                                            {
                                                layer.misc.gradient.end = static_cast<unsigned int> ( io.getInt( "value" ) );
                                            }
                                        }
                                    }
                                );
                            }

                        }
                        break;
                        case ( MapTileLayer::Type::RAIN ):
                        {
                            layer.misc.rain.start = 0;
                            layer.misc.rain.end = 0;

                            if ( o.hasArray( "properties" ) )
                            {
                                const JSONArray props = o.getArray( "properties" );
                                props.forEach
                                (
                                    [ &layer ]( const JSONItem & di )
                                    {
                                        const JSONObject io = di.asObject();
                                        const std::string name = io.getString( "name" );
                                        if ( name == "start" )
                                        {
                                            if ( io.hasFloat( "value" ) )
                                            {
                                                layer.misc.rain.start = static_cast<unsigned int> ( io.getFloat( "value" ) );
                                            }
                                            else
                                            {
                                                layer.misc.rain.start = static_cast<unsigned int> ( io.getInt( "value" ) );
                                            }
                                        }
                                        else if ( name == "end" )
                                        {
                                            if ( io.hasFloat( "value" ) )
                                            {
                                                layer.misc.rain.end = static_cast<unsigned int> ( io.getFloat( "value" ) );
                                            }
                                            else
                                            {
                                                layer.misc.rain.end = static_cast<unsigned int> ( io.getInt( "value" ) );
                                            }
                                        }
                                    }
                                );
                            }

                        }
                        break;
                        case ( MapTileLayer::Type::CONSTELLATION ):
                        {
                            layer.scrollx = 0.0f;
                            layer.scrolly = 0.0f;

                            if ( o.hasArray( "properties" ) )
                            {
                                const JSONArray props = o.getArray( "properties" );
                                props.forEach
                                (
                                    [ &layer ]( const JSONItem & di )
                                    {
                                        const JSONObject io = di.asObject();
                                        const std::string name = io.getString( "name" );
                                        if ( name == "scrollx" )
                                        {
                                            if ( io.hasInt( "value" ) )
                                            {
                                                layer.scrollx = static_cast<float> ( io.getInt( "value" ) );
                                            }
                                            else
                                            {
                                                layer.scrollx = io.getFloat( "value" );
                                            }
                                        }
                                        else if ( name == "scrolly" )
                                        {
                                            if ( io.hasInt( "value" ) )
                                            {
                                                layer.scrolly = static_cast<float> ( io.getInt( "value" ) );
                                            }
                                            else
                                            {
                                                layer.scrolly = io.getFloat( "value" );
                                            }
                                        }
                                    }
                                );
                            }

                        }
                        break;
                        case ( MapTileLayer::Type::PALCHANGE ):
                        {
                            layer.misc.palchange.start = 0;
                            layer.misc.palchange.end = 0;

                            if ( o.hasArray( "properties" ) )
                            {
                                const JSONArray props = o.getArray( "properties" );
                                props.forEach
                                (
                                    [ &layer ]( const JSONItem & di )
                                    {
                                        const JSONObject io = di.asObject();
                                        const std::string name = io.getString( "name" );
                                        if ( name == "start" )
                                        {
                                            if ( io.hasFloat( "value" ) )
                                            {
                                                layer.misc.palchange.start = static_cast<unsigned int> ( io.getFloat( "value" ) );
                                            }
                                            else
                                            {
                                                layer.misc.palchange.start = static_cast<unsigned int> ( io.getInt( "value" ) );
                                            }
                                        }
                                        else if ( name == "end" )
                                        {
                                            if ( io.hasFloat( "value" ) )
                                            {
                                                layer.misc.palchange.end = static_cast<unsigned int> ( io.getFloat( "value" ) );
                                            }
                                            else
                                            {
                                                layer.misc.palchange.end = static_cast<unsigned int> ( io.getInt( "value" ) );
                                            }
                                        }
                                    }
                                );
                            }

                        }
                        break;
                    }
                }
                else if ( type == "tilelayer" )
                {
                    JSONArray tiles = o.getArray( "data" );

                    std::string name = o.getString( "name" );
                    const MapTileLayer::Type type = BSL::findInMap<MapTileLayer::Type>
                    (
                        {
                            { "collision", MapTileLayer::Type::COLLISION },
                            { "tile", MapTileLayer::Type::TILE },
                            { "object", MapTileLayer::Type::OBJECT },
                            { "sprite", MapTileLayer::Type::SPRITE }
                        },
                        name,
                        "Map " + slug_ + " has invalid layer type " + name
                    );

                    float scrollx = 0.0f;
                    float scrolly = 0.0f;
                    unsigned int tilex = 0;
                    Layer lnum = Layer::BLOCKS_1;

                    if ( o.hasArray( "properties" ) )
                    {
                        const JSONArray props = o.getArray( "properties" );
                        props.forEach
                        (
                            [ &scrollx, &scrolly, &tilex, &lnum ]( const JSONItem & di )
                            {
                                const JSONObject io = di.asObject();
                                const std::string name = io.getString( "name" );
                                if ( name == "scrollx" )
                                {
                                    if ( io.hasInt( "value" ) )
                                    {
                                        scrollx = static_cast<float> ( io.getInt( "value" ) );
                                    }
                                    else
                                    {
                                        scrollx = io.getFloat( "value" );
                                    }
                                }
                                else if ( name == "scrolly" )
                                {
                                    if ( io.hasInt( "value" ) )
                                    {
                                        scrolly = static_cast<float> ( io.getInt( "value" ) );
                                    }
                                    else
                                    {
                                        scrolly = io.getFloat( "value" );
                                    }
                                }
                                else if ( name == "tilex" )
                                {
                                    tilex = io.getInt( "value" );
                                }
                                else if ( name == "layer" )
                                {
                                    if ( io.hasString( "value" ) )
                                    {
                                        const std::string lval = io.getString( "value" );
                                        lnum = getLayerFromString( lval ).value_or( lnum );
                                    }
                                    else if ( io.hasInt( "value" ) )
                                    {
                                        lnum = static_cast<Layer> ( io.getInt( "value" ) );
                                    }
                                }
                            }
                        );
                    }

                    layer =
                    {
                        JSONMap<int>
                        (
                            tiles,
                            []( const JSONItem & di )
                            {
                                return di.asInt();
                            }
                        ),
                        type,
                        scrollx,
                        scrolly,
                        0.0f,
                        0.0f,
                        tilex,
                        {},
                        lnum
                    };

                    if ( type == MapTileLayer::Type::OBJECT )
                    {
                        object_layers_.push_back( ii );
                    }
                }

                ++ii;
                return layer;
            }
        );

        loaded_ = true;
    };

    const std::string & MapData::slug() const
    {
        return slug_;
    };

    const std::vector<MapTileLayer> MapData::layers() const
    {
        return layers_;
    };

    unsigned int MapData::width() const
    {
        return width_;
    };

    unsigned int MapData::height() const
    {
        return height_;
    };

    std::optional<Warp> MapData::getWarp( const Rect & pos ) const
    {
        for ( auto & warp : warps_ )
        {
            if ( warp.coords.testCollision( pos ) )
            {
                return warp;
            }
        }

        return {};
    };

    void MapData::clear()
    {
        for ( auto & layer : layers_ )
        {
            if ( layer.type == MapTileLayer::Type::IMAGE )
            {
                free( layer.misc.image.texture );
            }
        }
    };

    void MapData::removeObject( unsigned int layer, unsigned int n )
    {
        layers_[ object_layers_[ layer ] ].tiles[ n ] = 0;
    };
}