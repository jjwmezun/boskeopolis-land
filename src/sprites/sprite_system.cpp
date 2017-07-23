#include "bad_apple_sprite.h"
#include "block_system.h"
#include "bouncy_cloud_block_sprite.h"
#include "buzz_saw_sprite.h"
#include "cactooie_spine_sprite.h"
#include "cactooie_sprite.h"
#include "camera.h"
#include "cloud_block_sprite.h"
#include "cloud_monster_sprite.h"
#include "cloud_platform_sprite.h"
#include "direction.h"
#include "eggnon_sprite.h"
#include "enemy_cart_sprite.h"
#include "event_system.h"
#include "falling_bough_sprite.h"
#include "fishstick_sprite.h"
#include "guard_sprite.h"
#include "handgun_sprite.h"
#include "heat_beam_sprite.h"
#include "hydrant_graphics.h"
#include "hydrant_sprite.h"
#include "iceblock_sprite.h"
#include "icecube_sprite.h"
#include <iostream>
#include "level.h"
#include "lightning_sprite.h"
#include "lil_pipe_monster_sprite.h"
#include "map.h"
#include "maze_chaser_sprite.h"
#include "mezun_exceptions.h"
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
#include "shroud_sprite.h"
#include "sillyfish_sprite.h"
#include "snowball_sprite.h"
#include "snowboulder_sprite.h"
#include "snowman_sprite.h"
#include "spike_egg_sprite.h"
#include "spiky_fruit_sprite.h"
#include "sprite_component_circle.h"
#include "sprite_component_right_and_left.h"
#include "sprite_component_up_and_down.h"
#include "sprite_system.h"

SpriteSystem::SpriteSystem( int entrance_x, int entrance_y )
:
	hero_ ( new OverworldPlayerSprite( entrance_x, entrance_y ) )
{};

SpriteSystem::~SpriteSystem() {};

std::unique_ptr<Sprite> SpriteSystem::spriteType( int type, int x, int y, const Map& lvmap )
{
	x = Unit::SubPixelsToPixels( x );
	y = Unit::SubPixelsToPixels( y );

	switch ( type )
	{
		case ( SPRITE_INDEX_START + 0 ):
			return std::unique_ptr<Sprite> ( new HydrantSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 1 ):
			return std::unique_ptr<Sprite> ( new GuardSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 2 ):
			return std::unique_ptr<Sprite> ( new PlatformSprite( x, y, Direction::Simple::RIGHT ) );
		break;
		case ( SPRITE_INDEX_START + 3 ):
			return std::unique_ptr<Sprite> ( new PlatformSprite( x, y, Direction::Simple::UP, 64000 ) );
		break;
		case ( SPRITE_INDEX_START + 4 ):
			return std::unique_ptr<Sprite> ( new BadAppleSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 5 ):
			return std::unique_ptr<Sprite> ( new SpikyFruitSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 6 ):
			return std::unique_ptr<Sprite> ( new RacerSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 7 ):
			return std::unique_ptr<Sprite> ( new FallingBoughSprite( x, y, Direction::Horizontal::LEFT ) );
		break;
		case ( SPRITE_INDEX_START + 8 ):
			return std::unique_ptr<Sprite> ( new FallingBoughSprite( x, y, Direction::Horizontal::RIGHT ) );
		break;
		case ( SPRITE_INDEX_START + 9 ):
			return std::unique_ptr<Sprite> ( new PufferbeeSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 10 ):
			return std::unique_ptr<Sprite> ( new PufferbeeSprite( x, y, std::unique_ptr<SpriteComponent> ( new SpriteComponentUpAndDown( 4 ) ) ) );
		break;
		case ( SPRITE_INDEX_START + 11 ):
			return std::unique_ptr<Sprite> ( new PufferbeeSprite( x, y, std::unique_ptr<SpriteComponent> ( new SpriteComponentRightAndLeft( 6 ) ) ) );
		break;
		case ( SPRITE_INDEX_START + 12 ):
			return std::unique_ptr<Sprite> ( new PufferbeeSprite( x, y, std::unique_ptr<SpriteComponent> ( new SpriteComponentCircle() ) ) );
		break;
		case ( SPRITE_INDEX_START + 13 ):
			return std::unique_ptr<Sprite> ( new RopeSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 14 ):
			return std::unique_ptr<Sprite> ( new RopeSprite( x, y, 10, 52, 1600 ) );
		break;
		case ( SPRITE_INDEX_START + 15 ):
			return std::unique_ptr<Sprite> ( new HandgunSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 16 ):
			return std::unique_ptr<Sprite> ( new BuzzSawSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 17 ):
			return std::unique_ptr<Sprite> ( new EnemyCartSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 18 ):
			return std::unique_ptr<Sprite> ( new SewerMonsterSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 19 ):
			return std::unique_ptr<Sprite> ( new IceCubeSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 20 ):
			return std::unique_ptr<Sprite> ( new IceCubeSprite( x, y, Direction::Horizontal::LEFT, true ) );
		break;
		case ( SPRITE_INDEX_START + 21 ):
			return std::unique_ptr<Sprite> ( new IceBlockSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 22 ):
			return std::unique_ptr<Sprite> ( new PenguinSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 23 ):
			return std::unique_ptr<Sprite> ( new FishstickSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 24 ):
			return std::unique_ptr<Sprite> ( new SillyfishSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 25 ):
			return std::unique_ptr<Sprite> ( new EggnonSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 26 ):
			return std::unique_ptr<Sprite> ( new MazeChaserSprite( x, y, MazeChaserSprite::Type::SHADOW ) );
		break;
		case ( SPRITE_INDEX_START + 27 ):
			return std::unique_ptr<Sprite> ( new MazeChaserSprite( x, y, MazeChaserSprite::Type::AMBUSH ) );
		break;
		case ( SPRITE_INDEX_START + 28 ):
			return std::unique_ptr<Sprite> ( new MazeChaserSprite( x, y, MazeChaserSprite::Type::RANDO ) );
		break;
		case ( SPRITE_INDEX_START + 29 ):
			return std::unique_ptr<Sprite> ( new MazeChaserSprite( x, y, MazeChaserSprite::Type::OBFUSCATING ) );
		break;
		case ( SPRITE_INDEX_START + 30 ):
			return std::unique_ptr<Sprite> ( new HeatBeamSprite( x, y, HeatBeamSprite::Type::ODD ) );
		break;
		case ( SPRITE_INDEX_START + 31 ):
			return std::unique_ptr<Sprite> ( new HeatBeamSprite( x, y, HeatBeamSprite::Type::EVEN ) );
		break;
		case ( SPRITE_INDEX_START + 32 ):
			return std::unique_ptr<Sprite> ( new CloudPlatformSprite( x, y, Direction::Horizontal::RIGHT ) );
		break;
		case ( SPRITE_INDEX_START + 33 ):
			return std::unique_ptr<Sprite> ( new CloudPlatformSprite( x, y, Direction::Horizontal::LEFT ) );
		break;
		case ( SPRITE_INDEX_START + 34 ):
			return std::unique_ptr<Sprite> ( new CloudBlockSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 35 ):
			return std::unique_ptr<Sprite> ( new BouncyCloudBlockSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 36 ):
			return std::unique_ptr<Sprite> ( new LilPipeMonsterSprite( x, y, Direction::Vertical::UP ) );
		break;
		case ( SPRITE_INDEX_START + 37 ):
			return std::unique_ptr<Sprite> ( new LilPipeMonsterSprite( x, y, Direction::Vertical::DOWN ) );
		break;
		case ( SPRITE_INDEX_START + 38 ):
			return std::unique_ptr<Sprite> ( new ShroudSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 39 ):
			return std::unique_ptr<Sprite> ( new CactooieSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 40 ):
			return std::unique_ptr<Sprite> ( new CloudMonsterSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 41 ):
			return std::unique_ptr<Sprite> ( new SnowmanSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 42 ):
			return std::unique_ptr<Sprite> ( new SnowboulderSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 43 ):
			return std::unique_ptr<Sprite> ( new FallingBoughSprite( x, y, Direction::Horizontal::LEFT, false ) );
		break;
		case ( SPRITE_INDEX_START + 44 ):
			return std::unique_ptr<Sprite> ( new FallingBoughSprite( x, y, Direction::Horizontal::RIGHT, false ) );
		break;
		case ( SPRITE_INDEX_START + 63 ):
			return std::unique_ptr<Sprite> ( new SawSprite( x, y ) );
		break;
		default:
			throw mezun::InvalidSprite( type );
		break;
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

void SpriteSystem::spawnCactooieSpine( int x, int y, Direction::Horizontal direction )
{
	sprites_.emplace_back( new CactooieSpineSprite( x, y, direction ) );
};

void SpriteSystem::spawnSnowball( int x, int y, Direction::Horizontal direction )
{
	sprites_.emplace_back( new SnowballSprite( x, y, direction ) );
};

void SpriteSystem::spritesFromMap( const Map& lvmap )
{
	for ( int i = 0; i < lvmap.spritesSize(); ++i )
	{
		const int x = Unit::BlocksToSubPixels( lvmap.mapX( i ) );
		const int y = Unit::BlocksToSubPixels( lvmap.mapY( i ) );
		const int type = lvmap.sprite( i ) - 1;

		if ( type != -1 )
		{
			std::unique_ptr<Sprite> new_sprite = std::move( spriteType( type, x, y, lvmap ) );
			sprites_.emplace_back( std::move( new_sprite ) );
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

void SpriteSystem::reset( const Level& level, const InventoryLevel& inventory )
{
	clearSprites();
	Sprite::resistance_x_ = Sprite::RESISTANCE_X_NORMAL;
	spritesFromMap( level.currentMap() );

	if ( level.currentMap().slippery() )
	{
		Sprite::traction_ = Sprite::TRACTION_ICY;
	}
	else
	{
		Sprite::traction_ = Sprite::TRACTION_NORMAL;
	}

	switch( level.currentMap().heroType() )
	{
		case ( HeroType::NORMAL ):
			hero_.reset( new PlayerSprite( level.entranceX(), level.entranceY(), 2, hero_->hp() ) );
		break;
		case ( HeroType::OVERWORLD ):
			hero_.reset( new OverworldPlayerSprite( level.entranceX(), level.entranceY() ) );
		break;
		case ( HeroType::CART ):
			hero_.reset( new PlayerCartSprite( level.entranceX(), level.entranceY(), 2, hero_->hp()  ) );
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

void SpriteSystem::update( Camera& camera, Map& lvmap, EventSystem& events, BlockSystem& blocks )
{
	for ( int i = 0; i < sprites_.size(); ++i )
	{
		if ( sprites_.at( i )->despawnWhenDead() )
		{
			if ( sprites_.at( i )->deathFinished() )
			{
				destroySprite( i );
				continue; // SPRITE IS DEAD--DO NOT TRY UPDATING.
			}
		}

		switch( sprites_.at( i )->cameraMovement() )
		{
			case ( Sprite::CameraMovement::PAUSE_OFFSCREEN ):
				if ( camera.onscreen( sprites_.at( i )->hitBox(), OFFSCREEN_PADDING ) )
				{
					sprites_.at( i )->update( camera, lvmap, events, *this, blocks );
				}
			break;

			case ( Sprite::CameraMovement::PERMANENT ):
				sprites_.at( i )->update( camera, lvmap, events, *this, blocks );
			break;

			case ( Sprite::CameraMovement::RESET_INSTANTLY_OFFSCREEN ):
				if ( camera.onscreen( sprites_.at( i )->hitBox(), OFFSCREEN_PADDING ) )
				{
					sprites_.at( i )->update( camera, lvmap, events, *this, blocks );
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
					sprites_.at( i )->update( camera, lvmap, events, *this, blocks );
				}
				else
				{
					if ( camera.offscreen( sprites_.at( i )->originalHitBox(), OFFSCREEN_PADDING ) )
					{
						sprites_.at( i )->reset();
					}
				}
			break;

			case ( Sprite::CameraMovement::DESPAWN_OFFSCREEN ):
				if ( camera.onscreen( sprites_.at( i )->hitBox(), OFFSCREEN_PADDING ) )
				{
					sprites_.at( i )->update( camera, lvmap, events, *this, blocks );
				}
				else
				{
					destroySprite( i );
				}
			break;
		}
	}
	hero_->update( camera, lvmap, events, *this, blocks );

	if ( hero_->deathFinished() )
	{
		events.fail();
	}
};

void SpriteSystem::spriteInteraction( Camera& camera, BlockSystem& blocks, Map& lvmap )
{
	for ( int i = 0; i < sprites_.size(); ++i )
	{
		if ( sprites_.at( i ) != nullptr )
		{
			if ( camera.onscreen( sprites_.at( i )->hitBox(), OFFSCREEN_PADDING ) || sprites_.at( i )->cameraMovement() == Sprite::CameraMovement::PERMANENT )
			{
				if ( sprites_.at( i )->interactsWithSprites() && hero_->interactsWithSprites() )
				{
					sprites_.at( i )->interact( *hero_, blocks, *this, lvmap );
					hero_->interact( *sprites_.at( i ), blocks, *this, lvmap );
				}

				for ( int j = 0; j < sprites_.size(); ++j )
				{
					if ( sprites_[ j ] != nullptr )
						if ( i != j )
							if ( camera.onscreen( sprites_[ j ]->hitBox(), OFFSCREEN_PADDING ) )
								if ( sprites_.at( i )->interactsWithSprites() && sprites_[ j ]->interactsWithSprites() )
									sprites_.at( i )->interact( *sprites_[ j ], blocks, *this, lvmap );
				}
			}
		}
	}
};

void SpriteSystem::render( Camera& camera, bool priority )
{
	for ( int i = 0; i < sprites_.size(); ++i )
	{
		if ( sprites_.at( i ) != nullptr )
		{
			sprites_.at( i )->render( camera, priority );
		}
	}

	if ( !priority )
	{
		hero_->render( camera );
	}
};

Sprite& SpriteSystem::hero()
{
	return *hero_;
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

void SpriteSystem::interactWithMap( Map& lvmap, Camera& camera )
{
	for ( auto& s : sprites_ )
	{
		lvmap.interact( *s, camera );
	}

	lvmap.interact( *hero_, camera );
};

void SpriteSystem::testNumOSprites() const
{
	std::cout<<"Num o' sprites: "<<sprites_.size()<<std::endl;
};