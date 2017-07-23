#include "map_layer_image.hpp"
#include "camera.hpp"
#include "main.hpp"
#include "render.hpp"

MapLayerImage::MapLayerImage
(
	std::string&& texture,
	int width,
	int height,
	int offset_x,
	int offset_y,
	double scroll_speed_x,
	double scroll_speed_y,
	int num_o_frames,
	bool repeat_x,
	bool repeat_y,
	int move_speed_x,
	int move_speed_y,
	int animation_speed,
	bool flip,
	Uint8 alpha
)
:
	MapLayer(),
	texture_ ( texture ),
	source_ ( { 0, 0, width, height } ),
	offset_x_ ( offset_x ),
	offset_y_ ( offset_y ),
	scroll_speed_x_ ( scroll_speed_x ),
	scroll_speed_y_ ( scroll_speed_y ),
	repeat_x_ ( repeat_x ),
	repeat_y_ ( repeat_y ),
	move_speed_x_ ( move_speed_x ),
	move_speed_y_ ( move_speed_y ),
	num_o_frames_ ( num_o_frames ),
	movement_position_x_ ( 0 ),
	movement_position_y_ ( 0 ),
	animation_speed_ ( animation_speed ),
	current_frame_ ( Counter( 0, num_o_frames - 1, 0, !flip ) ),
	animation_timer_ ( {} ),
	frame_dir_ ( ( flip ) ? Direction::Vertical::UP : Direction::Vertical::__NULL ),
	alpha_ ( alpha )
{};

void MapLayerImage::update( EventSystem& events, BlockSystem& blocks, const Camera& camera )
{
	if ( Main::nextFrame( 8 / animation_speed_ ) )
	{
		switch( frame_dir_ )
		{
			case ( Direction::Vertical::UP ):
				++current_frame_;

				if ( current_frame_.hitLimit() )
				{
					frame_dir_ = Direction::switchVertical( frame_dir_ );
				}
			break;

			case ( Direction::Vertical::DOWN ):
				--current_frame_;

				if ( current_frame_.hitLimit() )
				{
					frame_dir_ = Direction::switchVertical( frame_dir_ );
				}
			break;

			case ( Direction::Vertical::__NULL ):
				++current_frame_;
			break;
		}
	}
	source_.x = source_.w * current_frame_();

	movement_position_x_ += move_speed_x_;
	movement_position_y_ += move_speed_y_;
};

void MapLayerImage::render( Camera& camera ) const
{
	renderY( camera );
};

void MapLayerImage::renderY( Camera& camera ) const
{
	int dy = offset_y_ - ( (int)( camera.y() * scroll_speed_y_ ) ) + Unit::SubPixelsToPixels( movement_position_y_ );
	int dl = offset_x_ - ( (int)( camera.x() * scroll_speed_x_ ) ) + ( source_.w * current_frame_.value() ) + Unit::SubPixelsToPixels(  movement_position_x_ );

	if ( repeat_y_ )
	{
		while ( dy < -source_.h )
		{
			dy += source_.h;
		}

		while ( dy > 0 )
		{
			dy -= source_.h;
		}
	}

	if ( repeat_x_ )
	{
		while ( dl < -source_.w )
		{
			dl += source_.w;
		}

		while ( dl > 0 )
		{
			dl -= source_.w;
		}
	}

	sdl2::SDLRect dest = { dl, dy, source_.w, source_.h };

	renderX( camera, dest );

	if ( repeat_y_ )
	{
		while ( dest.y < camera.heightPixels() )
		{
			dest.x = dl;
			dest.y += source_.h;
			renderX( camera, dest );
		}
	}
};

void MapLayerImage::renderX( Camera& camera, sdl2::SDLRect& dest ) const
{
	if ( onscreen( dest, camera ) )
	{
		Render::renderObject( texture_, source_, dest );
	}

	if ( repeat_x_ )
	{
		while ( dest.x < camera.widthPixels() )
		{
			dest.x += source_.w;

			if ( onscreen( dest, camera ) )
			{
				Render::renderObject( texture_, source_, dest, SDL_FLIP_NONE, 0.0, alpha_ );
			}
		}
	}
};

bool MapLayerImage::onscreen( const sdl2::SDLRect& r, Camera& camera ) const
{
	return
		r.right() > 0 &&
		r.x < camera.widthPixels() &&
		r.bottom() > 0 &&
		r.y < camera.heightPixels();
};