#include "level_state.hpp"
#include "mezun_math.hpp"
#include "sprite_graphics.hpp"
#include "sprite_system.hpp"
#include "zombie_generator_sprite.hpp"
#include "zombie_sprite.hpp"

static constexpr int LOOP_LIMIT = 50;
static void spawn( LevelState& level_state, int x, int y )
{
	level_state.sprites().spawn( std::unique_ptr<ZombieSprite> ( new ZombieSprite( Unit::BlocksToPixels( x ), Unit::BlocksToPixels( y ) - 1 ) ) );
};

ZombieGeneratorSprite::ZombieGeneratorSprite( int x, int y )
:
	Sprite( nullptr, x, y, 16, 16, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT ),
    timer_ ( 0 ),
    activated_ ( false ),
    prev_x_ ( -999999, -9999999, -99999999 )
{};

ZombieGeneratorSprite::~ZombieGeneratorSprite() {};

void ZombieGeneratorSprite::spawnForScreen( LevelState& level_state, int i, int max, int min )
{
	int x = 0;
	int loop = 0;
	do
	{
		if ( loop >= LOOP_LIMIT )
		{
			return;
		}
		x = Unit::PixelsToBlocks( level_state.camera().x() ) + mezun::randInt( max, min );
		++loop;
	} while ( !validSpawnPoint( x ) || ( x < prev_x_[ i ] + 2 && x > prev_x_[ i ] - 2 ) || ( x < player_x_ + 4 && x > player_x_ - 4 ) );
	prev_x_[ i ] = x;
	spawn( level_state, x, spawn_points_[ x ] );
};

void ZombieGeneratorSprite::customUpdate( LevelState& level_state )
{
	const Camera& camera = level_state.camera();
	if ( activated_ )
	{
		if ( timer_ >= 30 )
		{
			timer_ = 0;

			int number_of_zombies = 0;
			for ( const auto& sprite : level_state.sprites().getSpritesList() )
			{
				if ( sprite != nullptr && sprite->hasType( SpriteType::ZOMBIE ) )
				{
					++number_of_zombies;
				}
			}

			if ( number_of_zombies < 30 )
			{
				spawnForScreen( level_state, 0, Unit::WINDOW_WIDTH_BLOCKS, 0 );
				if ( camera.x() > Unit::WINDOW_WIDTH_PIXELS / 2 )
				{
					spawnForScreen( level_state, 1, 0, -Unit::WINDOW_WIDTH_BLOCKS );
				}
				if ( camera.right() < level_state.currentMap().widthPixels() - ( Unit::WINDOW_WIDTH_PIXELS / 2 ) )
				{
					spawnForScreen( level_state, 2, Unit::WINDOW_WIDTH_BLOCKS * 2, Unit::WINDOW_WIDTH_BLOCKS );
				}
			}
		}
		else
		{
			++timer_;
		}
	}
	else
	{
		if ( rightPixels() > camera.x() && xPixels() < camera.right() )
		{
			activated_ = true;
		}

		//
		// Generate list o’ spawn points.
		//
		// For each vertical slice o’ the map go down from the top tile till
		// it finds the 1st nonempty tile & tests if this is the right tile
		// ( the grass ground tile ). If it is, add the y position to the
		// list. Else add null -1 to the list. If no nonempty tile is found
		// ( for bottomless pits, for example ), also add null -1 to the list.
		//
		if ( spawn_points_.empty() )
		{
			const Map& lvmap = level_state.currentMap();
			for ( int x = 0; x < lvmap.widthBlocks(); ++x )
			{
				for ( int y = 0; y < lvmap.heightBlocks(); ++y )
				{
					const int i = lvmap.indexFromXAndY( x, y );
					for ( const auto& layer : lvmap.blocks_layers_ )
					{
						if ( i < layer.blocks_.size() )
						{
							const int type = layer.blocks_[ i ];
							if ( type > 0 )
							{
								if ( type == 74 )
								{
									spawn_points_.push_back( y );
									goto end_x_loop_set;
								}
								goto end_x_loop_null;
							}
						}
					}
				}
				end_x_loop_null:
					spawn_points_.push_back( -1 );

				end_x_loop_set:
					;
			}

			assert( spawn_points_.size() == lvmap.widthBlocks() );
		}
	}
};

void ZombieGeneratorSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( them.hasType( SpriteType::HERO ) )
	{
		player_x_ = Unit::SubPixelsToBlocks( them.centerXSubPixels() );
	}
};

bool ZombieGeneratorSprite::validSpawnPoint( int x ) const
{
	return x > 0 && x < spawn_points_.size() && spawn_points_[ x ] != -1;
};
