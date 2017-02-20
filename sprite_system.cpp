


// Name
//===================================
//
// SpriteSystem
//


// DEPENDENCIES
//===================================

    #include "bad_apple_sprite.h"
    #include "block_system.h"
    #include "buzz_saw_sprite.h"
    #include "camera.h"
    #include "cloud_platform_sprite.h"
    #include "direction.h"
    #include "eggnon_sprite.h"
    #include "enemy_cart_sprite.h"
    #include "event_system.h"
    #include "falling_bough_sprite.h"
    #include "fishstick_sprite.h"
    #include "guard_sprite.h"
    #include "handgun_sprite.h"
    #include "hydrant_graphics.h"
    #include "hydrant_sprite.h"
    #include "iceblock_sprite.h"
    #include "icecube_sprite.h"
    #include "level.h"
    #include "map.h"
    #include "maze_chaser_sprite.h"
    #include "overworld_player_sprite.h"
    #include "penguin_sprite.h"
    #include "platform_sprite.h"
    #include "player_cart_sprite.h"
    #include "player_graphics.h"
    #include "player_sprite_fluttering.h"
    #include "pufferbee_sprite.h"
    #include "racer_sprite.h"
    #include "rope_sprite.h"
    #include "saw_sprite.h"
    #include "sewer_monster_sprite.h"
    #include "sillyfish_sprite.h"
    #include "spike_egg_sprite.h"
    #include "spiky_fruit_sprite.h"
    #include "sprite_component_circle.h"
    #include "sprite_component_right_and_left.h"
    #include "sprite_component_up_and_down.h"
    #include "sprite_system.h"
    #include "water_effect.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    SpriteSystem::SpriteSystem( int entrance_x, int entrance_y )
    :
        hero_ ( new PlayerSprite( entrance_x, entrance_y ) )
    {};

    SpriteSystem::~SpriteSystem() {};

    std::unique_ptr<Sprite> SpriteSystem::spriteType( int type, int x, int y )
    {
        x = Unit::SubPixelsToPixels( x );
        y = Unit::SubPixelsToPixels( y );

        switch ( type )
        {
            case ( 1 ):
                return std::unique_ptr<Sprite> ( new GuardSprite( x, y ) );
            break;
            case ( 2 ):
                return std::unique_ptr<Sprite> ( new PlatformSprite( x, y, Direction::Simple::RIGHT ) );
            break;
            case ( 3 ):
                return std::unique_ptr<Sprite> ( new PlatformSprite( x, y, Direction::Simple::UP, 64000 ) );
            break;
            case ( 4 ):
                return std::unique_ptr<Sprite> ( new BadAppleSprite( x, y ) );
            break;
            case ( 5 ):
                return std::unique_ptr<Sprite> ( new SpikyFruitSprite( x, y ) );
            break;
            case ( 6 ):
                return std::unique_ptr<Sprite> ( new RacerSprite( x, y ) );
            break;
            case ( 7 ):
                return std::unique_ptr<Sprite> ( new FallingBoughSprite( x, y, Direction::Horizontal::LEFT ) );
            break;
            case ( 8 ):
                return std::unique_ptr<Sprite> ( new FallingBoughSprite( x, y, Direction::Horizontal::RIGHT ) );
            break;
            case ( 9 ):
                return std::unique_ptr<Sprite> ( new PufferbeeSprite( x, y ) );
            break;
            case ( 10 ):
                return std::unique_ptr<Sprite> ( new PufferbeeSprite( x, y, std::unique_ptr<SpriteComponent> ( new SpriteComponentUpAndDown( 4 ) ) ) );
            break;
            case ( 11 ):
                return std::unique_ptr<Sprite> ( new PufferbeeSprite( x, y, std::unique_ptr<SpriteComponent> ( new SpriteComponentRightAndLeft( 6 ) ) ) );
            break;
            case ( 12 ):
                return std::unique_ptr<Sprite> ( new PufferbeeSprite( x, y, std::unique_ptr<SpriteComponent> ( new SpriteComponentCircle() ) ) );
            break;
            case ( 13 ):
                return std::unique_ptr<Sprite> ( new RopeSprite( x, y ) );
            break;
            case ( 14 ):
                return std::unique_ptr<Sprite> ( new RopeSprite( x, y, 10, 52, 1600 ) );
            break;
            case ( 15 ):
                return std::unique_ptr<Sprite> ( new HandgunSprite( x, y ) );
            break;
            case ( 16 ):
                return std::unique_ptr<Sprite> ( new BuzzSawSprite( x, y ) );
            break;
            case ( 17 ):
                return std::unique_ptr<Sprite> ( new EnemyCartSprite( x, y ) );
            break;
            case ( 18 ):
                return std::unique_ptr<Sprite> ( new SewerMonsterSprite( x, y ) );
            break;
            case ( 19 ):
                return std::unique_ptr<Sprite> ( new IceCubeSprite( x, y ) );
            break;
            case ( 20 ):
                return std::unique_ptr<Sprite> ( new IceCubeSprite( x, y, Direction::Horizontal::LEFT, true ) );
            break;
            case ( 21 ):
                return std::unique_ptr<Sprite> ( new IceBlockSprite( x, y ) );
            break;
            case ( 22 ):
                return std::unique_ptr<Sprite> ( new PenguinSprite( x, y ) );
            break;
            case ( 23 ):
                return std::unique_ptr<Sprite> ( new FishstickSprite( x, y ) );
            break;
            case ( 24 ):
                return std::unique_ptr<Sprite> ( new SillyfishSprite( x, y ) );
            break;
            case ( 25 ):
                return std::unique_ptr<Sprite> ( new EggnonSprite( x, y ) );
            break;
            case ( 26 ):
                return std::unique_ptr<Sprite> ( new MazeChaserSprite( x, y, MazeChaserSprite::Type::SHADOW ) );
            break;
            case ( 27 ):
                return std::unique_ptr<Sprite> ( new MazeChaserSprite( x, y, MazeChaserSprite::Type::AMBUSH ) );
            break;
            case ( 28 ):
                return std::unique_ptr<Sprite> ( new MazeChaserSprite( x, y, MazeChaserSprite::Type::RANDO ) );
            break;
            case ( 29 ):
                return std::unique_ptr<Sprite> ( new MazeChaserSprite( x, y, MazeChaserSprite::Type::OBFUSCATING ) );
            break;
            case ( 62 ):
                return std::unique_ptr<Sprite> ( new CloudPlatformSprite( x, y ) );
            break;
            case ( 63 ):
                return std::unique_ptr<Sprite> ( new SawSprite( x, y ) );
            break;
            default:
                return std::unique_ptr<Sprite> ( new HydrantSprite( x, y ) );
        }
    };

    void SpriteSystem::spawn( SpawnSprite type, int x, int y )
    {
        switch( type )
        {
            case ( SpawnSprite::SPIKE_EGG ):
                sprites_.emplace_back( new SpikeEggSprite( x, y ) );
            break;
        }
    };

    void SpriteSystem::spritesFromMap( Map& lvmap )
    {
        int x = 0;
        int y = 0;

        for ( int i = 0; i < lvmap.spritesSize(); ++i )
        {
            x = Unit::BlocksToSubPixels( lvmap.mapX( i ) );
            y = Unit::BlocksToSubPixels( lvmap.mapY( i ) );

            const int type = lvmap.sprite( i ) - 1;

            if ( type > -1 )
            {
                sprites_.emplace_back( std::move( spriteType( type, x, y ) ) );
            }
        }
    };

    void SpriteSystem::interact( BlockSystem& blocks, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera )
    {
        for ( int i = 0; i < sprites_.size(); ++i )
        {
            if ( sprites_.at( i ) != nullptr )
            {
                if ( sprites_.at( i )->interactsWithBlocks() )
                {
                    if
                    (
                        camera.onscreen( sprites_.at( i )->hitBox(), OFFSCREEN_PADDING ) ||
                        sprites_.at( i )->hasCameraMovement( Sprite::CameraMovement::PERMANENT )
                    )
                    {
                        blocks.interact( *sprites_.at( i ), level, events, inventory, camera );
                    }
                }
            }
        }

        if ( hero_->interactsWithBlocks() )
        {
            blocks.interact( *hero_, level, events, inventory, camera );
        }
    };

    void SpriteSystem::reset( Level& level, InventoryLevel& inventory )
    {
        clearSprites();
        Sprite::resistance_x_ = Sprite::RESISTANCE_X_NORMAL;
        spritesFromMap( level.currentMap() );

        if ( level.currentMap().slippery() )
            Sprite::traction_ = Sprite::TRACTION_ICY;
        else
            Sprite::traction_ = Sprite::TRACTION_NORMAL;

        switch( level.currentMap().heroType() )
        {
            case ( HeroType::NORMAL ):
                hero_.reset( new PlayerSprite( level.entranceX(), level.entranceY(), 2, hero_->hp() ) );
            break;
            case ( HeroType::OVERWORLD ):
                hero_.reset( new OverworldPlayerSprite( level.entranceX(), level.entranceY() ) );
            break;
            case ( HeroType::CART ):
                hero_.reset( new PlayerCartSprite( level.entranceX(), level.entranceY() ) );
            break;
            case ( HeroType::FLUTTERING ):
                hero_.reset( new PlayerSpriteFluttering( level.entranceX(), level.entranceY() ) );
            break;
        }

    };

    void SpriteSystem::clearSprites()
    {
        sprites_.clear();
    };

    void SpriteSystem::destroySprite( int n )
    {
        if ( n < sprites_.size() )
        {
            sprites_.erase( sprites_.begin() + n );
        }
    };

    void SpriteSystem::update( Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, BlockSystem& blocks )
    {
        for ( int i = 0; i < sprites_.size(); ++i )
        {
            if ( sprites_.at( i ) != nullptr )
            {

                switch( sprites_.at( i )->cameraMovement() )
                {
                    case ( Sprite::CameraMovement::PAUSE_OFFSCREEN ):
                        if ( camera.onscreen( sprites_.at( i )->hitBox(), OFFSCREEN_PADDING ) )
                        {
                            sprites_.at( i )->update( input, camera, lvmap, game, events, *this, blocks );
                        }
                    break;

                    case ( Sprite::CameraMovement::PERMANENT ):
                        sprites_.at( i )->update( input, camera, lvmap, game, events, *this, blocks );
                    break;

                    case ( Sprite::CameraMovement::RESET_INSTANTLY_OFFSCREEN ):
                        if ( camera.onscreen( sprites_.at( i )->hitBox(), OFFSCREEN_PADDING ) )
                        {
                            sprites_.at( i )->update( input, camera, lvmap, game, events, *this, blocks );
                        }
                        else
                        {
                            if ( camera.onscreen( sprites_.at( i )->originalHitBox(), OFFSCREEN_PADDING ) )
                            {
                                sprites_.at( i )->reset();
                            }
                        }
                    break;

                    case ( Sprite::CameraMovement::RESET_OFFSCREEN_AND_AWAY ):
                        if ( camera.onscreen( sprites_.at( i )->hitBox(), OFFSCREEN_PADDING ) )
                        {
                            sprites_.at( i )->update( input, camera, lvmap, game, events, *this, blocks );
                        }
                        else
                        {
                            if ( camera.offscreen( sprites_.at( i )->originalHitBox(), OFFSCREEN_PADDING ) )
                            {
                                sprites_.at( i )->reset();
                            }
                        }
                    break;
                }

                if ( sprites_.at( i )->despawnWhenDead() )
                {
                    if ( sprites_.at( i )->deathFinished() )
                    {
                        destroySprite( i );
                    }
                }
            }
        }
        hero_->update( input, camera, lvmap, game, events, *this, blocks );

        if ( hero_->deathFinished() )
        {
            events.failed_ = true;
        }
    };

    void SpriteSystem::spriteInteraction( Camera& camera, BlockSystem& blocks )
    {
        for ( int i = 0; i < sprites_.size(); ++i )
        {
            if ( sprites_.at( i ) != nullptr )
            {
                if ( camera.onscreen( sprites_.at( i )->hitBox(), OFFSCREEN_PADDING ) || sprites_.at( i )->cameraMovement() == Sprite::CameraMovement::PERMANENT )
                {
                    if ( sprites_.at( i )->interactsWithSprites() && hero_->interactsWithSprites() )
                    {
                        sprites_.at( i )->interact( *hero_, blocks, *this );
                        hero_->interact( *sprites_.at( i ), blocks, *this );
                    }

                    for ( int j = 0; j < sprites_.size(); ++j )
                    {
                        if ( sprites_[ j ] != nullptr )
                            if ( i != j )
                                if ( camera.onscreen( sprites_[ j ]->hitBox(), OFFSCREEN_PADDING ) )
                                    if ( sprites_.at( i )->interactsWithSprites() && sprites_[ j ]->interactsWithSprites() )
                                        sprites_.at( i )->interact( *sprites_[ j ], blocks, *this );
                    }
                }
            }
        }
    };

    void SpriteSystem::render( Graphics& graphics, Camera& camera, bool priority )
    {
        for ( int i = 0; i < sprites_.size(); ++i )
        {
            if ( sprites_.at( i ) != nullptr )
            {
                sprites_.at( i )->render( graphics, camera, priority );
            }
        }

        if ( !priority )
        {
            hero_->render( graphics, camera );
        }
    };

    Sprite& SpriteSystem::hero()
    {
        return *hero_;
    };

    void SpriteSystem::effects( WaterEffect* water_effect )
    {
        if ( water_effect != nullptr )
        {
            for ( int i = 0; i < sprites_.size(); ++i )
            {
                water_effect->testSprite( sprites_.at( i ).get() );
            }
            water_effect->testSprite( hero_.get() );
        }
    };

    SpriteSystem::HeroType SpriteSystem::heroType( std::string property )
    {
        if ( property.compare( "CART" ) == 0 )
        {
            return HeroType::CART;
        }
        else if ( property.compare( "FLUTTERING" ) == 0 )
        {
            return HeroType::FLUTTERING;
        }
        else if ( property.compare( "OVERWORLD" ) == 0 )
        {
            return HeroType::OVERWORLD;
        }

        return HeroType::NORMAL;
    };