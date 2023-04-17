#include "map.hpp"
#include "scale_lift_sprite.hpp"
#include "sprite_system.hpp"

namespace BSL
{
    SpriteSystem::SpriteSystem() {};

    void SpriteSystem::init( Game & game, Map & map )
    {
        sprites_.emplace_back( createAutumnSprite() );
        sprites_[ sprites_.size() - 1 ].init( game );
        std::vector<std::vector<Map::SpriteData>> sprites_list = map.getSprites();
        for ( int y = 0; y < map.getHeightBlocks(); ++y )
        {
            for ( int x = 0; x < map.getWidthBlocks(); ++x )
            {
                std::vector<Map::SpriteData> & inner = sprites_list[ map.getIFromXAndY( x, y ) ];
                for ( Map::SpriteData & item : inner )
                {
                    if ( item.type )
                    {
                        switch ( item.type )
                        {
                            case ( 1 ):
                            {
                                sprites_.emplace_back( createCrabSprite( x, y ) );
                            }
                            break;
                            case ( 2 ):
                            {
                                sprites_.emplace_back( createBadAppleSprite( x, y, Dir::X::LEFT ) );
                            }
                            break;
                            case ( 3 ):
                            {
                                sprites_.emplace_back( createBadAppleSprite( x, y, Dir::X::RIGHT ) );
                            }
                            break;
                            case ( 5 ):
                            {
                                sprites_.emplace_back( createGrumpTruckSprite( x, y, Dir::X::LEFT ) );
                            }
                            break;
                            case ( 15 ):
                            {
                                sprites_.emplace_back( createScaleLiftSprite( x, y, Dir::X::LEFT ) );
                            }
                            break;
                            case ( 16 ):
                            {
                                sprites_.emplace_back( createScaleLiftSprite( x, y, Dir::X::RIGHT ) );
                            }
                            break;
                            default:
                            {
                                throw std::runtime_error( "Invalid sprite #" + std::to_string( item.type ) );
                            }
                            break;
                        }
                        sprites_[ sprites_.size() - 1 ].init( game );
                    }
                }
            }
        }
    };

    void SpriteSystem::update( float dt, const Controller & controller, Level & level, Game & game )
    {
        for ( unsigned int i = 0; i < sprites_.size(); ++i )
        {
            Sprite & sprite = sprites_[ i ];
            sprite.update( dt, controller, level, game, sprites_ );
        }
    };


    Sprite SpriteSystem::createAutumnSprite()
    {
        Sprite autumn
        {
            Sprite::Type::AUTUMN,
            { 64.0f, 64.0f, 16.0f, 25.0f },
            { Sprite::Attribute::PROTAG }
        };
        autumn.misc_.autumn.jump_padding = 0.0f;
        autumn.misc_.autumn.walk_frame = 0;
        autumn.misc_.autumn.jump_lock = false;
        autumn.misc_.autumn.invincibility_ = 0.0f;
        return autumn;
    };

    Sprite SpriteSystem::createCrabSprite( float x, float y )
    {
        return
        {
            Sprite::Type::CRAB,
            Rect{ blocksToPixels( x ), blocksToPixels( y ), 16.0f, 16.0f },
            {},
            ArgList{
                { "startspeed", 0.1f },
                { "maxspeed", 0.8f }
            }
        };
    };

    Sprite SpriteSystem::createBadAppleSprite( float x, float y, Dir::X dir )
    {
        return
        {
            Sprite::Type::BADAPPLE,
            Rect{ blocksToPixels( x ), blocksToPixels( y ) - 2.0f, 16.0f, 16.0f },
            {},
            ArgList{
                { "startspeed", 0.1f },
                { "maxspeed", 0.8f },
                { "dirx", dir }
            }
        };
    };

    Sprite SpriteSystem::createGrumpTruckSprite( float x, float y, Dir::X dir )
    {
        return
        {
            Sprite::Type::TRUCK,
            Rect{ blocksToPixels( x ), blocksToPixels( y ) - 16.0f, 40.0f, 32.0f },
            {},
            ArgList{
                { "startspeed", 0.2f },
                { "maxspeed", 1.5f },
                { "dirx", dir }
            }
        };
    };
}