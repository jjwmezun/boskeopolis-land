#include "map.hpp"
#include "sprite_system.hpp"

namespace BSL
{
    Sprite createAutumnSprite()
    {
        return
        {
            Sprite::Type::AUTUMN,
            { 64.0f, 64.0f, 16.0f, 25.0f }
        };
    };

    SpriteSystem::SpriteSystem()
    :
        autumn_ ( createAutumnSprite() )
    {};

    void SpriteSystem::init( Game & game, Map & map )
    {
        autumn_.init( game );
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
                                sprites_.emplace_back
                                (
                                    Sprite::Type::CRAB,
                                    Rect{ blocksToPixels( x ), blocksToPixels( y ), 16.0f, 16.0f },
                                    ArgList{
                                        { "startspeed", 0.1f },
                                        { "maxspeed", 1.0f }
                                    }
                                );
                                sprites_[ sprites_.size() - 1 ].init( game );
                            }
                            break;
                        }
                    }
                }
            }
        }
    };

    void SpriteSystem::update( float dt, const Controller & controller, Level & level, Game & game )
    {
        autumn_.update( dt, controller, level, game );
        for ( unsigned int i = 0; i < sprites_.size(); ++i )
        {
            Sprite & sprite = sprites_[ i ];
            sprite.update( dt, controller, level, game );

            autumn_.interact( sprite, level, game );
            sprite.interact( autumn_, level, game );

            for ( unsigned int j = 0; j < sprites_.size(); ++j )
            {
                // Don't make sprite interact with self.
                if ( i == j ) continue;

                Sprite & other = sprites_[ j ];

                sprite.interact( other, level, game );
                other.interact( sprite, level, game );
            }
        }
    };
}