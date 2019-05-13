#include <algorithm>
#include <cassert>
#include "main.hpp"
#include "map.hpp"
#include "map_layer_doom.hpp"
#include "player_doom_sprite.hpp"
#include "render.hpp"
#include "sprite_system.hpp"
#include "sprite.hpp"

#include <iostream>

#define SCREEN_HEIGHT Unit::WINDOW_HEIGHT_PIXELS - 32
#define SCREEN_HEIGHT_D ( double )( SCREEN_HEIGHT )

static constexpr int FLOOR_TEXTURE_SIZE = 8;
static const int FLOOR_TEXTURE[ FLOOR_TEXTURE_SIZE ][ FLOOR_TEXTURE_SIZE ] =
{
	{ 4, 4, 4, 4, 4, 4, 4, 4 },
	{ 4, 3, 4, 4, 4, 5, 4, 4 },
	{ 4, 4, 4, 4, 4, 4, 3, 4 },
	{ 4, 5, 4, 3, 4, 4, 4, 4 },
	{ 4, 4, 4, 4, 4, 4, 4, 5 },
	{ 3, 4, 4, 4, 5, 4, 3, 4 },
	{ 4, 4, 3, 4, 4, 4, 4, 4 },
	{ 5, 4, 4, 4, 4, 3, 4, 4 }
};

static constexpr double calcSideDist( bool negative, double player_pos, double map_pos, double delta )
{
	return ( negative )
		? ( player_pos - map_pos ) * delta
		: ( map_pos + 1.0 - player_pos ) * delta;
}

static constexpr double calcPerpWallDist( int map_pos, double hero_pos, int step, double ray_dir )
{
	return
		(
			( double )( map_pos )
			- hero_pos
			+
			(
				( double )( 1 - step )
				/ 2.0
			)
		)
		/ ray_dir;
}

static constexpr int calcBaseTextureX( double wall_x )
{
	return int( wall_x * double( 16 ) );
}

static constexpr bool sortItems( const sdl2::SDLRect& lhs, const sdl2::SDLRect& rhs )
{
	return lhs.h < rhs.h;
};

static constexpr int calcFloorTexture( double floor )
{
	if ( floor < 0 )
	{
		const int t = FLOOR_TEXTURE_SIZE - ( ( int )( std::abs( floor ) ) % FLOOR_TEXTURE_SIZE );
		if ( t == 8 ) { return 0; }
		return t;
	}
	return ( int )( floor * FLOOR_TEXTURE_SIZE ) % FLOOR_TEXTURE_SIZE;
}

MapLayerDoom::MapLayerDoom()
:
	floor_and_ceiling_ ( Render::createRenderBox( RAY_MAX, SCREEN_HEIGHT ) ),
	animation_timer_ (),
	items_ (),
	item_types_ (),
	wall_src_ ( 0, 0, 1, 16 ),
	item_src_ ( 0, 64, 16, 16 ),
	colors_ { 0 },
	texture_coordinate_ { 0 },
	lines_ {{ 0, 0, 0, 0 }},
	floor_and_ceiling_x_ { 0 },
	floor_and_ceiling_y_ { 0 }
{};

MapLayerDoom::~MapLayerDoom() {};

void MapLayerDoom::render( const Camera& camera )
{
	Render::renderRenderBox( floor_and_ceiling_ );
	for ( int i = 0; i < RAY_MAX; ++i )
	{
		//Render::renderRect( lines_[ i ], colors_[ i ] + 4 );
		wall_src_.x = texture_coordinate_[ i ] + ( 16 * colors_[ i ] );
		Render::renderObject
		(
			"tilesets/dungeon3.png",
			wall_src_,
			lines_[ i ]
		);
	}

	for ( auto item : items_ )
	{
		Render::renderObject
		(
			"tilesets/universal.png",
			item_src_,
			item
		);
	}
};

void MapLayerDoom::update( EventSystem& events, BlockSystem& blocks, const Camera& camera, Map& lvmap, const SpriteSystem& sprites )
{
	if ( animation_timer_.hit() )
	{
		item_src_.x += 16;
		if ( item_src_.x > 5 * 16 )
		{
			item_src_.x = 0;
		}
	}
	animation_timer_.update();

	const Sprite& hero = sprites.hero();

	// If hero hasn't moved, no need to update walls or items.
	if ( !hero.jump_lock_ )
	{
		return;
	}

	items_.clear();
	const double hero_x = ( double )( hero.centerXSubPixels() ) / 16000.0;
	const double hero_y = ( double )( hero.centerYSubPixels() ) / 16000.0;
	const double direction_x = ( double )( hero.direction_x_ ) / PlayerDoomSprite::CONVERSION_PRECISION;
	const double direction_y = ( double )( hero.direction_y_ ) / PlayerDoomSprite::CONVERSION_PRECISION;
	const double plane_x = ( double )( hero.jump_top_speed_ ) / PlayerDoomSprite::CONVERSION_PRECISION;
	const double plane_y = ( double )( hero.jump_top_speed_normal_ ) / PlayerDoomSprite::CONVERSION_PRECISION;
	auto block_map = lvmap.blocks_;

	for ( int ray_x = 0; ray_x < RAY_MAX; ++ray_x )
	{
		const double camera_x  = -1.0 * ( ( 2.0 * ( ( double )( ray_x ) / RAY_MAX_D ) ) - 1.0 );
		const double ray_dir_x = direction_x + plane_x * camera_x;
		const double ray_dir_y = direction_y + plane_y * camera_x;
		const double delta_dist_x = std::abs( 1.0 / ray_dir_x );
		const double delta_dist_y = std::abs( 1.0 / ray_dir_y );
		const bool negative_ray_dir_x = ( ray_dir_x < 0 );
		const bool negative_ray_dir_y = ( ray_dir_y < 0 );
		const int step_x = ( negative_ray_dir_x ) ? -1 : 1;
		const int step_y = ( negative_ray_dir_y ) ? -1 : 1;

		int map_x = ( int )( hero_x );
		int map_y = ( int )( hero_y );

		double side_dist_x = calcSideDist( negative_ray_dir_x, hero_x, ( double )( map_x ), delta_dist_x );
		double side_dist_y = calcSideDist( negative_ray_dir_y, hero_y, ( double )( map_y ), delta_dist_y );
		//std::cout<<side_dist_x<<", "<<side_dist_y<<std::endl;
		int side;

		// DDA
		const bool is_middle = ray_x == ( int )( RAY_MAX_D / 2 );
		bool took_item = false;
		for ( int t = 0; t < 1000; ++t ) // Just to make sure we don't get any rare infinite loops.
		{
			if ( side_dist_x < side_dist_y )
			{
				side_dist_x += delta_dist_x;
				map_x += step_x;
				side = 0;
			}
			else
			{
				side_dist_y += delta_dist_y;
				map_y += step_y;
				side = 1;
			}

			//Check if ray has hit a wall
			const int block = block_map[ lvmap.indexFromXAndY( map_x, map_y ) ];
			if ( block == 10 )
			{
				break;
			}
			else if ( !took_item && block == 1 )
			{
				const double perp_wall_dist = ( side == 0 )
					? calcPerpWallDist( map_x, hero_x, step_x, ray_dir_x )
					: calcPerpWallDist( map_y, hero_y, step_y, ray_dir_y );
				const int line_height = ( int )( SCREEN_HEIGHT_D / perp_wall_dist );
				const int draw_start_y = ( int )( SCREEN_HEIGHT_D / 2 ) - ( int )( ( double )( line_height ) / 2 );
				const int draw_start_x = ( int )( ( double )( ray_x ) / 2 ) - ( int )( ( double )( line_height ) / 2 );
				items_.push_back( { ray_x, draw_start_y, line_height, line_height } );
				block_map[ lvmap.indexFromXAndY( map_x, map_y ) ] = 0;
			}
		}

		const double perp_wall_dist = ( side == 0 )
			? calcPerpWallDist( map_x, hero_x, step_x, ray_dir_x )
			: calcPerpWallDist( map_y, hero_y, step_y, ray_dir_y );
		const int line_height = ( perp_wall_dist != 0.0 && perp_wall_dist != -0.0 )
			? std::min( SCREEN_HEIGHT, ( int )( SCREEN_HEIGHT_D / perp_wall_dist ) )
			: SCREEN_HEIGHT;
		const int draw_start = ( int )( SCREEN_HEIGHT_D / 2.0 ) - ( int )( ( double )( line_height ) / 2.0 );

		const double wall_x = ( side == 0 )
			? hero_y + perp_wall_dist * ray_dir_y
			: hero_x + perp_wall_dist * ray_dir_x;
		const double wall_x_remain = wall_x - floor( wall_x );
		const int texture_x = ( ( ( side == 0 && ray_dir_x > 0 ) || ( side == 1 && ray_dir_y < 0 ) )
			? 16 - calcBaseTextureX( wall_x_remain ) - 1
			: calcBaseTextureX( wall_x_remain ) ) % 16;

		const Direction::Simple wall_dir =
			  ( side == 0 && ray_dir_x > 0 ) ? Direction::Simple::RIGHT
			: ( side == 0 && ray_dir_x < 0 ) ? Direction::Simple::LEFT
			: ( side == 1 && ray_dir_y > 0 ) ? Direction::Simple::DOWN
			                                 : Direction::Simple::UP;

		const double floor_x_wall =
			  ( wall_dir == Direction::Simple::RIGHT ) ? ( double )( map_x )
			: ( wall_dir == Direction::Simple::LEFT )  ? ( double )( map_x ) + 1.0
			                                           : ( double )( map_x ) + wall_x_remain;

   		const double floor_y_wall =
   			  ( wall_dir == Direction::Simple::DOWN ) ? ( double )( map_y )
   			: ( wall_dir == Direction::Simple::UP )   ? ( double )( map_y ) + 1.0
   			                                          : ( double )( map_y ) + wall_x_remain;

		const int floor_start = draw_start + ( int )( ( double )( line_height ) / 2.0 );
		for ( int y = floor_start; y < SCREEN_HEIGHT; ++y )
		{
			const double current_distance = SCREEN_HEIGHT_D / ( 2.0 * ( double )( y ) - SCREEN_HEIGHT_D );
			const double weight = ( current_distance - 0.0 ) / ( perp_wall_dist - 0.0 );
			const double current_floor_x = weight * floor_x_wall + ( 1.0 - weight ) * hero_x;
			const double current_floor_y = weight * floor_y_wall + ( 1.0 - weight ) * hero_y;
			const int floor_texture_x = calcFloorTexture( current_floor_x );
			const int floor_texture_y = calcFloorTexture( current_floor_y );
			floor_and_ceiling_x_[ y ][ ray_x ] = floor_and_ceiling_x_[ SCREEN_HEIGHT - y ][ ray_x ] = floor_texture_x;
			floor_and_ceiling_y_[ y ][ ray_x ] = floor_and_ceiling_y_[ SCREEN_HEIGHT - y ][ ray_x ] = floor_texture_y;
		}

		colors_[ ray_x ] = side;
		lines_[ ray_x ] = { ray_x, draw_start, 1, line_height };
		texture_coordinate_[ ray_x ] = texture_x;
	}

	std::sort( items_.begin(), items_.end(), &sortItems );

	const Palette& palette = Main::getPalette();
	Uint8 pixels[ SCREEN_HEIGHT ][ RAY_MAX * 4 ] = { 255 };
	for ( int y = 0; y < SCREEN_HEIGHT; ++y )
	{
		for ( int x = 0; x < RAY_MAX; ++x )
		{
			const int color_index = FLOOR_TEXTURE[ floor_and_ceiling_y_[ y ][ x ] ][ floor_and_ceiling_x_[ y ][ x ] ];
			const sdl2::SDLColor& color = palette.color( color_index );
			pixels[ y ][ x * 4 ] = color.a;
			pixels[ y ][ ( x * 4 ) + 1 ] = color.b;
			pixels[ y ][ ( x * 4 ) + 2 ] = color.g;
			pixels[ y ][ ( x * 4 ) + 3 ] = color.r;
		}
	}
	SDL_UpdateTexture( floor_and_ceiling_, NULL, pixels, RAY_MAX * 4 * sizeof( Uint8 ) );
};
