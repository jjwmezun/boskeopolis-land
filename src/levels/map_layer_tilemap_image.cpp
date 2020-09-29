#include "block_system.hpp"
#include "camera.hpp"
#include <cassert>
#include "level_state.hpp"
#include "map_layer_tilemap_image.hpp"
#include "render.hpp"

MapLayerTilemapImage::MapLayerTilemapImage( const std::vector<int>& tiles, int width, int height, Unit::Layer layer_position )
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
		level_state.blocks().renderTiles( tiles_, width_, height_ );
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
