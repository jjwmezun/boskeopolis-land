#include <algorithm>
#include "block_system.hpp"
#include <cassert>
#include "main.hpp"
#include "map.hpp"
#include "map_layer_doom.hpp"
#include "mezun_math.hpp"
#include "player_doom_sprite.hpp"
#include "render.hpp"
#include "sprite_system.hpp"
#include "sprite.hpp"

#include <iostream>

static constexpr int TEXTURE_WIDTH = 164;
static constexpr int FLOOR_TEXTURE_X_OFFSET = 32;
static constexpr int CEILING_TEXTURE_X_OFFSET = 48;
static constexpr int MAP_WIDTH = Unit::WINDOW_WIDTH_PIXELS / 4;
static constexpr int MAP_HEIGHT = ( Unit::WINDOW_HEIGHT_PIXELS - 32 ) / 4;
static constexpr int MAP_PADDING = 8;
static constexpr int MAP_X = Unit::WINDOW_WIDTH_PIXELS - MAP_WIDTH - MAP_PADDING;
static constexpr int MAP_Y = Unit::WINDOW_HEIGHT_PIXELS - MAP_HEIGHT - MAP_PADDING - 32;

static constexpr double calcSideDist( bool negative, double player_pos, double map_pos, double delta )
{
	return ( negative )
		? ( player_pos - map_pos ) * delta
		: ( map_pos + 1.0 - player_pos ) * delta;
}

static constexpr double calcPerpWallDist( int map_pos, double hero_pos, int step, double ray_dir )
{
	const double perp_wall_dist =
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

	return
	  ( perp_wall_dist == 0.0  ) ?  0.000001
	: ( perp_wall_dist == -0.0 ) ? -0.000001
								 : perp_wall_dist;
}

static constexpr int calcBaseTextureX( double wall_x )
{
	return int( wall_x * double( Unit::PIXELS_PER_BLOCK ) );
}

static constexpr int calcFloorTexture( double floor )
{
	if ( floor < 0 )
	{
		const int t = Unit::PIXELS_PER_BLOCK - ( ( int )( std::abs( floor ) ) % Unit::PIXELS_PER_BLOCK );
		if ( t == Unit::PIXELS_PER_BLOCK ) { return 0; }
		return t;
	}
	return ( int )( floor * Unit::PIXELS_PER_BLOCK ) % Unit::PIXELS_PER_BLOCK;
}

MapLayerDoom::MapLayerDoom()
:
	floor_and_ceiling_ ( Render::createRenderBox( RAY_MAX, SCREEN_HEIGHT ) ),
	map_ ( Render::createRenderBox( MAP_WIDTH, MAP_HEIGHT ) ),
	items_ (),
	texture_source_ ( 0, 0, 1, Unit::PIXELS_PER_BLOCK ),
	render_screen_ ( 0, 0, RAY_MAX, SCREEN_HEIGHT ),
	map_dest_ ( MAP_X, MAP_Y, MAP_WIDTH, MAP_HEIGHT ),
	wall_distances_ (),
	wall_items_ (),
	floor_and_ceiling_pixels_ { 255 },
	floor_graphics_ { 43 },
	ceiling_graphics_ { 0 }
{};

MapLayerDoom::~MapLayerDoom()
{
	if ( floor_and_ceiling_ != nullptr )
	{
		SDL_DestroyTexture( floor_and_ceiling_ );
	}
};

void MapLayerDoom::render( const Camera& camera )
{
	Render::renderRenderBox( floor_and_ceiling_, render_screen_ );
	for ( const auto& slice : wall_items_ )
	{
		texture_source_.x = slice.texture_index;
		Render::renderObject
		(
			"tilesets/dungeon3.png",
			texture_source_,
			slice.position
		);
	}

	for ( const auto& item : items_ )
	{
		Render::renderObject
		(
			"tilesets/dungeon3.png",
			item.source,
			item.position
		);
	}
	Render::renderRenderBox( map_, { 0, 0, MAP_WIDTH, MAP_HEIGHT }, map_dest_ );
};

void MapLayerDoom::update( EventSystem& events, BlockSystem& blocks, const Camera& camera, Map& lvmap, const SpriteSystem& sprites )
{
	// 43 is not a valid color in our palette.
	if ( floor_graphics_[ 0 ][ 0 ] == 43 )
	{
		const Palette& palette = Main::getPalette();
		SDL_Surface* texture_data = Render::getSurface( "tilesets/dungeon3.png" );
		const Uint8* texture_pixels = ( Uint8* )( texture_data->pixels );

		for ( int y = 0; y < Unit::PIXELS_PER_BLOCK; ++y )
		{
			for ( int x = 0; x < Unit::PIXELS_PER_BLOCK; ++x )
			{
				const int texture_index = y * TEXTURE_WIDTH + x;
				const int floor_color_index = ( int )( texture_pixels[ texture_index + FLOOR_TEXTURE_X_OFFSET ] );
				const SDL_Color& floor_color = palette.color( floor_color_index );
				memcpy( ( void* )( &floor_graphics_[ y ][ x * NUMBER_OF_COLOR_CHANNELS ] ), ( void* )( &floor_color ), PIXEL_SIZE );
				const int ceiling_color_index = ( int )( texture_pixels[ texture_index + CEILING_TEXTURE_X_OFFSET ] );
				const SDL_Color& ceiling_color = palette.color( ceiling_color_index );
				memcpy( ( void* )( &ceiling_graphics_[ y ][ x * NUMBER_OF_COLOR_CHANNELS ] ), ( void* )( &ceiling_color ), PIXEL_SIZE );
			}
		}
	}

	//
	//  Update animation
	//
	//////////////////////////////////////////////////////////




	//
	//  WALLS
	//
	//////////////////////////////////////////////////////////

		const Sprite& hero = sprites.hero();
		// If hero hasn't moved, no need to update walls or items.
		if ( !hero.jump_lock_ )
		{
			return;
		}

		items_.clear();
		const double hero_x = ( double )( hero.centerXSubPixels() ) / ( double )( Unit::SUBPIXELS_PER_BLOCK );
		const double hero_y = ( double )( hero.centerYSubPixels() ) / ( double )( Unit::SUBPIXELS_PER_BLOCK );
		const double direction_x = ( double )( hero.direction_x_ ) / ( double )PlayerDoomSprite::CONVERSION_PRECISION;
		const double direction_y = ( double )( hero.direction_y_ ) / ( double )PlayerDoomSprite::CONVERSION_PRECISION;
		const double plane_x = ( double )( hero.jump_top_speed_ ) / ( double )PlayerDoomSprite::CONVERSION_PRECISION;
		const double plane_y = ( double )( hero.jump_top_speed_normal_ ) / ( double )PlayerDoomSprite::CONVERSION_PRECISION;

		auto block_list = lvmap.blocks_;
		bool items[ block_list.size() ] = { false };
		int mapxs[ RAY_MAX ];
		int mapys[ RAY_MAX ];
		for ( int ray_x = 0; ray_x < RAY_MAX; ++ray_x )
		{
			// Make camera x go from 1.0 to -1.0.
			const double camera_x  = -1.0 * ( ( 2.0 * ( ( double )( ray_x ) / RAY_MAX_D ) ) - 1.0 );
			const double ray_dir_x = direction_x + plane_x * camera_x;
			const double ray_dir_y = direction_y + plane_y * camera_x;
			const double delta_dist_x = std::abs( 1.0 / ray_dir_x );
			const double delta_dist_y = std::abs( 1.0 / ray_dir_y );
			const bool negative_ray_dir_x = ( ray_dir_x < 0.0 );
			const bool negative_ray_dir_y = ( ray_dir_y < 0.0 );
			const int step_x = ( negative_ray_dir_x ) ? -1 : 1;
			const int step_y = ( negative_ray_dir_y ) ? -1 : 1;

			int map_x = ( int )( hero_x );
			int map_y = ( int )( hero_y );
			double side_dist_x = calcSideDist( negative_ray_dir_x, hero_x, ( double )( map_x ), delta_dist_x );
			double side_dist_y = calcSideDist( negative_ray_dir_y, hero_y, ( double )( map_y ), delta_dist_y );
			int side;

			// DDA
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
				const int block_index = lvmap.indexFromXAndY( map_x, map_y );
				const int block = lvmap.block( block_index );
				if ( block == 10 )
				{
					mapxs[ ray_x ] = map_x;
					mapys[ ray_x ] = map_y;
					break;
				}
				else if ( block == 1 )
				{
					items[ block_index ] = true;
				}
			}

			const double perp_wall_dist = ( side == 0 )
				? calcPerpWallDist( map_x, hero_x, step_x, ray_dir_x )
				: calcPerpWallDist( map_y, hero_y, step_y, ray_dir_y );
			wall_distances_[ ray_x ] = perp_wall_dist;

			const int line_height = ( int )( SCREEN_HEIGHT_D / perp_wall_dist );
			const int draw_start = ( int )( SCREEN_HEIGHT_D / 2.0 ) - ( int )( ( double )( line_height ) / 2.0 );

			double wall_x = ( side == 0 )
				? hero_y + perp_wall_dist * ray_dir_y
				: hero_x + perp_wall_dist * ray_dir_x;
			wall_x = wall_x - floor( wall_x );
			const int texture_x = ( ( ( side == 0 && ray_dir_x > 0 ) || ( side == 1 && ray_dir_y < 0 ) )
				? 16 - calcBaseTextureX( wall_x ) - 1
				: calcBaseTextureX( wall_x ) ) % 16;

			wall_items_[ ray_x ] = { { ray_x, draw_start, 1, line_height }, texture_x + ( side * 16 ) };


		//
		//  Floor Plans
		//
		/////////////////////////////////////////////////////////

			const Direction::Simple wall_dir =
				  ( side == 0 && ray_dir_x > 0 ) ? Direction::Simple::RIGHT
				: ( side == 0 && ray_dir_x < 0 ) ? Direction::Simple::LEFT
				: ( side == 1 && ray_dir_y > 0 ) ? Direction::Simple::DOWN
				                                 : Direction::Simple::UP;

			const double floor_x_wall =
				  ( wall_dir == Direction::Simple::RIGHT ) ? ( double )( map_x )
				: ( wall_dir == Direction::Simple::LEFT )  ? ( double )( map_x ) + 1.0
				                                           : ( double )( map_x ) + wall_x;

	   		const double floor_y_wall =
	   			  ( wall_dir == Direction::Simple::DOWN ) ? ( double )( map_y )
	   			: ( wall_dir == Direction::Simple::UP )   ? ( double )( map_y ) + 1.0
	   			                                          : ( double )( map_y ) + wall_x;

			// Start floor just under wall.
			const int floor_start = draw_start + ( int )( ( double )( line_height ) / 2.0 );
			for ( int y = floor_start; y < SCREEN_HEIGHT; ++y )
			{
				const double current_distance = SCREEN_HEIGHT_D / ( ( 2.0 * ( double )( y ) ) - SCREEN_HEIGHT_D );
				const double weight = current_distance / perp_wall_dist;
				const double current_floor_x = weight * floor_x_wall + ( 1.0 - weight ) * hero_x;
				const double current_floor_y = weight * floor_y_wall + ( 1.0 - weight ) * hero_y;
				const int floor_texture_x = calcFloorTexture( current_floor_x );
				const int floor_texture_y = calcFloorTexture( current_floor_y );
				memcpy( ( void* )( &floor_and_ceiling_pixels_[ y ][ ray_x * NUMBER_OF_COLOR_CHANNELS ] ), ( void* )( &floor_graphics_[ floor_texture_y ][ floor_texture_x * NUMBER_OF_COLOR_CHANNELS ] ), PIXEL_SIZE );
				memcpy( ( void* )( &floor_and_ceiling_pixels_[ SCREEN_HEIGHT - y - 1 ][ ray_x * NUMBER_OF_COLOR_CHANNELS ] ), ( void* )( &ceiling_graphics_[ floor_texture_y ][ floor_texture_x * NUMBER_OF_COLOR_CHANNELS ] ), PIXEL_SIZE );
			}
	}

	items_.clear();
	int item = 0;
	for ( const int i : items )
	{
		if ( i )
		{
		const double x = ( double )( lvmap.mapX( item ) );
		const double y = ( double )( lvmap.mapY( item ) );
		const double distance_x = abs( x - hero_x );
		const double distance_y = abs( y - hero_y );
		const double distance = sqrt( distance_x * distance_x + distance_y * distance_y );
		const double item_x_relative = x - hero_x;
		const double item_y_relative = y - hero_y;
		const double inverse_delta = 1.0 / ( plane_x * direction_y - direction_x * plane_y );
		const double transform_x = inverse_delta * ( direction_y * item_x_relative - direction_x * item_y_relative );
		const double transform_y = inverse_delta * ( -plane_y * item_x_relative + plane_x * item_y_relative );
		const double item_screen_x = RAY_MAX - ( ( RAY_MAX / 2.0 ) * ( 1.0 + transform_x / transform_y ) );
		const int line_height = ( int )( SCREEN_HEIGHT_D / transform_y );
		const int draw_start_y = ( int )( SCREEN_HEIGHT_D / 2.0 ) - ( int )( ( double )( line_height ) / 2.0 );
		const int draw_start_x = item_screen_x - ( double )( line_height ) / 2.0;

		double source_left = 1.0;
		double source_right = 0;

		/*
		for ( int lx = 0; lx < line_height; ++lx )
		{
			const double slice_distance_x = abs( x + int( floor( ( double )( lx ) / 2.0 ) ) - hero_x );
			const double slice_distance = sqrt( slice_distance_x * slice_distance_x + distance_y * distance_y );
			const int screen_x = draw_start_x + lx;
			if ( screen_x < 0 || screen_x >= RAY_MAX ) { continue; }
			if ( slice_distance < wall_distances_[ screen_x ] )
			{
				show = true;
				break;
			}
			//source_left = ( double )( line_height - lx ) / ( double )( line_height );
		}
		for ( int lx = 0; lx < line_height; ++lx )
		{
			const int screen_x = draw_start_x + lx;
			const double relative_x = ( ( double )( lx ) / ( double )( line_height ) ) - 0.5;
			const double slice_x = x + relative_x;
			const double slice_distance_x = abs( relative_x - hero_x );
			const double slice_distance = sqrt( slice_distance_x * slice_distance_x + distance_y * distance_y );
			if ( slice_distance < wall_distances_[ screen_x ] )
			{
				break;
			}
			source_left = ( double )( line_height - lx ) / ( double )( line_height );
		}
		*/

		const int clipped_source_width = ( int )( 16.0 * source_left );
		const int clipped_source_x = 64 + ( 16 - clipped_source_width );
		const int clipped_width = ( int )( line_height * source_left );
		const int clipped_x = item_screen_x + ( int )( line_height ) - clipped_width;

		items_.push_back({ { clipped_x, draw_start_y, clipped_width, line_height }, { clipped_source_x, 0, clipped_source_width, 16 }, distance, 1 });
		}
		++item;
	}
	std::sort( items_.begin(), items_.end(), sortItems );

	SDL_UpdateTexture( floor_and_ceiling_, nullptr, &floor_and_ceiling_pixels_, RAY_MAX * NUMBER_OF_COLOR_CHANNELS * sizeof( Uint8 ) );


	// Update Map

	Render::setRenderTarget( map_ );
		Render::renderRect( { 0, 0, MAP_WIDTH, MAP_HEIGHT }, 1 );

		const std::vector<Block>& block_obj_list = blocks.getBlocksList();
		for ( const Block& b : block_obj_list )
		{
			if ( b.typeID() == 9 || b.typeID() == 0 )
			{
				Render::renderRect
				(
					{
						camera.relativeX( Unit::SubPixelsToPixels( b.hit_box_ ) ) / 4,
						camera.relativeY( Unit::SubPixelsToPixels( b.hit_box_ ) ) / 4,
						4,
						4
					},
					( b.typeID() == 9 ) ? 5 : 2
				);
			}
		}

		const double map_icon_angle = ( double )( hero.bounce_height_ ) / ( double )PlayerDoomSprite::CONVERSION_PRECISION;
		const int map_icon_x = camera.relativeX( Unit::SubPixelsToPixels( hero.hit_box_ ) ) / 4;
		const int map_icon_y = camera.relativeY( Unit::SubPixelsToPixels( hero.hit_box_ ) ) / 4;
		const int map_line_y = ( direction_y + 1.0 ) * ( MAP_HEIGHT / 2 );
		const int map_line_x = ( direction_x + 1.0 ) * ( MAP_WIDTH / 2 );
		for ( int x = 0; x < RAY_MAX; ++x )
		{
			const int mx = ( mapxs[ x ] * 16 - camera.x() ) / 4;
			const int my = ( mapys[ x ] * 16 - camera.y() ) / 4;
			Render::renderLine( mx, my, map_icon_x + 2, map_icon_y + 2, 4 );
		}
		Render::renderObject
		(
			"tilesets/dungeon3.png",
			{ 160, 0, 4, 4 },
			{
				map_icon_x,
				map_icon_y,
				4,
				4
			},
			SDL_FLIP_NONE,
			map_icon_angle
		);

	Render::releaseRenderTarget();
};

bool MapLayerDoom::sortItems( const Item& lhs, const Item& rhs )
{
	return lhs.distance > rhs.distance;
};
