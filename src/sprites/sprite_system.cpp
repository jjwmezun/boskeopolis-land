#include "angry_truck_sprite.hpp"
#include "angry_truck_generator_sprite.hpp"
#include "ant_sprite.hpp"
#include "bad_apple_sprite.hpp"
#include "barrel_o_gorillas_sprite.hpp"
#include "bat_sprite.hpp"
#include "bird_sprite.hpp"
#include "block_system.hpp"
#include "bouncy_cloud_block_sprite.hpp"
#include "bubble_sprite.hpp"
#include "bullet_sprite.hpp"
#include "buzz_saw_sprite.hpp"
#include "cactooie_spine_sprite.hpp"
#include "cactooie_sprite.hpp"
#include "camera.hpp"
#include "change_camera_autoscroll_sprite.hpp"
#include "cannon_sprite.hpp"
#include "capn_clearbeard_sprite.hpp"
#include "carrot_sprite.hpp"
#include "change_layer_sprite.hpp"
#include "choque_sprite.hpp"
#include "circle_block_sprite.hpp"
#include "cloud_block_sprite.hpp"
#include "cloud_monster_sprite.hpp"
#include "cloud_platform_sprite.hpp"
#include "cowpoker_sprite.hpp"
#include "crab_sprite.hpp"
#include "desert_hawk_sprite.hpp"
#include "direction.hpp"
#include "downhill_gem_sprite.hpp"
#include "dungeon_enemy_sprite.hpp"
#include "dungeon_switch_sprite.hpp"
#include "eggnon_sprite.hpp"
#include "enemy_cart_sprite.hpp"
#include "event_system.hpp"
#include "falling_snow_boulder_spawn_sprite.hpp"
#include "faucet_sprite.hpp"
#include "falling_bough_sprite.hpp"
#include "falling_fist_sprite.hpp"
#include "falling_gem_sprite.hpp"
#include "fishstick_sprite.hpp"
#include "full_heal_block_sprite.hpp"
#include "gemmy_sprite.hpp"
#include "ghost_sprite.hpp"
#include "gorilla_sprite.hpp"
#include "guard_sprite.hpp"
#include "handgun_sprite.hpp"
#include "health.hpp"
#include "heat_beam_sprite.hpp"
#include "hieroglyph_pusher_sprite.hpp"
#include "hydrant_sprite.hpp"
#include "ice_box_rock_solid_hack_sprite.hpp"
#include "iceblock_sprite.hpp"
#include "icecube_sprite.hpp"
#include "icicle_sprite.hpp"
#include <iostream>
#include "level.hpp"
#include "lava_platform_sprite.hpp"
#include "lifesaver_sprite.hpp"
#include "lightning_sprite.hpp"
#include "lil_pipe_monster_sprite.hpp"
#include "locked_door_sprite.hpp"
#include "map.hpp"
#include "maze_player_sprite.hpp"
#include "maze_chaser_sprite.hpp"
#include "message_block_sprite.hpp"
#include "mezun_exceptions.hpp"
#include "moon_sprite.hpp"
#include "move_water_sprite.hpp"
#include "olive_spawner_sprite.hpp"
#include "olive_sprite.hpp"
#include "pelican_sprite.hpp"
#include "penguin_sprite.hpp"
#include "pipe_eel_sprite.hpp"
#include "platform_sprite.hpp"
#include "player_car_sprite.hpp"
#include "player_cart_sprite.hpp"
#include "player_graphics.hpp"
#include "player_open_chest_sprite.hpp"
#include "player_spaceship_sprite.hpp"
#include "player_sprite_fluttering.hpp"
#include "pi_block_sprite.hpp"
#include "pollo_sprite.hpp"
#include "pollo_del_aire_sprite.hpp"
#include "pollo_del_aire_circle_sprite.hpp"
#include "pollo_del_aire_still_sprite.hpp"
#include "pollo_del_aire_vertical_sprite.hpp"
#include "pufferbee_sprite.hpp"
#include "pufferbee_square_sprite.hpp"
#include "quadrapus_sprite.hpp"
#include "racer_sprite.hpp"
#include "random_treasure_chest_sprite.hpp"
#include "rising_package_sprite.hpp"
#include "rising_ice_cube_sprite.hpp"
#include "rising_platform_sprite.hpp"
#include "rolling_ball_sprite.hpp"
#include "rope_sprite.hpp"
#include "rotating_block_sprite.hpp"
#include "saw_sprite.hpp"
#include "sewer_monster_sprite.hpp"
#include "shmup_bullet_sprite.hpp"
#include "shmup_enemy_sprite.hpp"
#include "shmup_shooter_sprite.hpp"
#include "shooter_player_sprite.hpp"
#include "shroud_sprite.hpp"
#include "sillyfish_sprite.hpp"
#include "snowball_sprite.hpp"
#include "snowboulder_sprite.hpp"
#include "snowman_sprite.hpp"
#include "spawn_anchor_missile_sprite.hpp"
#include "spawn_icicle_sprite.hpp"
#include "spider_sprite.hpp"
#include "spike_egg_sprite.hpp"
#include "spiky_fruit_sprite.hpp"
#include "spring_sprite.hpp"
#include "sprite_component_circle.hpp"
#include "sprite_component_right_and_left.hpp"
#include "sprite_component_up_and_down.hpp"
#include "sprite_system.hpp"
#include "stronger_cowpoker_sprite.hpp"
#include "switch_block_sprite.hpp"
#include "tall_tombstone_sprite.hpp"
#include "top_down_player_sprite.hpp"
#include "top_down_moveable_block_sprite.hpp"
#include "treasure_chest_sprite.hpp"
#include "truck_platform_sprite.hpp"
#include "underground_subway_sprite.hpp"
#include "urban_bird_sprite.hpp"
#include "volcano_monster_sprite.hpp"
#include "waterdrop_sprite.hpp"
#include "waterdrop_spawner_sprite.hpp"
#include "weight_platform_sprite.hpp"

SpriteSystem::SpriteSystem( int entrance_x, int entrance_y )
:
	hero_ (),
	permanently_killed_enemies_ ( 0 )
{
	// Minimize chances o' sprite # going past space
	// & forcing slow vector relocation.
	sprites_.reserve( 50 );
};

SpriteSystem::~SpriteSystem() {};

std::unique_ptr<Sprite> SpriteSystem::spriteType( int type, int x, int y, int i, const Map& lvmap, EventSystem& events )
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
			return std::unique_ptr<Sprite> ( new RisingPlatformSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 4 ):
			return std::unique_ptr<Sprite> ( new BadAppleSprite( x, y, Direction::Horizontal::LEFT ) );
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
			return std::unique_ptr<Sprite> ( new LifesaverSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 44 ):
			return std::unique_ptr<Sprite> ( new QuadrapusSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 45 ):
			return std::unique_ptr<Sprite> ( new WaterdropSpawnerSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 46 ):
			return std::unique_ptr<Sprite> ( new FaucetSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 47 ):
			return std::unique_ptr<Sprite> ( new PipeEelSprite( x, y, Direction::Vertical::DOWN ) );
		break;
		case ( SPRITE_INDEX_START + 48 ):
			return std::unique_ptr<Sprite> ( new PipeEelSprite( x, y, Direction::Vertical::UP ) );
		break;
		case ( SPRITE_INDEX_START + 49 ):
			return std::move( CowpokerSprite::TallCowpokerSprite( x, y, i ) );
		break;
		case ( SPRITE_INDEX_START + 50 ):
			return std::move( CowpokerSprite::ShortCowpokerSprite( x, y, i ) );
		break;
		case ( SPRITE_INDEX_START + 51 ):
			return std::unique_ptr<Sprite> ( new LockedDoorSprite( x, y, i ) );
		break;
		case ( SPRITE_INDEX_START + 52 ):
			return std::unique_ptr<Sprite> ( new CannonSprite( x, y, Direction::Vertical::__NULL, i ) );
		break;
		case ( SPRITE_INDEX_START + 53 ):
			return std::unique_ptr<Sprite> ( new StrongerCowpokerSprite( x, y, i ) );
		break;
		case ( SPRITE_INDEX_START + 54 ):
			return std::unique_ptr<Sprite> ( new CannonSprite( x, y, Direction::Vertical::UP, i ) );
		break;
		case ( SPRITE_INDEX_START + 55 ):
			return std::unique_ptr<Sprite> ( new CannonSprite( x, y, Direction::Vertical::DOWN, i ) );
		break;
		case ( SPRITE_INDEX_START + 56 ):
			return std::unique_ptr<Sprite> ( new SpringSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 57 ):
			return std::unique_ptr<Sprite> ( new PolloSprite( x, y, Direction::Horizontal::LEFT, i, false ) );
		break;
		case ( SPRITE_INDEX_START + 58 ):
			return std::unique_ptr<Sprite> ( new PolloSprite( x, y, Direction::Horizontal::RIGHT, i, false ) );
		break;
		case ( SPRITE_INDEX_START + 59 ):
			return std::unique_ptr<Sprite> ( new PolloDelAireSprite( x, y, Direction::Horizontal::LEFT, Direction::Vertical::__NULL, i, false ) );
		break;
		case ( SPRITE_INDEX_START + 60 ):
			return std::unique_ptr<Sprite> ( new PolloDelAireSprite( x, y, Direction::Horizontal::RIGHT, Direction::Vertical::__NULL, i, false ) );
		break;
		case ( SPRITE_INDEX_START + 61 ):
			return std::unique_ptr<Sprite> ( new PolloDelAireVerticalSprite( x, y, Direction::Vertical::UP, i, false ) );
		break;
		case ( SPRITE_INDEX_START + 62 ):
			return std::unique_ptr<Sprite> ( new PolloDelAireVerticalSprite( x, y, Direction::Vertical::DOWN, i, false ) );
		break;
		case ( SPRITE_INDEX_START + 63 ):
			return std::unique_ptr<Sprite> ( new PolloDelAireStillSprite( x, y, i, false ) );
		break;
		case ( SPRITE_INDEX_START + 64 ):
			return std::unique_ptr<Sprite> ( new PolloDelAireSprite( x, y, Direction::Horizontal::LEFT, Direction::Vertical::UP, i, false ) );
		break;
		case ( SPRITE_INDEX_START + 65 ):
			return std::unique_ptr<Sprite> ( new PolloDelAireSprite( x, y, Direction::Horizontal::LEFT, Direction::Vertical::DOWN, i, false ) );
		break;
		case ( SPRITE_INDEX_START + 66 ):
			return std::unique_ptr<Sprite> ( new PolloDelAireSprite( x, y, Direction::Horizontal::RIGHT, Direction::Vertical::UP, i, false ) );
		break;
		case ( SPRITE_INDEX_START + 67 ):
			return std::unique_ptr<Sprite> ( new PolloDelAireSprite( x, y, Direction::Horizontal::RIGHT, Direction::Vertical::DOWN, i, false ) );
		break;
		case ( SPRITE_INDEX_START + 68 ):
			return std::unique_ptr<Sprite> ( new PolloDelAireCircleSprite( x, y, true, i, false ) );
		break;
		case ( SPRITE_INDEX_START + 69 ):
			return std::unique_ptr<Sprite> ( new PolloDelAireCircleSprite( x, y, false, i, false ) );
		break;
		case ( SPRITE_INDEX_START + 70 ):
			return std::unique_ptr<Sprite> ( new BadAppleSprite( x, y, Direction::Horizontal::RIGHT ) );
		break;
		case ( SPRITE_INDEX_START + 71 ):
			return std::unique_ptr<Sprite> ( new AngryTruckSprite( x, y, false ) );
		break;
		case ( SPRITE_INDEX_START + 72 ):
			return std::unique_ptr<Sprite> ( new ChoqueSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 73 ):
			return std::unique_ptr<Sprite> ( new HieroglyphPusherSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 74 ):
			return std::unique_ptr<Sprite> ( new UndergroundSubwaySprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 75 ):
			return std::unique_ptr<Sprite> ( new BatSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 76 ):
			return std::unique_ptr<Sprite> ( new BirdSprite( x, y, Direction::Horizontal::LEFT ) );
		break;
		case ( SPRITE_INDEX_START + 77 ):
			return std::unique_ptr<Sprite> ( new BirdSprite( x, y, Direction::Horizontal::RIGHT ) );
		break;
		case ( SPRITE_INDEX_START + 78 ):
			return std::unique_ptr<Sprite> ( new GhostSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 79 ):
			return std::unique_ptr<Sprite> ( new GemmySprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 80 ):
			return std::unique_ptr<Sprite> ( new FallingGemSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 81 ):
			return std::unique_ptr<Sprite> ( new DownhillGemSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 82 ):
			return std::unique_ptr<Sprite> ( new TallTombstoneSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 83 ):
			return std::unique_ptr<Sprite> ( new FallingFistSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 84 ):
			return std::unique_ptr<Sprite> ( new MoonSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 85 ):
			return std::unique_ptr<Sprite> ( ChangeLayerSprite::makeBackground( x ) );
		break;
		case ( SPRITE_INDEX_START + 86 ):
			return std::unique_ptr<Sprite> ( ChangeLayerSprite::makeForeground( x ) );
		break;
		case ( SPRITE_INDEX_START + 87 ):
			return std::unique_ptr<Sprite> ( ChangeLayerSprite::makeBackgroundDuo( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 88 ):
			return std::unique_ptr<Sprite> ( new RisingIceCubeSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 89 ):
			return std::unique_ptr<Sprite> ( new CarrotSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 90 ):
			return std::unique_ptr<Sprite> ( new OliveSpawnerSprite( x, y, Direction::Horizontal::RIGHT ) );
		break;
		case ( SPRITE_INDEX_START + 91 ):
			return std::unique_ptr<Sprite> ( new IceBoxRockSolidHackSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 92 ):
			return std::unique_ptr<Sprite> ( new OliveSpawnerSprite( x, y, Direction::Horizontal::LEFT ) );
		break;
		case ( SPRITE_INDEX_START + 93 ):
			return std::unique_ptr<Sprite> ( new SpawnIcicleSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 94 ):
			return std::unique_ptr<Sprite> ( new RisingIceCubeSprite( x, y, 4 ) );
		break;
		case ( SPRITE_INDEX_START + 95 ):
			return std::unique_ptr<Sprite> ( new TreasureChestSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 96 ):
			return std::unique_ptr<Sprite> ( new RandomTreasureChestSprite( x, y, events ) );
		break;
		case ( SPRITE_INDEX_START + 97 ):
			return std::unique_ptr<Sprite> ( new PelicanSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 98 ):
			return std::unique_ptr<Sprite> ( new CrabSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 99 ):
			return std::unique_ptr<Sprite> ( new DesertHawkSprite( x, y, Direction::Horizontal::LEFT ) );
		break;
		case ( SPRITE_INDEX_START + 100 ):
			return std::unique_ptr<Sprite> ( new SpiderSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 101 ):
			return std::unique_ptr<Sprite> ( new VolcanoMonsterSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 102 ):
			return std::unique_ptr<Sprite> ( new DesertHawkSprite( x, y, Direction::Horizontal::RIGHT ) );
		break;
		case ( SPRITE_INDEX_START + 103 ):
			return std::unique_ptr<Sprite> ( new SwitchBlockSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 104 ):
			return std::unique_ptr<Sprite> ( new MoveWaterSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 105 ):
			return std::unique_ptr<Sprite> ( new SpawnAnchorMissileSprite( x, y, SpawnAnchorMissileSprite::AMType::LEFT_AND_RIGHT ) );
		break;
		case ( SPRITE_INDEX_START + 106 ):
			return std::unique_ptr<Sprite> ( new SpawnAnchorMissileSprite( x, y, SpawnAnchorMissileSprite::AMType::UP ) );
		break;
		case ( SPRITE_INDEX_START + 107 ):
			return std::unique_ptr<Sprite> ( new SpawnAnchorMissileSprite( x, y, SpawnAnchorMissileSprite::AMType::DOWN ) );
		break;
		case ( SPRITE_INDEX_START + 108 ):
			return std::unique_ptr<Sprite> ( new CapnClearbeardSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 109 ):
			return std::unique_ptr<Sprite> ( new LavaPlatformSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 110 ):
			return std::unique_ptr<Sprite> ( new CircleBlockSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 111 ):
			return std::unique_ptr<Sprite> ( new WeightPlatformSprite( x, y, Direction::Horizontal::LEFT ) );
		break;
		case ( SPRITE_INDEX_START + 112 ):
			return std::unique_ptr<Sprite> ( new UrbanBirdSprite( x, y, hero_.get() ) );
		break;
		case ( SPRITE_INDEX_START + 113 ):
			return std::unique_ptr<Sprite> ( new GorillaSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 114 ):
			return std::unique_ptr<Sprite> ( new BarrelOGorillasSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 115 ):
			return std::unique_ptr<Sprite> ( new WeightPlatformSprite( x, y, Direction::Horizontal::RIGHT ) );
		break;
		case ( SPRITE_INDEX_START + 116 ):
			return std::unique_ptr<Sprite> ( new BubbleSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 117 ):
			return std::unique_ptr<Sprite> ( new FullHealBlockSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 118 ):
			return std::unique_ptr<Sprite> ( new MessageBlockSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 119 ):
			return std::unique_ptr<Sprite> ( new PolloSprite( x, y, Direction::Horizontal::LEFT, i, true ) );
		break;
		case ( SPRITE_INDEX_START + 120 ):
			return std::unique_ptr<Sprite> ( new PolloSprite( x, y, Direction::Horizontal::RIGHT, i, true ) );
		break;
		case ( SPRITE_INDEX_START + 121 ):
			return std::unique_ptr<Sprite> ( new PolloDelAireSprite( x, y, Direction::Horizontal::LEFT, Direction::Vertical::__NULL, i, true ) );
		break;
		case ( SPRITE_INDEX_START + 122 ):
			return std::unique_ptr<Sprite> ( new PolloDelAireSprite( x, y, Direction::Horizontal::RIGHT, Direction::Vertical::__NULL, i, true ) );
		break;
		case ( SPRITE_INDEX_START + 123 ):
			return std::unique_ptr<Sprite> ( new PolloDelAireVerticalSprite( x, y, Direction::Vertical::UP, i, true ) );
		break;
		case ( SPRITE_INDEX_START + 124 ):
			return std::unique_ptr<Sprite> ( new PolloDelAireVerticalSprite( x, y, Direction::Vertical::DOWN, i, true ) );
		break;
		case ( SPRITE_INDEX_START + 125 ):
			return std::unique_ptr<Sprite> ( new PolloDelAireStillSprite( x, y, i, true ) );
		break;
		case ( SPRITE_INDEX_START + 126 ):
			return std::unique_ptr<Sprite> ( new PolloDelAireSprite( x, y, Direction::Horizontal::LEFT, Direction::Vertical::UP, i, true ) );
		break;
		case ( SPRITE_INDEX_START + 127 ):
			return std::unique_ptr<Sprite> ( new PolloDelAireSprite( x, y, Direction::Horizontal::LEFT, Direction::Vertical::DOWN, i, true ) );
		break;
		case ( SPRITE_INDEX_START + 128 ):
			return std::unique_ptr<Sprite> ( new PolloDelAireSprite( x, y, Direction::Horizontal::RIGHT, Direction::Vertical::UP, i, true ) );
		break;
		case ( SPRITE_INDEX_START + 129 ):
			return std::unique_ptr<Sprite> ( new PolloDelAireSprite( x, y, Direction::Horizontal::RIGHT, Direction::Vertical::DOWN, i, true ) );
		break;
		case ( SPRITE_INDEX_START + 130 ):
			return std::unique_ptr<Sprite> ( new PolloDelAireCircleSprite( x, y, true, i, true ) );
		break;
		case ( SPRITE_INDEX_START + 131 ):
			return std::unique_ptr<Sprite> ( new PolloDelAireCircleSprite( x, y, false, i, true ) );
		break;
		case ( SPRITE_INDEX_START + 132 ):
			return std::unique_ptr<Sprite> ( new TruckPlatformSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 133 ):
			return std::unique_ptr<Sprite> ( new RisingPackageSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 134 ):
			return std::unique_ptr<Sprite> ( new AngryTruckGeneratorSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 135 ):
			return std::unique_ptr<Sprite> ( new FallingSnowBoulderSpawnSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 136 ):
			return std::unique_ptr<Sprite> ( new PufferbeeSquareSprite( x, y, Direction::Simple::DOWN ) );
		break;
		case ( SPRITE_INDEX_START + 137 ):
			return std::unique_ptr<Sprite> ( new PufferbeeSquareSprite( x, y, Direction::Simple::UP ) );
		break;
		case ( SPRITE_INDEX_START + 138 ):
			return std::unique_ptr<Sprite> ( new AntSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 139 ):
			return std::unique_ptr<Sprite> ( new PiBlockSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 140 ):
			return std::unique_ptr<Sprite> ( new ShmupShooterSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 141 ):
			return std::unique_ptr<Sprite> ( new ChangeCameraAutoscrollSprite( x, y, Direction::Simple::DOWN ) );
		break;
		case ( SPRITE_INDEX_START + 142 ):
			return std::unique_ptr<Sprite> ( new ChangeCameraAutoscrollSprite( x, y, Direction::Simple::LEFT ) );
		break;
		case ( SPRITE_INDEX_START + 143 ):
			return std::unique_ptr<Sprite> ( new ChangeCameraAutoscrollSprite( x, y, Direction::Simple::RIGHT ) );
		break;
		case ( SPRITE_INDEX_START + 144 ):
			return std::unique_ptr<Sprite> ( new ChangeCameraAutoscrollSprite( x, y, Direction::Simple::UP ) );
		break;
		case ( SPRITE_INDEX_START + 145 ):
			return std::unique_ptr<Sprite> ( new ShmupEnemySprite() );
		break;
		case ( SPRITE_INDEX_START + 146 ):
			return std::unique_ptr<Sprite> ( new DungeonSwitchSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 147 ):
			return std::unique_ptr<Sprite> ( new DungeonEnemySprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 148 ):
			return std::unique_ptr<Sprite> ( new TopDownMoveableBlockSprite( x, y ) );
		break;
		default:
			throw mezun::InvalidSprite( type );
		break;
	}
};

void SpriteSystem::spawn( std::unique_ptr<Sprite> sprite )
{
	sprites_.emplace_back( sprite.release() );
};

void SpriteSystem::spawnCactooieSpine( int x, int y, Direction::Horizontal direction )
{
	sprites_.emplace_back( new CactooieSpineSprite( x, y, direction ) );
};

void SpriteSystem::spawnSnowball( int x, int y, Direction::Horizontal direction )
{
	sprites_.emplace_back( new SnowballSprite( x, y, direction ) );
};

void SpriteSystem::spawnWaterdrop( int x, int y )
{
	sprites_.emplace_back( new WaterdropSprite( x, y ) );
};

void SpriteSystem::spawnEnemyBullet( int x, int y, Direction::Simple direction )
{
	sprites_.emplace_back( new BulletSprite( x, y, direction, false ) );
};

void SpriteSystem::spawnHeroBullet( int x, int y, Direction::Simple direction )
{
	sprites_.emplace_back( new BulletSprite( x, y, direction, true ) );
};

void SpriteSystem::spawnShmupBullet( int x, int y, double dy, double dx )
{
	sprites_.emplace_back( new ShmupBulletSprite( x, y, dy, dx ) );
};

void SpriteSystem::spawnOlive( int x, int y, Direction::Horizontal start_dir )
{
	sprites_.emplace_back( new OliveSprite( x, y, start_dir ) );
};

void SpriteSystem::spawnIcicle( int x, int y )
{
	sprites_.emplace_back( new IcicleSprite( x, y ) );
};

void SpriteSystem::heroOpenTreasureChest()
{
	hero_.reset( new PlayerOpenChestSprite( hero_->xPixels(), hero_->yPixels(), hero_->direction_x_ ) );
};

void SpriteSystem::spritesFromMap( const Map& lvmap, EventSystem& events )
{
	for ( int i = 0; i < lvmap.spritesSize(); ++i )
	{
		const int x = Unit::BlocksToSubPixels( lvmap.mapX( i ) );
		const int y = Unit::BlocksToSubPixels( lvmap.mapY( i ) );
		const int type = lvmap.sprite( i ) - 1;

		if ( type != -1 )
		{
			std::unique_ptr<Sprite> new_sprite = std::move( spriteType( type, x, y, i, lvmap, events ) );
			sprites_.emplace_back( std::move( new_sprite ) );
		}
	}
};

void SpriteSystem::interact( BlockSystem& blocks, Level& level, EventSystem& events, Camera& camera, Health& health )
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
					blocks.interact( *sprites_.at( i ), level, events, camera, health, *this );
				}
			}
		}
	}

	if ( hero_->interactsWithBlocks() )
	{
		blocks.interact( *hero_, level, events, camera, health, *this );
	}
};

void SpriteSystem::reset( const Level& level, EventSystem& events )
{
	Sprite::resistance_x_ = level.currentMap().wind_strength_;

	if ( level.currentMap().moon_gravity_ )
	{
		Sprite::moonGravityOn();
	}
	else
	{
		Sprite::moonGravityOff();
	}

	if ( level.currentMap().slippery_ )
	{
		Sprite::traction_ = Sprite::TRACTION_ICY;
	}
	else
	{
		Sprite::traction_ = Sprite::TRACTION_NORMAL;
	}

	switch( level.currentMap().hero_type_ )
	{
		case ( HeroType::NORMAL ):
			hero_.reset( new PlayerSprite( level.entranceX(), level.entranceY() + 10 ) );
		break;
		case ( HeroType::SHOOTER ):
			hero_.reset( new ShooterPlayerSprite( level.entranceX(), level.entranceY() ) );
		break;
		case ( HeroType::OVERWORLD ):
			hero_.reset( new MazePlayerSprite( level.entranceX(), level.entranceY() ) );
		break;
		case ( HeroType::CART ):
			hero_.reset( new PlayerCartSprite( level.entranceX(), level.entranceY() ) );
		break;
		case ( HeroType::FLUTTERING ):
			hero_.reset( new PlayerSpriteFluttering( level.entranceX(), level.entranceY() ) );
		break;
		case ( HeroType::SPACESHIP ):
			hero_.reset( new PlayerSpaceshipSprite( level.entranceX(), level.entranceY() ) );
		break;
		case ( HeroType::CAR ):
			hero_.reset( new PlayerCarSprite( level.entranceX(), level.entranceY() ) );
		break;
		case ( HeroType::TOP_DOWN ):
			hero_.reset( new TopDownPlayerSprite( level.entranceX(), level.entranceY() ) );
		break;
	}

	clearSprites();
	spritesFromMap( level.currentMap(), events );
};

void SpriteSystem::clearSprites()
{
	sprites_.clear();
};

void SpriteSystem::destroySprite( int n, Map& lvmap )
{
	if ( sprites_.at( n )->hasType( Sprite::SpriteType::DEATH_COUNT ) )
	{
		lvmap.deleteSprite( sprites_.at( n )->map_id_ );
		++permanently_killed_enemies_;
	}

	if ( n < sprites_.size() )
	{
		// Save time by just replacing dying sprite with last sprite & just
		// popping off last sprite ( which is null now, anyway ),
		// saving us from having to slowly shift every sprite back
		// with vector's native erase method.
		sprites_[ n ].reset( sprites_[ sprites_.size() - 1 ].release() );
		sprites_.pop_back();
	}
};

void SpriteSystem::update( Camera& camera, Map& lvmap, EventSystem& events, BlockSystem& blocks, Health& health )
{
	for ( int i = 0; i < sprites_.size(); ++i )
	{
		if ( sprites_.at( i )->despawnWhenDead() )
		{
			if ( sprites_.at( i )->deathFinished() )
			{
				destroySprite( i, lvmap );
				continue; // SPRITE IS DEAD--DO NOT TRY UPDATING.
			}
		}

		switch( sprites_.at( i )->cameraMovement() )
		{
			case ( Sprite::CameraMovement::PAUSE_OFFSCREEN ):
				if ( camera.onscreen( sprites_.at( i )->hitBox(), OFFSCREEN_PADDING ) )
				{
					sprites_.at( i )->update( camera, lvmap, events, *this, blocks, health );
				}
			break;

			case ( Sprite::CameraMovement::PERMANENT ):
				sprites_.at( i )->update( camera, lvmap, events, *this, blocks, health );
			break;

			case ( Sprite::CameraMovement::RESET_INSTANTLY_OFFSCREEN ):
				if ( camera.onscreen( sprites_.at( i )->hitBox(), OFFSCREEN_PADDING ) )
				{
					sprites_.at( i )->update( camera, lvmap, events, *this, blocks, health );
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
					sprites_.at( i )->update( camera, lvmap, events, *this, blocks, health );
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
					sprites_.at( i )->update( camera, lvmap, events, *this, blocks, health );
				}
				else
				{
					destroySprite( i, lvmap );
				}
			break;
		}
	}

	if ( health.hp() <= 0 )
	{
		hero_->kill();
	}

	hero_->update( camera, lvmap, events, *this, blocks, health );

	if ( hero_->deathFinished() )
	{
		events.fail();
	}
};

void SpriteSystem::spriteInteraction( Camera& camera, BlockSystem& blocks, Map& lvmap, Health& health, EventSystem& events )
{
	for ( int i = 0; i < sprites_.size(); ++i )
	{
		if ( sprites_.at( i ) != nullptr )
		{
			if ( camera.onscreen( sprites_.at( i )->hitBox(), OFFSCREEN_PADDING ) || sprites_[ i ]->cameraMovement() == Sprite::CameraMovement::PERMANENT )
			{
				if ( sprites_.at( i )->interactsWithSprites() && hero_->interactsWithSprites() )
				{
					sprites_.at( i )->interact( *hero_, blocks, *this, lvmap, health, events );
					hero_->interact( *sprites_.at( i ), blocks, *this, lvmap, health, events );
				}

				for ( int j = 0; j < sprites_.size(); ++j )
				{
					if ( sprites_[ j ] != nullptr )
						if ( i != j )
							if ( camera.onscreen( sprites_[ j ]->hitBox(), OFFSCREEN_PADDING ) || sprites_[ j ]->cameraMovement() == Sprite::CameraMovement::PERMANENT )
								if ( sprites_.at( i )->interactsWithSprites() && sprites_[ j ]->interactsWithSprites() )
									sprites_.at( i )->interact( *sprites_[ j ], blocks, *this, lvmap, health, events );
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

	hero_->render( camera, priority );
};

Sprite& SpriteSystem::hero()
{
	return *hero_;
};

const Sprite& SpriteSystem::hero() const
{
	return *hero_;
};

SpriteSystem::HeroType SpriteSystem::heroType( const std::string& property )
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
	else if ( property.compare( "SHOOTER" ) == 0 )
	{
		return HeroType::SHOOTER;
	}
	else if ( property.compare( "SPACESHIP" ) == 0 )
	{
		return HeroType::SPACESHIP;
	}
	else if ( property.compare( "CAR" ) == 0 )
	{
		return HeroType::CAR;
	}
	else if ( property.compare( "TOP_DOWN" ) == 0 )
	{
		return HeroType::TOP_DOWN;
	}

	return HeroType::NORMAL;
};

void SpriteSystem::interactWithMap( Map& lvmap, Camera& camera, Health& health )
{
	lvmap.interact( *hero_, camera, health );

	for ( auto& s : sprites_ )
	{
		if ( s != nullptr )
		{
			if ( s->interactsWithBlocks() )
			{
				if
				(
					camera.onscreen( s->hitBox(), OFFSCREEN_PADDING ) ||
					s->hasCameraMovement( Sprite::CameraMovement::PERMANENT )
				)
				{
					lvmap.interact( *s, camera, health );
				}
			}
		}
	}
};

int SpriteSystem::permanentlyKilledEnemies() const
{
	return permanently_killed_enemies_;
};

int SpriteSystem::numberOfSprites() const
{
	return sprites_.size();
};
