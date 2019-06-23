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

static constexpr int TEXTURE_WIDTH = 336;
static constexpr int FLOOR_TEXTURE_X_OFFSET = 32;
static constexpr int CEILING_TEXTURE_X_OFFSET = 48;
static constexpr int MAP_WIDTH = Unit::WINDOW_WIDTH_PIXELS / 4;
static constexpr int MAP_HEIGHT = ( Unit::WINDOW_HEIGHT_PIXELS - 32 ) / 4;
static constexpr int MAP_PADDING = 8;
static constexpr int MAP_X = Unit::WINDOW_WIDTH_PIXELS - MAP_WIDTH - MAP_PADDING;
static constexpr int MAP_Y = Unit::WINDOW_HEIGHT_PIXELS - MAP_HEIGHT - MAP_PADDING - 32;

static constexpr double subPixelsToBlocksDouble( int sp )
{
	return ( double )( sp ) / ( double )( Unit::SUBPIXELS_PER_BLOCK );
};

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
	hero_shooting_ ( false ),
	animation_timer_ (),
	hero_ ( nullptr ),
	floor_and_ceiling_ ( Render::createRenderBox( RAY_MAX, SCREEN_HEIGHT ) ),
	map_ ( Render::createRenderBox( MAP_WIDTH, MAP_HEIGHT ) ),
	item_info_ (),
	items_ (),
	item_frames_ ( { 0, 288, 7 * 16, 9 * 16, 176, 272 } ),
	texture_source_ ( 0, 0, 1, Unit::PIXELS_PER_BLOCK ),
	render_screen_ ( 0, 0, RAY_MAX, SCREEN_HEIGHT ),
	map_src_ ( 0, 0, MAP_WIDTH, MAP_HEIGHT ),
	map_dest_ ( MAP_X, MAP_Y, MAP_WIDTH, MAP_HEIGHT ),
	hand_src_ ( 0, 0, 55, 24 ),
	hand_dest_ ( ( Unit::WINDOW_WIDTH_PIXELS / 2 - ( 24 * 4 ) ), Unit::WINDOW_HEIGHT_PIXELS - 32 - ( 24 * 4 ), 55 * 4, 24 * 4 ),
	map_bars_
	({
		{ 0, MAP_HEIGHT - 2, MAP_WIDTH, 2 },
		{ MAP_WIDTH - 2, 0, 2, MAP_HEIGHT },
		{ 0, 0, MAP_WIDTH, 2 },
		{ 0, 0, 2, MAP_HEIGHT }
	}),
	hand_frames_ ({ 0, 24, 48, 72, 96, 120, 120, 144, 144, 168, 168, 168, 168, 192, 192, 192, 192, 168, 168, 144, 120, 96, 72, 48, 24 }),
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

	for ( const auto& item : items_ )
	{
		Render::renderObject
		(
			"tilesets/dungeon3.png",
			item.source,
			item.position
		);
	}

	Render::renderObject
	(
		"sprites/autumn-3d-hand.png",
		hand_src_,
		hand_dest_
	);
	Render::renderRenderBox( map_, map_src_, map_dest_ );
};

void MapLayerDoom::update( EventSystem& events, BlockSystem& blocks, const Camera& camera, Map& lvmap, const SpriteSystem& sprites )
{
	// 43 is not a valid color in our palette.
	if ( testFloorAndCeilingNotSetup() )
	{
		setupFloorAndCeiling();
	}
	updateAnimation();

	if ( hero_ == nullptr )
	{
		hero_ = dynamic_cast<const PlayerDoomSprite*> ( &sprites.hero() );
	}

	//
	//  WALLS
	//
	//////////////////////////////////////////////////////////

		hand_src_.y = hand_frames_[ ( hero_->shoot_timer_ > 32 - 25 ) ? 32 - hero_->shoot_timer_ : 0 ];

		items_.clear();
		const double hero_x = subPixelsToBlocksDouble( hero_->centerXSubPixels() );
		const double hero_y = subPixelsToBlocksDouble( hero_->centerYSubPixels() );
		const double direction_x = hero_->ddirx_;
		const double direction_y = hero_->ddiry_;
		const double plane_x = hero_->planex_;
		const double plane_y = hero_->planey_;

		const auto& sprites_list = sprites.getSpritesList();

		// RAY LOOP START

		bool items_caught[ lvmap.blocks_.size() ] = { false };
		int block_types[ RAY_MAX ];
		item_info_.clear();

		// Start by assuming all sprites not caught.
		bool sprites_caught[ sprites_list.size() ] = { false };

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

				for ( int i = 0; i < ( int )( sprites_list.size() ); ++i )
				{
					const auto& sprite = sprites_list[ i ];
					const sdl2::SDLRect box = { Unit::BlocksToSubPixels( map_x ), Unit::BlocksToSubPixels( map_y ), Unit::BlocksToSubPixels( 1 ), Unit::BlocksToSubPixels( 1 ) };
					if ( sprite->inBox( box ) )
					{
						sprites_caught[ i ] = true;
					}
				}

				//Check if ray has hit a wall
				const int block_index = lvmap.indexFromXAndY( map_x, map_y );
				const int block = lvmap.block( block_index );
				if ( block == 10 || block == 19 || block == 20 )
				{
					block_types[ ray_x ] = block;
					break;
				}
				else if ( !items_caught[ block_index ] && ( block == 1 || block == 6 || block == 5 || block == 2 ) )
				{
					int type = 0;
					switch ( block )
					{
						case ( 2 ):
						{
							type = 4;
						}
						break;
						case ( 5 ):
						{
							type = 3;
						}
						break;
						case ( 6 ):
						{
							type = 1;
						}
						break;
					}
					const double x = ( double )( lvmap.mapX( block_index ) ) + 0.5;
					const double y = ( double )( lvmap.mapY( block_index ) ) + 0.5;
					item_info_.push_back( { x, y, type } );
					items_caught[ block_index ] = true;
				}
			}

			if ( hero_->has_moved_ ) // Don’t need to run loop ’less the player has moved.
			{
				const double perp_wall_dist = ( side == 0 )
					? calcPerpWallDist( map_x, hero_x, step_x, ray_dir_x )
					: calcPerpWallDist( map_y, hero_y, step_y, ray_dir_y );

				const double line_height_d = SCREEN_HEIGHT_D / perp_wall_dist;
				const int line_height = ( int )( line_height_d );
				wall_distances_[ ray_x ] = line_height_d; // We'll need this for items later.
				const int draw_start = ( int )( SCREEN_HEIGHT_D / 2.0 ) - ( int )( ( double )( line_height ) / 2.0 );

				double wall_x = ( side == 0 )
					? hero_y + perp_wall_dist * ray_dir_y
					: hero_x + perp_wall_dist * ray_dir_x;
				wall_x = wall_x - floor( wall_x );
				const int texture_x = ( ( ( side == 0 && ray_dir_x > 0 ) || ( side == 1 && ray_dir_y < 0 ) )
					? 16 - calcBaseTextureX( wall_x ) - 1
					: calcBaseTextureX( wall_x ) ) % 16;
				const int texture_index = ( block_types[ ray_x ] == 10 ) ? 0 : ( block_types[ ray_x ] == 19 ) ? 8 : 11;
				wall_items_[ ray_x ] = { { ray_x, draw_start, 1, line_height }, texture_x + ( ( texture_index + side ) * 16 ) };


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
	}
	// RAY LOOP END

	if ( hero_->has_moved_ )
	{
		SDL_UpdateTexture( floor_and_ceiling_, nullptr, &floor_and_ceiling_pixels_, RAY_MAX * NUMBER_OF_COLOR_CHANNELS * sizeof( Uint8 ) );
		Render::setRenderTarget( floor_and_ceiling_ );
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
		Render::releaseRenderTarget();
	}



	//
	//  ITEMS
	//
	/////////////////////////////////////////////////////////

		std::vector<ItemInfo> items = item_info_;
		for ( int i = 0; i < ( int )( sprites_list.size() ); ++i )
		{
			if ( sprites_caught[ i ] )
			{
				const auto& sprite = sprites_list[ i ];
				if ( sprite->hasType( Sprite::SpriteType::ENEMY ) )
				{
					if ( !sprite->jump_lock_ )
					{
						items.push_back( { subPixelsToBlocksDouble( sprite->hit_box_.x ) + 0.5, subPixelsToBlocksDouble( sprite->hit_box_.y ) + 0.5, 2 } );
					}
				}
				else
				{
					items.push_back( { subPixelsToBlocksDouble( sprite->hit_box_.x ) + 0.5, subPixelsToBlocksDouble( sprite->hit_box_.y ) + 0.5, 5 } );
				}
			}
		}

		items_.clear();
		for ( int i = 0; i < ( int )( items.size() ); ++i )
		{
			const ItemInfo& item = items[ i ];
			const double x = item.x;
			const double y = item.y;
			const double distance_x = abs( x - hero_x );
			const double distance_y = abs( y - hero_y );
			const double distance = sqrt( distance_x * distance_x + distance_y * distance_y );
			const double item_x_relative = x - hero_x;
			const double item_y_relative = y - hero_y;
			const double inverse_delta = 1.0 / ( plane_x * direction_y - direction_x * plane_y );
			const double transform_x = inverse_delta * ( direction_y * item_x_relative - direction_x * item_y_relative );
			const double transform_y = inverse_delta * ( -plane_y * item_x_relative + plane_x * item_y_relative );
			const double line_height_d = abs( SCREEN_HEIGHT_D / transform_y );
			const int line_height = ( int )( line_height_d );
			const int item_screen_x =
				RAY_MAX
				-
					( int )
					(
						( ( double )( RAY_MAX ) / 2.0 )
						* ( 1.0 + transform_x / transform_y )
					);
			const int draw_start_y = ( int )( SCREEN_HEIGHT_D / 2.0 ) - ( int )( ( double )( line_height ) / 2.0 );
			const int draw_start_x = item_screen_x - ( int )( ( double )( line_height ) / 2.0 );

			double source_left = 1.0;
			double right_clip = 0;

			int lx = 0;
			// Test for left clipping
			while ( lx < line_height )
			{
				const int screen_x = draw_start_x + lx;
				if ( screen_x > 0 && screen_x < RAY_MAX && line_height_d >= wall_distances_[ screen_x ] )
				{
					break;
				}
				source_left = ( double )( line_height - lx ) / ( double )( line_height );
				++lx;
			}
			// Test for right clipping
			while ( lx < line_height )
			{
				const int screen_x = draw_start_x + lx;
				if ( screen_x > 0 && screen_x < RAY_MAX && line_height_d < wall_distances_[ screen_x ] )
				{
					right_clip = ( double )( line_height - lx ) / ( double )( line_height );
					break;
				}
				++lx;
			}

			const int clipped_source_width_from_left = ( int )( 16.0 * source_left );
			const int clipped_source_x = item_frames_[ item.type ] + ( 16 - clipped_source_width_from_left );
			const int clipped_source_width = clipped_source_width_from_left - ( int )( right_clip * 16.0 );
			const int clipped_width_from_left = ( int )( line_height * source_left );
			const int clipped_x = draw_start_x + ( int )( line_height ) - clipped_width_from_left;
			const int clipped_width = clipped_width_from_left - ( int )( line_height * right_clip );

			items_.push_back({ { clipped_x, draw_start_y, clipped_width, line_height }, { clipped_source_x, 16, clipped_source_width, 16 }, distance, 1 });
		}
		std::sort( items_.begin(), items_.end(), sortItems );



	//
	//  MINIMAP
	//
	/////////////////////////////////////////////////////////

		Render::setRenderTarget( map_ );
			Render::renderRect( { 0, 0, MAP_WIDTH, MAP_HEIGHT }, 1 );

			const std::vector<Block>& block_obj_list = blocks.getBlocksList();
			for ( const Block& b : block_obj_list )
			{
				if ( b.typeID() == 9 || b.typeID() == 18 || b.typeID() == 19 || b.typeID() == 0 || b.typeID() == 1 || b.typeID() == 4 )
				{
					Render::renderRect
					(
						{
							camera.relativeX( Unit::SubPixelsToPixels( b.hit_box_ ) ) / 4,
							camera.relativeY( Unit::SubPixelsToPixels( b.hit_box_ ) ) / 4,
							4,
							4
						},
						( b.typeID() == 9 || b.typeID() == 18 || b.typeID() == 19 ) ? 5 : 2
					);
				}
			}

			const double map_icon_angle = hero_->angle_;
			const int map_icon_x = camera.relativeX( Unit::SubPixelsToPixels( hero_->hit_box_ ) ) / 4;
			const int map_icon_y = camera.relativeY( Unit::SubPixelsToPixels( hero_->hit_box_ ) ) / 4;
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

			for ( const auto& sprite : sprites_list )
			{
				const int size = ( sprite->hasType( Sprite::SpriteType::ENEMY ) ? 4 : 2 );
				const int enemy_icon_x = camera.relativeX( Unit::SubPixelsToPixels( sprite->hit_box_ ) ) / 4;
				const int enemy_icon_y = camera.relativeY( Unit::SubPixelsToPixels( sprite->hit_box_ ) ) / 4;
				Render::renderRect
				(
					{
						enemy_icon_x,
						enemy_icon_y,
						size,
						size
					},
					3
				);
			}
			Render::renderRect( map_bars_[ 0 ], 5 );
			Render::renderRect( map_bars_[ 1 ], 5 );
			Render::renderRect( map_bars_[ 2 ], 5 );
			Render::renderRect( map_bars_[ 3 ], 5 );
		Render::releaseRenderTarget();
};

bool MapLayerDoom::sortItems( const Item& lhs, const Item& rhs )
{
	return lhs.distance > rhs.distance;
};

inline bool MapLayerDoom::testFloorAndCeilingNotSetup() const
{
	return floor_graphics_[ 0 ][ 0 ] == 43;
}

inline void MapLayerDoom::setupFloorAndCeiling()
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

void MapLayerDoom::updateAnimation()
{
	if ( animation_timer_.hit() )
	{
		item_frames_[ 3 ] = ( item_frames_[ 3 ] == 144 ) ? 160 : 144;
	}
	if ( animation_timer_.counter() % 8 == 0 )
	{
		item_frames_[ 0 ] += 16;
		item_frames_[ 1 ] += 16;
		item_frames_[ 4 ] += 16;
		if ( item_frames_[ 0 ] >= 6 * 16 )
		{
			item_frames_[ 0 ] = 0;
			item_frames_[ 4 ] = 176;
		}
		if ( item_frames_[ 1 ] >= 288 + 48 )
		{
			item_frames_[ 1 ] = 288;
		}
	}
	animation_timer_.update();
}
