#include "block_system.hpp"
#include "camera.hpp"
#include <cassert>
#include "level_state.hpp"
#include "map_layer_tilemap_image.hpp"
#include "render.hpp"

MapLayerTilemapImage::MapLayerTilemapImage( const std::vector<int>& tiles, int width, int height, int layer_position )
:
	MapLayer( layer_position ),
	tiles_ ( tiles ),
	texture_ ( nullptr ),
	src_ ( 0, 0, Unit::BlocksToPixels( width ), Unit::BlocksToPixels( height ) ),
	width_ ( width ),
	height_ ( height )
{
	dest_ = src_;
};

MapLayerTilemapImage::~MapLayerTilemapImage() {};

void MapLayerTilemapImage::update( LevelState& level_state )
{
	if ( texture_ == nullptr )
	{
		texture_ = Render::createRenderBox( src_.w, src_.h );
		assert( tiles_.size() == width_ * height_ );
		sdl2::SDLRect block_dest = { 0, 0, Unit::BlocksToPixels( 1 ), Unit::BlocksToPixels( 1 ) };
		sdl2::SDLRect tile_src = block_dest;
		Render::setRenderTarget( texture_ );
		Render::clearScreenTransparency();
		for ( int y = 0; y < height_; ++y )
		{
			for ( int x = 0; x < width_; ++x )
			{
				const int i = mezun::nOfXY( x, y, width_ );
				const BlockType* type = level_state.blocks().getBlockType( tiles_[ i ] - 1 );
				if ( type != nullptr && type->graphics() != nullptr )
				{
					block_dest.x = Unit::BlocksToPixels( x );
					block_dest.y = Unit::BlocksToPixels( y );
					tile_src.x = type->graphics()->current_frame_x_;
					tile_src.y = type->graphics()->current_frame_y_;
					Render::renderObject( type->graphics()->texture_, tile_src, block_dest );
				}
			}
		}
		Render::releaseRenderTarget();
		tiles_.clear();
	}
};

void MapLayerTilemapImage::render( const Camera& camera )
{
	dest_.x = -camera.x();
	dest_.y = -camera.y();
	if ( texture_ != nullptr )
	{
		Render::renderObject( texture_, src_, dest_ );
	}
};
