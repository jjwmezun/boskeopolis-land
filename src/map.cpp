#include <cmath>
#include "game.hpp"
#include "json.hpp"
#include "map.hpp"
#include "map_layer_palette_change.hpp"
#include "map_layer_rain.hpp"
#include "nasringine/nasr.h"
#include "sprite.hpp"
#include <unordered_map>

#include "tileset.hpp"

#include <iostream>

namespace BSL
{
    struct MapTileLayer
    {
        enum class Type
        {
            COLLISION,
            TILE,
            OBJECT,
            SPRITE
        };

        std::vector<int> tiles;
        MapTileLayer::Type type;
    };

    Map::Map( std::string && slug )
    :
        slug_ ( slug ),
        width_ ( 0 ),
        height_ ( 0 ),
        i_ ( 0 ),
        remove_block_ ( false )
    {};

    void Map::init( Game & game )
    {
        const Tileset & objects = game.getObjects();

        // Backgrounds.
        game.render().addRectGradient
        (
            0.0f,
            0.0f,
            BSL::WINDOW_WIDTH_PIXELS,
            BSL::WINDOW_HEIGHT_PIXELS,
            Dir::XY::DOWN,
            1,
            254,
            true
        );
        layers_.emplace_back( std::make_unique<MapLayerRain>( 32, 128 ) );
        layers_.emplace_back( std::make_unique<MapLayerPaletteChange>( 32, 128 ) );
        for ( auto & layer : layers_ )
        {
            layer->init( game );
        }

        // Get tile data from JSON file.
        JSON json { "assets/maps/" + slug_ + ".json" };
        width_ = json.getInt( "width" );
        height_ = json.getInt( "height" );
        JSONArray l = json.getArray( "layers" );
        std::vector<MapTileLayer> layers = JSONMap<MapTileLayer>
        (
            l,
            [&]( const JSONItem & i )
            {
                JSONObject o = i.asObject();

                JSONArray tiles = o.getArray( "data" );

                std::string name = o.getString( "name" );
                std::unordered_map<std::string, MapTileLayer::Type> t
                {
                    { "collision", MapTileLayer::Type::COLLISION },
                    { "tile", MapTileLayer::Type::TILE },
                    { "object", MapTileLayer::Type::OBJECT },
                    { "sprite", MapTileLayer::Type::SPRITE }
                };
                auto it = t.find( name );
                if ( it == t.end() )
                {
                    throw std::runtime_error( "Map " + slug_ + " has invalid layer type " + name );
                }

                return MapTileLayer
                {
                    JSONMap<int>
                    (
                        tiles,
                        []( const JSONItem & di )
                        {
                            return di.asInt();
                        }
                    ),
                    it->second
                };
            }   
        );

        // Generate map data from tile data.
        for ( unsigned int i = 0; i < width_ * height_; ++i )
        {
            blocks_.push_back({});
            sprites_.push_back({});
        }

        for ( const MapTileLayer & layer : layers )
        {
            switch ( layer.type )
            {
                case ( MapTileLayer::Type::COLLISION ):
                {
                    collision_.push_back( layer.tiles );
                }
                break;
                case ( MapTileLayer::Type::TILE ):
                {
                    /*
                    std::vector<NasrTile> tiles;
                    for ( int tile : layer.tiles )
                    {
                        NasrTile t;
                        if ( tile == 0 )
                        {
                            t = { 0, 0, 0, 255 };
                        }
                        else
                        {
                            const int i = tile - 7073;
                            t =
                            {
                                static_cast<unsigned char>( i % 16 ),
                                static_cast<unsigned char>( std::floor( i / 16 ) ),
                                0,
                                0
                            };
                        }

                        tiles.push_back( t );
                    }

                    game.render().addTilemap
                    (
                        "urban",
                        tiles,
                        width_,
                        height_
                    );*/

                    int texture = NasrAddTextureBlank( getWidthPixels(), getHeightPixels() );
                    int menu_texture = NasrLoadFileAsTexture( "assets/graphics/tilesets/urban.png" );
                    if ( menu_texture < 0 )
                    {
                        // TODO: Throw exception.
                        std::cout << "NO MENU" << std::endl;
                    }
                    NasrRect src = { 0.0f, 0.0f, 16.0f, 16.0f };
                    NasrRect dest = src;
                    NasrSetTextureAsTarget( texture );
                    
                    for ( int i = 0; i < layer.tiles.size(); ++i )
                    {
                        const int tile = layer.tiles[ i ];
                        if ( tile > 0 )
                        {
                            const int ti = tile - 7073;
                            src.x = static_cast<float> ( ti % 16 ) * 16.0f;
                            src.y = std::floor( static_cast<float> ( ti ) / 16.0f ) * 16.0f;
                            dest.x = static_cast<float> ( i % width_ ) * 16.0f;
                            dest.y = std::floor( i / width_ ) * 16.0f;
                            NasrDrawSpriteToTexture
                            (
                                menu_texture,
                                src,
                                dest,
                                0,
                                0,
                                0.0f,
                                0.0f,
                                0.0f,
                                1.0f,
                                1,
                                1
                            );
                        }
                    }
                    NasrReleaseTextureTarget();

                    game.render().addTextureSprite
                    (
                        texture,
                        0.0f,
                        0.0f,
                        static_cast<float> ( getWidthPixels() ),
                        static_cast<float> ( getHeightPixels() ),
                        0.0f,
                        0.0f
                    );
                }
                break;
                case ( MapTileLayer::Type::OBJECT ):
                {
                    std::vector<NasrTile> tiles;
                    int i = 0;
                    for ( unsigned int i = 0; i < layer.tiles.size(); ++i )
                    {
                        tiles.push_back({ 0, 0, 0, 255 });
                        const int tile = layer.tiles[ i ];
                        if ( tile == 0 )
                        {
                            continue;
                        }

                        const int objtype = tile - 1121;
                        if ( objtype < 0 )
                        {
                            throw std::runtime_error( "Invalid object #" + tile );
                        }

                        const BlockType & type = objects.getBlockType( objtype );
                        tiles[ i ] =
                        {
                            type.getX(),
                            type.getY(),
                            0,
                            type.getAnimation()
                        };
                        blocks_[ i ].emplace_back( block_layers_.size(), type );
                    }
                    block_layers_.push_back
                    (
                        game.render().addTilemap
                        (
                            "objects",
                            tiles,
                            width_,
                            height_
                        )
                    );
                }
                break;
                case ( MapTileLayer::Type::SPRITE ):
                {
                    int i = 0;
                    for ( unsigned int i = 0; i < layer.tiles.size(); ++i )
                    {
                        const int tile = layer.tiles[ i ];
                        if ( tile == 0 )
                        {
                            continue;
                        }
                        const int spritetype = tile - 4096;
                        if ( spritetype < 0 )
                        {
                            throw std::runtime_error( "Invalid sprite #" + tile );
                        }
                        sprites_[ i ].emplace_back( spritetype, false );
                    }
                }
                break;
            }
        }
    };

    void Map::update( Level & level, const Game & game, float dt )
    {
        for ( auto & layer : layers_ )
        {
            layer->update( level, game, dt );
        }
    };

    void Map::interact( Sprite & sprite, Level & level, Game & game )
    {
        const int y1 = pixelsToBlocks( sprite.getPos().y );
        const int y2 = pixelsToBlocks( sprite.getPos().bottom() );
        const int x1 = pixelsToBlocks( sprite.getPos().x );
        const int x2 = pixelsToBlocks( sprite.getPos().right() );

        if
        (
            y1 < 0 ||
            x1 < 0 ||
            y2 >= height_ ||
            x2 >= width_
        )
        {
            return;
        }

        for ( unsigned int y = y1; y <= y2; ++y )
        {
            for ( unsigned int x = x1; x <= x2; ++x )
            {
                i_ = 0;
                const unsigned int n = getIFromXAndY( x, y );
                while ( i_ < blocks_[ n ].size() )
                {
                    Block & block = blocks_[ n ][ i_ ];
                    block.interact( sprite, level, game, *this );
                    if ( remove_block_ )
                    {
                        block_layers_[ block.getLayer() ].clearTile( x, y );
                        blocks_[ n ].erase( blocks_[ n ].begin() + i_ );
                        remove_block_ = false;
                    }
                    else
                    {
                        ++i_;
                    }
                }
            }
        }
    };

    bool Map::testCollision( unsigned int x, unsigned int y, std::vector<CollisionType> types ) const
    {
        if ( x > width_ || y > height_ ) return false;

        const int i = getIFromXAndY( x, y );
        for ( const std::vector<int> & layer : collision_ )
        {
            for ( const CollisionType type : types )
            {
                if ( layer[ i ] == static_cast<int>( type ) )
                {
                    return true;
                }
            }
        }

        return false;
    };
}