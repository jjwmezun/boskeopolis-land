#include <algorithm>
#include "block_system.hpp"
#include "block_type.hpp"
#include "camera.hpp"
#include "collision.hpp"
#include "level_state.hpp"
#include "map_layer_tilemap.hpp"
#include "mezun_math.hpp"
#include "sprite.hpp"

static constexpr int FADE_SPEED = 4;

MapLayerTilemap::MapLayerTilemap( const std::vector<int>& tiles, int map_width, int map_height, bool fade, Unit::Layer layer_position )
:
	MapLayer( layer_position ),
	alpha_ ( 255 ),
	fade_type_ ( ( fade ) ? FadeType::NOT_FADING : FadeType::DONT_FADE ),
	width_ ( map_width ),
	height_ ( map_height ),
	tiles_ ( tiles ),
	blocks_ ()
{};

MapLayerTilemap::~MapLayerTilemap() {};

void MapLayerTilemap::update( LevelState& level_state )
{
	const Camera& camera = level_state.camera();
	if ( camera.changed() )
	{
		blocks_.clear();

		const int first_x = std::max( ( int )( floor( camera.x() / Unit::PIXELS_PER_BLOCK ) - CAMERA_PADDING ), 0 );
		const int first_y = std::max( ( int )( floor( camera.y() / Unit::PIXELS_PER_BLOCK ) - CAMERA_PADDING ), 0 );
		const int last_x = std::min( ( int )( ceil( camera.right() / Unit::PIXELS_PER_BLOCK ) + CAMERA_PADDING ), width_ );
		const int last_y = std::min( ( int )( ceil( camera.screenBottom() / Unit::PIXELS_PER_BLOCK ) + CAMERA_PADDING ), height_ );

		for ( int y = first_y; y < last_y; ++y )
		{
			const int yp = Unit::BlocksToPixels( y );

			for ( int x = first_x; x < last_x; ++x )
			{
				const int i = nOfXY( x, y );
				if ( tiles_.at( i ) > 0 )
				{
					const int xp = Unit::BlocksToPixels( x );
					const int type = tiles_.at( i ) - 1;
					BlockType* block_type = level_state.blocks().getBlockType( type );
					if ( block_type != nullptr )
					{
						blocks_.emplace_back( xp, yp, block_type, i, 0, type );
					}
				}
			}
		}
	}

	int alpha = alpha_;
	switch ( fade_type_ )
	{
		case ( FadeType::NOT_FADING ):
			alpha = std::min( alpha + FADE_SPEED, 255 );
		break;

		case ( FadeType::FADING ):
			alpha = std::max( alpha - FADE_SPEED, 0 );
		break;
	}
	alpha_ = alpha;
};

void MapLayerTilemap::render( const Camera& camera )
{
	for ( auto& b : blocks_ )
	{
		if ( camera.onscreen( b.hitBox() ) )
		{
			switch ( fade_type_ )
			{
				case ( FadeType::DONT_FADE ):
					b.render( camera );
				break;

				case ( FadeType::NOT_FADING ):
				case ( FadeType::FADING ):
					if ( alpha_ == 0 ) return;
					if ( b.hasType() && b.type()->graphics() != nullptr )
					{
						b.type()->graphics()->masterRender
						(
							Unit::SubPixelsToPixels( b.hitBox() ),
							b.type()->graphics()->current_frame_x_,
							b.type()->graphics()->current_frame_y_,
							&camera,
							alpha_
						);
					}
				break;
			}
		}
	}
};

void MapLayerTilemap::interact( Sprite& sprite, LevelState& level_state )
{
	if ( fade_type_ != FadeType::DONT_FADE && sprite.hasType( Sprite::SpriteType::HERO ) )
	{
		for ( auto& b : blocks_ )
		{
			const Collision collision = sprite.testBlockCollision( b );
			if ( collision.collideAny() )
			{
				fade_type_ = FadeType::FADING;
				return;
			}
		}
		fade_type_ = FadeType::NOT_FADING;
	}
};

int MapLayerTilemap::nOfXY( int x, int y ) const
{
	return mezun::nOfXY( x, y, width_ );
};
