#include "angry_truck_sprite.hpp"
#include "angry_truck_generator_sprite.hpp"
#include "ant_sprite.hpp"
#include "attic_monster_sprite.hpp"
#include "bad_apple_sprite.hpp"
#include "barrel_o_gorillas_sprite.hpp"
#include "bat_sprite.hpp"
#include "bird_sprite.hpp"
#include "block_system.hpp"
#include "bouncy_cloud_block_sprite.hpp"
#include "bouncing_ball_sprite.hpp"
#include "bouncing_spike_fruit_sprite.hpp"
#include "bridge_monster_sprite.hpp"
#include "bubble_sprite.hpp"
#include "bullet_sprite.hpp"
#include "buzz_saw_sprite.hpp"
#include "cactooie_sprite.hpp"
#include "camera.hpp"
#include "change_camera_autoscroll_sprite.hpp"
#include "cannon_sprite.hpp"
#include "capn_clearbeard_sprite.hpp"
#include "carrot_sprite.hpp"
#include "chamsby_sprite.hpp"
#include "change_layer_sprite.hpp"
#include "choque_sprite.hpp"
#include "circle_block_sprite.hpp"
#include "clock_hand_sprite.hpp"
#include "cloud_block_sprite.hpp"
#include "cloud_monster_sprite.hpp"
#include "cloud_platform_sprite.hpp"
#include "cockroach_sprite.hpp"
#include "cowpoker_sprite.hpp"
#include "crab_sprite.hpp"
#include "crane_crate_sprite.hpp"
#include <cstdlib>
#include "desert_hawk_sprite.hpp"
#include "devil_sprite.hpp"
#include "direction.hpp"
#include "doom_door_sprite.hpp"
#include "doom_enemy_sprite.hpp"
#include "downhill_gem_sprite.hpp"
#include "dragonfly_sprite.hpp"
#include "dungeon_enemy_spreader_sprite.hpp"
#include "dungeon_enemy_sprite.hpp"
#include "dungeon_enemy_walls_sprite.hpp"
#include "dungeon_healer_sprite.hpp"
#include "dungeon_switch_sprite.hpp"
#include "enemy_cart_sprite.hpp"
#include "electric_wall_sprite.hpp"
#include "event_system.hpp"
#include "falling_snow_boulder_spawn_sprite.hpp"
#include "falling_spike_sprite.hpp"
#include "faucet_sprite.hpp"
#include "falling_bough_sprite.hpp"
#include "falling_dollar_generator_sprite.hpp"
#include "falling_fist_sprite.hpp"
#include "falling_gem_sprite.hpp"
#include "falling_star_generator_sprite.hpp"
#include "firebar_sprite.hpp"
#include "fishstick_sprite.hpp"
#include "flashlight_player_sprite.hpp"
#include "flying_grave_monster_sprite.hpp"
#include "frog_sprite.hpp"
#include "frozen_meat_sprite.hpp"
#include "full_heal_block_sprite.hpp"
#include "gemmy_sprite.hpp"
#include "ghost_coffin_sprite.hpp"
#include "ghost_sprite.hpp"
#include "gorilla_sprite.hpp"
#include "guard_spike_sprite.hpp"
#include "guard_sprite.hpp"
#include "harpoon_sprite.hpp"
#include "health.hpp"
#include "heat_beam_sprite.hpp"
#include "hero_renderable.hpp"
#include "hieroglyph_pusher_sprite.hpp"
#include "hook_sprite.hpp"
#include "hopping_zombie_sprite.hpp"
#include "horizontal_pike_sprite.hpp"
#include "horizontal_spark_sprite.hpp"
#include "hydrant_sprite.hpp"
#include "ice_box_rock_solid_hack_sprite.hpp"
#include "iceblock_sprite.hpp"
#include "icecube_sprite.hpp"
#include "icicle_sprite.hpp"
#include "input_component_sequence.hpp"
#include "iron_wall_sprite.hpp"
#include "jumping_crock_sprite.hpp"
#include "level.hpp"
#include "level_state.hpp"
#include "lava_platform_sprite.hpp"
#include "level_state.hpp"
#include "lifesaver_sprite.hpp"
#include "lightning_sprite.hpp"
#include "lil_pipe_monster_sprite.hpp"
#include "light_switch_sprite.hpp"
#include "locked_door_sprite.hpp"
#include "log_rafts_sprite.hpp"
#include "mansion_door_sprite.hpp"
#include "mansion_ghost_sprite.hpp"
#include "map.hpp"
#include "maze_player_sprite.hpp"
#include "maze_chaser_sprite.hpp"
#include "message_block_sprite.hpp"
#include "mezun_exceptions.hpp"
#include "moon_sprite.hpp"
#include "moveable_block_sprite.hpp"
#include "moving_space_stone_sprite.hpp"
#include "move_water_sprite.hpp"
#include "octopig_sprite.hpp"
#include "olive_spawner_sprite.hpp"
#include "olive_sprite.hpp"
#include "pelican_sprite.hpp"
#include "pedestrian_generator_sprite.hpp"
#include "pendulum_sprite.hpp"
#include "penguin_sprite.hpp"
#include "pike_sprite.hpp"
#include "pipe_eel_sprite.hpp"
#include "platform_sprite.hpp"
#include "player_car_sprite.hpp"
#include "player_cart_sprite.hpp"
#include "player_doom_sprite.hpp"
#include "player_graphics.hpp"
#include "player_open_chest_sprite.hpp"
#include "player_plane_sprite.hpp"
#include "player_spaceship_sprite.hpp"
#include "player_sprite_fluttering.hpp"
#include "pi_block_sprite.hpp"
#include "pollo_sprite.hpp"
#include "pollo_del_aire_sprite.hpp"
#include "pollo_del_aire_circle_sprite.hpp"
#include "pollo_del_aire_still_sprite.hpp"
#include "pollo_del_aire_vertical_sprite.hpp"
#include "pollo_picante_sprite.hpp"
#include "pufferbee_sprite.hpp"
#include "pufferbee_square_sprite.hpp"
#include "quadrapus_sprite.hpp"
#include "racer_sprite.hpp"
#include "raft_sprite.hpp"
#include "random_treasure_chest_sprite.hpp"
#include "rising_package_sprite.hpp"
#include "rising_ice_cube_sprite.hpp"
#include "rising_platform_sprite.hpp"
#include "rolling_ball_sprite.hpp"
#include "rope_sprite.hpp"
#include "rotating_block_sprite.hpp"
#include "rug_monster_sprite.hpp"
#include "saw_sprite.hpp"
#include "sewer_monster_sprite.hpp"
#include "shmup_enemy_sprite.hpp"
#include "shmup_shooter_sprite.hpp"
#include "shooter_player_sprite.hpp"
#include "shroud_sprite.hpp"
#include "sillyfish_sprite.hpp"
#include "snowboulder_sprite.hpp"
#include "snowman_sprite.hpp"
#include "space_lizard_sprite.hpp"
#include "spark_sprite.hpp"
#include "spawn_anchor_missile_sprite.hpp"
#include "spawn_fist_missile_sprite.hpp"
#include "spawn_icicle_sprite.hpp"
#include "spider_sprite.hpp"
#include "spike_egg_sprite.hpp"
#include "spiky_fruit_sprite.hpp"
#include "spring_enemy_sprite.hpp"
#include "spring_sprite.hpp"
#include "sprite_component_circle.hpp"
#include "sprite_component_right_and_left.hpp"
#include "sprite_component_up_and_down.hpp"
#include "sprite_renderable.hpp"
#include "sprite_system.hpp"
#include "statue_laser_sprite.hpp"
#include "stronger_cowpoker_sprite.hpp"
#include "swamp_monster_sprite.hpp"
#include "swamp_pole_sprite.hpp"
#include "switch_block_sprite.hpp"
#include "tall_tombstone_sprite.hpp"
#include "target_sprite.hpp"
#include "thrower_sprite.hpp"
#include "top_down_player_sprite.hpp"
#include "top_down_moveable_block_sprite.hpp"
#include "toy_soldier_sprite.hpp"
#include "treasure_chest_sprite.hpp"
#include "truck_platform_sprite.hpp"
#include "underground_subway_sprite.hpp"
#include "urban_bird_sprite.hpp"
#include "vacuum_sprite.hpp"
#include "vanishing_forest_ground_sprite.hpp"
#include "vertical_pike_sprite.hpp"
#include "volcano_monster_sprite.hpp"
#include "wall_crawler_sprite.hpp"
#include "waterdrop_spawner_sprite.hpp"
#include "water_spout_sprite.hpp"
#include "weight_platform_sprite.hpp"
#include "window_monster_sprite.hpp"
#include "zombie_generator_sprite.hpp"

static constexpr int SPRITES_LIMIT = 50;
static constexpr int OFFSCREEN_PADDING = Unit::BlocksToPixels( 2 );
static constexpr int SPRITE_INDEX_START = 400;

static bool permanentOrOnScreen( const Camera& camera, const Sprite& sprite )
{
	return sprite.cameraMovement() == Sprite::CameraMovement::PERMANENT ||
		camera.onscreen( sprite.hitBox(), OFFSCREEN_PADDING );
};

SpriteSystem::SpriteSystem( LevelState& level_state )
:
	id_ ( 0 ),
	permanently_killed_enemies_ ( 0 ),
	level_state_ ( level_state ),
	hero_ (),
	sprites_ ()
{
	// Minimize chances o' sprite # going past space
	// & forcing slow vector relocation.
	sprites_.reserve( SPRITES_LIMIT );
};

SpriteSystem::~SpriteSystem() {};

std::unique_ptr<Sprite> SpriteSystem::spriteType( int type, int x, int y, int i, LevelState& level_state )
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
			return std::unique_ptr<Sprite> ( new SpikyFruitSprite( x, y, level_state.currentMap() ) );
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
			return std::unique_ptr<Sprite> ( new SpawnFistMissileSprite( x, y ) );
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
			return nullptr;
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
			return std::unique_ptr<Sprite> ( generateSnowmanSprite( x, y ) );
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
			return std::unique_ptr<Sprite> ( new AngryTruckSprite( x, y, false, Direction::Horizontal::LEFT ) );
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
			return std::unique_ptr<Sprite> ( new GhostSprite( x, y, Direction::Horizontal::RIGHT ) );
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
			return std::unique_ptr<Sprite> ( new RandomTreasureChestSprite( x, y, level_state.events() ) );
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
			return std::unique_ptr<Sprite> ( new DungeonEnemySprite( x, y, Unit::Layer::SPRITES_1 ) );
		break;
		case ( SPRITE_INDEX_START + 148 ):
			return std::unique_ptr<Sprite> ( new TopDownMoveableBlockSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 149 ):
			return std::unique_ptr<Sprite> ( new DungeonEnemySprite( x, y, Unit::Layer::SPRITES_2
			 ) );
		break;
		case ( SPRITE_INDEX_START + 150 ):
			return std::unique_ptr<Sprite> ( new FirebarSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 151 ):
			return std::unique_ptr<Sprite> ( new SpringEnemySprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 152 ):
			return std::unique_ptr<Sprite> ( new DungeonEnemyWallsSprite( x, y, Direction::Simple::LEFT ) );
		break;
		case ( SPRITE_INDEX_START + 153 ):
			return std::unique_ptr<Sprite> ( new DungeonEnemySprite( x, y, Unit::Layer::SPRITES_1, true ) );
		break;
		case ( SPRITE_INDEX_START + 154 ):
			return std::unique_ptr<Sprite> ( new BouncingSpikeFruitSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 155 ):
			return std::unique_ptr<Sprite> ( new RopeSprite( x, y, 16, 34, 1000 ) );
		break;
		case ( SPRITE_INDEX_START + 156 ):
			return std::unique_ptr<Sprite> ( new CraneCrateSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 157 ):
			return std::unique_ptr<Sprite> ( new HookSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 158 ):
			return std::unique_ptr<Sprite> ( new DoomEnemySprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 159 ):
			return std::unique_ptr<Sprite> ( new DoomDoorSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 160 ):
			return std::unique_ptr<Sprite> ( new IronWallSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 161 ):
			return std::unique_ptr<Sprite> ( new RaftSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 162 ):
			return std::unique_ptr<Sprite> ( new SwampMonsterSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 163 ):
			return std::unique_ptr<Sprite> ( new LifesaverSprite( x, y, true ) );
		break;
		case ( SPRITE_INDEX_START + 164 ):
			return std::unique_ptr<Sprite> ( generateOctopigSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 165 ):
			return std::unique_ptr<Sprite> ( new ChamsbySprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 166 ):
			return std::unique_ptr<Sprite> ( new PikeSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 167 ):
			return std::unique_ptr<Sprite> ( new WindowMonsterSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 168 ):
			return std::unique_ptr<Sprite> ( new HarpoonSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 169 ):
			return std::unique_ptr<Sprite> ( new WaterSpoutSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 170 ):
			return std::unique_ptr<Sprite> ( new SpaceLizardSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 171 ):
			return std::unique_ptr<Sprite> ( new SparkSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 172 ):
			return std::unique_ptr<Sprite> ( new DungeonEnemySpreaderSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 173 ):
			return std::unique_ptr<Sprite> ( new DungeonEnemyWallsSprite( x, y, Direction::Simple::UP ) );
		break;
		case ( SPRITE_INDEX_START + 174 ):
			return std::unique_ptr<Sprite> ( new DungeonHealerSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 175 ):
			return std::unique_ptr<Sprite> ( new GuardSpikeSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 176 ):
			return std::unique_ptr<Sprite> ( new MansionGhostSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 177 ):
			return std::unique_ptr<Sprite> ( new GhostCoffinSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 178 ):
			return std::unique_ptr<Sprite> ( new MansionDoorSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 179 ):
			return std::unique_ptr<Sprite> ( new RugMonsterSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 180 ):
			return std::unique_ptr<Sprite> ( new PedestrianGeneratorSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 181 ):
			return std::unique_ptr<Sprite> ( new BridgeMonsterSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 182 ):
			return std::unique_ptr<Sprite> ( new FrogSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 183 ):
			return std::unique_ptr<Sprite> ( new SwampPoleSprite( x, y, 1 ) );
		break;
		case ( SPRITE_INDEX_START + 184 ):
			return std::unique_ptr<Sprite> ( new SwampPoleSprite( x, y, 2 ) );
		break;
		case ( SPRITE_INDEX_START + 185 ):
			return std::unique_ptr<Sprite> ( new SwampPoleSprite( x, y, 3 ) );
		break;
		case ( SPRITE_INDEX_START + 186 ):
			return std::unique_ptr<Sprite> ( new SwampPoleSprite( x, y, 4 ) );
		break;
		case ( SPRITE_INDEX_START + 187 ):
			return std::unique_ptr<Sprite> ( new SwampPoleSprite( x, y, 5 ) );
		break;
		case ( SPRITE_INDEX_START + 188 ):
			return std::unique_ptr<Sprite> ( new SwampPoleSprite( x, y, 6 ) );
		break;
		case ( SPRITE_INDEX_START + 189 ):
			return std::unique_ptr<Sprite> ( new SwampPoleSprite( x, y, 7 ) );
		break;
		case ( SPRITE_INDEX_START + 190 ):
			return std::unique_ptr<Sprite> ( new MoveableBlockSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 191 ):
			return std::unique_ptr<Sprite> ( new WallCrawlerSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 192 ):
			return std::unique_ptr<Sprite> ( new LightSwitchSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 193 ):
			return std::unique_ptr<Sprite> ( new FallingSpikeSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 194 ):
			return std::unique_ptr<Sprite> ( new MovingSpaceStoneSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 195 ):
			return std::unique_ptr<Sprite> ( new ElectricWallSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 196 ):
			return std::unique_ptr<Sprite> ( new PolloPicanteSprite( x, y, Direction::Horizontal::RIGHT ) );
		break;
		case ( SPRITE_INDEX_START + 197 ):
			return std::unique_ptr<Sprite> ( new PolloPicanteSprite( x, y, Direction::Horizontal::LEFT ) );
		break;
		case ( SPRITE_INDEX_START + 198 ):
			return std::unique_ptr<Sprite> ( new LogRaftsSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 199 ):
			return std::unique_ptr<Sprite> ( new ThrowerSprite( x, y, Direction::Horizontal::LEFT ) );
		break;
		case ( SPRITE_INDEX_START + 200 ):
			return std::unique_ptr<Sprite> ( new ThrowerSprite( x, y, Direction::Horizontal::RIGHT ) );
		break;
		case ( SPRITE_INDEX_START + 201 ):
			return std::unique_ptr<Sprite> ( new TargetSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 202 ):
			return std::unique_ptr<Sprite> ( new DragonflySprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 203 ):
			return std::unique_ptr<Sprite> ( new DragonflySprite( x, y, true ) );
		break;
		case ( SPRITE_INDEX_START + 204 ):
			return std::unique_ptr<Sprite> ( new FrozenMeatSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 205 ):
			return std::unique_ptr<Sprite> ( new HorizontalSparkSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 206 ):
			return std::unique_ptr<Sprite> ( new StatueLaserSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 207 ):
			return std::unique_ptr<Sprite> ( new HorizontalPikeSprite( x, y, Direction::Horizontal::RIGHT ) );
		break;
		case ( SPRITE_INDEX_START + 208 ):
			return std::unique_ptr<Sprite> ( new HorizontalPikeSprite( x, y, Direction::Horizontal::LEFT ) );
		break;
		case ( SPRITE_INDEX_START + 209 ):
			return std::unique_ptr<Sprite> ( new VerticalPikeSprite( x, y, Direction::Vertical::UP ) );
		break;
		case ( SPRITE_INDEX_START + 210 ):
			return std::unique_ptr<Sprite> ( new VerticalPikeSprite( x, y, Direction::Vertical::DOWN ) );
		break;
		case ( SPRITE_INDEX_START + 211 ):
			return std::unique_ptr<Sprite> ( new FallingDollarGeneratorSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 212 ):
			return std::unique_ptr<Sprite> ( new AngryTruckSprite( x, y, false, Direction::Horizontal::RIGHT ) );
		break;
		case ( SPRITE_INDEX_START + 213 ):
			return std::unique_ptr<Sprite> ( new JumpingCrockSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 214 ):
			return std::unique_ptr<Sprite> ( new VacuumSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 215 ):
			return std::unique_ptr<Sprite> ( new BouncingBallSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 216 ):
			return std::unique_ptr<Sprite> ( new ToySoldierSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 217 ):
			return std::unique_ptr<Sprite> ( new FallingStarGeneratorSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 218 ):
			return std::unique_ptr<Sprite> ( new AtticMonsterSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 219 ):
			return std::unique_ptr<Sprite> ( new CockroachSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 220 ):
			return std::unique_ptr<Sprite> ( new ZombieGeneratorSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 221 ):
			return std::unique_ptr<Sprite> ( new HoppingZombieSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 222 ):
			return std::unique_ptr<Sprite> ( new GhostSprite( x, y, Direction::Horizontal::LEFT, 3 ) );
		break;
		case ( SPRITE_INDEX_START + 223 ):
			return std::unique_ptr<Sprite> ( new VanishingForestGroundSprite( x, y, 5 ) );
		break;
		case ( SPRITE_INDEX_START + 224 ):
			return std::unique_ptr<Sprite> ( new DevilSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 225 ):
			return std::unique_ptr<Sprite> ( new PendulumSprite( x, y ) );
		break;
		case ( SPRITE_INDEX_START + 226 ):
			return std::unique_ptr<Sprite> ( new ClockHandSprite( x, y ) );
		break;
		default:
			throw mezun::InvalidSprite( type );
		break;
	}
};

void SpriteSystem::spawn( std::unique_ptr<Sprite>&& sprite )
{
	if ( sprites_.size() < SPRITES_LIMIT - 1 )
	{
		addSprite( std::move( sprite ) );
	}
};

void SpriteSystem::addSprite( std::unique_ptr<Sprite>&& sprite )
{
	const int id = id_;
	sprites_map_.insert( std::pair<int, Sprite*> ( id, sprite.get() ) );
	sprite->system_id_ = id;
	sprite->renderable_id_ = level_state_.addRenderable( std::unique_ptr<SpriteRenderable> ( new SpriteRenderable( id ) ), sprite->layer_ );
	sprites_.emplace_back( std::move( sprite ) );
	++id_;
};

void SpriteSystem::spawnEnemyBullet( int x, int y, Direction::Simple direction )
{
	spawn( std::unique_ptr<BulletSprite> ( new BulletSprite( x, y, direction, false ) ) );
};

void SpriteSystem::spawnHeroBullet( int x, int y, Direction::Simple direction )
{
	spawn( std::unique_ptr<BulletSprite> ( new BulletSprite( x, y, direction, true ) ) );
};

void SpriteSystem::heroOpenTreasureChest()
{
	hero_.reset( new PlayerOpenChestSprite( hero_->xPixels(), hero_->yPixels(), hero_->direction_x_ ) );
};

void SpriteSystem::spritesFromMap( LevelState& level_state )
{
	const Map& lvmap = level_state.currentMap();
	for ( int i = 0; i < lvmap.spritesSize(); ++i )
	{
		if ( sprites_.size() < SPRITES_LIMIT - 1 )
		{
			const int x = Unit::BlocksToSubPixels( lvmap.mapX( i ) );
			const int y = Unit::BlocksToSubPixels( lvmap.mapY( i ) );
			const int type = lvmap.sprite( i ) - 1;

			if ( type != -1 )
			{
				std::unique_ptr<Sprite> new_sprite = std::move( spriteType( type, x, y, i, level_state ) );
				addSprite( std::move( new_sprite ) );
			}
		}
	}
};

void SpriteSystem::interact( LevelState& level_state )
{
	BlockSystem& blocks = level_state.blocks();
	for ( auto i = 0; i < sprites_.size(); ++i )
	{
		if ( sprites_[ i ] != nullptr )
		{
			if ( sprites_[ i ]->interactsWithBlocks() )
			{
				if
				(
					sprites_[ i ]->hasCameraMovement( Sprite::CameraMovement::PERMANENT ) ||
					level_state.camera().onscreen( sprites_[ i ]->hitBox(), OFFSCREEN_PADDING )
				)
				{
					blocks.interact( *sprites_[ i ], level_state );
				}
			}
		}
	}

	if ( hero_->interactsWithBlocks() )
	{
		blocks.interact( *hero_, level_state );
	}
};

void SpriteSystem::reset( LevelState& level_state )
{
	resetInternal( level_state, false );
};

void SpriteSystem::resetTrainer( LevelState& level_state )
{
	resetInternal( level_state, true );
};

void SpriteSystem::resetInternal( LevelState& level_state, bool trainer )
{
	const Map& lvmap = level_state.currentMap();
	const Level& level = level_state.level();

	Sprite::resistance_x_ = lvmap.wind_strength_;

	if ( lvmap.slippery_ )
	{
		Sprite::traction_ = Sprite::TRACTION_ICY;
	}
	else
	{
		Sprite::traction_ = Sprite::TRACTION_NORMAL;
	}

	Sprite::setGravity( lvmap.gravity_ );

	if ( hero_ == nullptr )
	{
		switch( lvmap.hero_type_ )
		{
			case ( HeroType::NORMAL ):
				hero_.reset
				(
					new PlayerSprite
					(
						level.entranceX(),
						level.entranceY() + 10,
						PlayerSprite::DEFAULT_JUMP_START_SPEED,
						PlayerSprite::DEFAULT_JUMP_TOP_SPEED,
						( trainer ) ? std::unique_ptr<InputComponent> ( new InputComponentSequence( "trainer-" + level.getCodeName(), true ) ) : std::unique_ptr<InputComponent> ( new InputComponentPlayer() )
					)
				);
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
			case ( HeroType::DOOM ):
				hero_.reset( new PlayerDoomSprite( level.entranceX(), level.entranceY() ) );
			break;
			case ( HeroType::PLANE ):
				hero_.reset( new PlayerPlaneSprite( level.entranceX(), level.entranceY() ) );
			break;
			case ( HeroType::FLASHLIGHT ):
				hero_.reset( new FlashlightPlayerSprite( level.entranceX(), level.entranceY() ) );
			break;
		}

		hero_->renderable_id_ = level_state.addRenderable( std::unique_ptr<HeroRenderable>( new HeroRenderable() ), hero_->layer_ );
	}

	clearSprites();
	spritesFromMap( level_state );
};

void SpriteSystem::clearSprites()
{
	for ( int i = 0; i < sprites_.size(); ++i )
	{
		destroySprite( i, level_state_.currentMap() );
	}
};

void SpriteSystem::destroySprite( int n, Map& lvmap )
{
	if ( ( std::vector<std::unique_ptr<Sprite> >::size_type )( n ) < sprites_.size() )
	{
		if ( sprites_.at( n )->hasType( Sprite::SpriteType::DEATH_COUNT ) )
		{
			lvmap.deleteSprite( sprites_.at( n )->map_id_ );
			++permanently_killed_enemies_;
		}

		level_state_.removeRenderable( sprites_[ n ]->renderable_id_ );
		sprites_map_.erase( sprites_[ n ]->system_id_ );
		// Save time by just replacing dying sprite with last sprite & just
		// popping off last sprite ( which is null now, anyway ),
		// saving us from having to slowly shift every sprite back
		// with vector's native erase method.
		sprites_[ n ].reset( sprites_[ sprites_.size() - 1 ].release() );
		sprites_.pop_back();
	}
};

void SpriteSystem::update( LevelState& level_state )
{
	const Camera& camera = level_state.camera();
	Map& lvmap = level_state.currentMap();
	for ( auto i = 0; i < sprites_.size(); ++i )
	{
		if ( sprites_[ i ]->despawnWhenDead() )
		{
			if ( sprites_[ i ]->deathFinished() )
			{
				destroySprite( i, lvmap );
				continue; // SPRITE IS DEAD--DO NOT TRY UPDATING.
			}
		}

		switch( sprites_[ i ]->cameraMovement() )
		{
			case ( Sprite::CameraMovement::PAUSE_OFFSCREEN ):
				if ( camera.onscreen( sprites_[ i ]->hitBox(), OFFSCREEN_PADDING ) )
				{
					sprites_[ i ]->update( level_state );
				}
			break;

			case ( Sprite::CameraMovement::PERMANENT ):
				sprites_[ i ]->update( level_state );
			break;

			case ( Sprite::CameraMovement::RESET_INSTANTLY_OFFSCREEN ):
				if ( camera.onscreen( sprites_[ i ]->hitBox(), OFFSCREEN_PADDING ) )
				{
					sprites_[ i ]->update( level_state );
				}
				else
				{
					if ( camera.onscreen( sprites_[ i ]->originalHitBox(), OFFSCREEN_PADDING ) )
					{
						sprites_[ i ]->reset();
					}
				}
			break;

			case ( Sprite::CameraMovement::RESET_OFFSCREEN_AND_AWAY ):
				if ( camera.onscreen( sprites_[ i ]->hitBox(), OFFSCREEN_PADDING ) )
				{
					sprites_[ i ]->update( level_state );
				}
				else
				{
					if ( camera.offscreen( sprites_[ i ]->originalHitBox(), OFFSCREEN_PADDING ) )
					{
						sprites_[ i ]->reset();
					}
				}
			break;

			case ( Sprite::CameraMovement::DESPAWN_OFFSCREEN ):
				if ( camera.onscreen( sprites_[ i ]->hitBox(), OFFSCREEN_PADDING ) )
				{
					sprites_[ i ]->update( level_state );
				}
				else
				{
					destroySprite( i, lvmap );
				}
			break;
		}
	}

	if ( level_state.health().hp() <= 0 )
	{
		hero_->kill();
	}

	hero_->update( level_state );

	if ( hero_->deathFinished() )
	{
		level_state.events().fail();
	}
};

void SpriteSystem::spriteInteraction( LevelState& level_state )
{
	const Camera& camera = level_state.camera();
	for ( auto i = 0; i < sprites_.size(); ++i )
	{
		// Check that current sprite is valid & can interact with others.
		if ( sprites_[ i ] != nullptr && sprites_[ i ]->sprite_interact_ )
		{
			if ( permanentOrOnScreen( camera, *sprites_[ i ] ) )
			{
				if ( hero_->sprite_interact_ )
				{
					sprites_[ i ]->interact( *hero_, level_state );
					if ( !sprites_[ i ]->sprite_interact_from_this_to_others_only_ )
					{
						hero_->interact( *sprites_[ i ], level_state );
					}
				}

				// Interact with all other sprites.
				for ( auto j = 0; j < sprites_.size(); ++j )
				{
					if
					(
						i != j && // Skip if current sprite ( so it doesn’t interact with itself ).
						sprites_[ j ] != nullptr &&
						permanentOrOnScreen( camera, *sprites_[ j ] ) &&
						sprites_[ j ]->sprite_interact_ &&
						!sprites_[ j ]->sprite_interact_from_this_to_others_only_
					)
					{
						sprites_[ i ]->interact( *sprites_[ j ], level_state );
					}
				}
			}
		}
	}
};

void SpriteSystem::renderHero( const LevelState& level_state ) const
{
	hero_->render( level_state.camera() );
};

void SpriteSystem::renderSprite( int id, const LevelState& level_state ) const
{
	auto sprite = sprites_map_.find( id );
	if ( sprite == sprites_map_.end() )
	{
		printf( "Bad Render Sprite ID: %d\n", id );
		exit( -1 );
	};
	sprite->second->render( level_state.camera() );
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
	else if ( property.compare( "DOOM" ) == 0 )
	{
		return HeroType::DOOM;
	}
	else if ( property.compare( "PLANE" ) == 0 )
	{
		return HeroType::PLANE;
	}
	else if ( property.compare( "FLASHLIGHT" ) == 0 )
	{
		return HeroType::FLASHLIGHT;
	}

	return HeroType::NORMAL;
};

void SpriteSystem::interactWithMap( LevelState& level_state )
{
	Map& lvmap = level_state.currentMap();
	lvmap.interact( *hero_, level_state );

	for ( auto& s : sprites_ )
	{
		if ( s != nullptr )
		{
			if ( s->interactsWithBlocks() )
			{
				if
				(
					level_state.camera().onscreen( s->hitBox(), OFFSCREEN_PADDING ) ||
					s->hasCameraMovement( Sprite::CameraMovement::PERMANENT )
				)
				{
					lvmap.interact( *s, level_state );
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

bool SpriteSystem::spritesOnScreen( const Camera& camera, const Sprite::SpriteType type ) const
{
	for ( const auto& sprite : sprites_ )
	{
		if ( camera.onscreen( sprite->hitBox() ) && sprite->hasType( type ) )
		{
			return true;
		}
	}
	return false;
};

const std::vector<std::unique_ptr<Sprite>>& SpriteSystem::getSpritesList() const
{
	return sprites_;
};
