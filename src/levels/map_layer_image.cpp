#include "map_layer_image.hpp"
#include "camera.hpp"
#include "level_state.hpp"
#include "main.hpp"
#include "map.hpp"
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
	int repeat_x,
	int repeat_y,
	int move_speed_x,
	int move_speed_y,
	int animation_speed,
	bool flip,
	Uint8 alpha,
	SDL_BlendMode blend_mode,
	Unit::Layer layer_position
)
:
	MapLayer( layer_position ),
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
	alpha_ ( alpha ),
	blend_mode_ ( blend_mode )
{};

MapLayerImage::~MapLayerImage() {};

void MapLayerImage::update( LevelState& level_state )
{
	move( level_state.currentMap().widthPixels(), level_state.camera().rect() );
};

void MapLayerImage::move( int width, const sdl2::SDLRect& container )
{
	if ( Main::nextStateFrame( animation_speed_ ) )
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

	// To make scrolling nonrepeating BGs repeat o'er the whole map ( used in "Playing Railroad" ) & to prevent unlikely but theoretical problems
	// o' movement_position flipping to the max value if it goes too low, causing the BG to jerk unevenly, if BG goes far 'nough left, push it forward past the right edge o' map in BG-width increments ( so that BG tiling & scrolling stays smooth ).
	// So far this isn't needed for rightward scrolling, since no backgrounds scroll rightward yet.
	if ( movement_position_x_ < -Unit::PixelsToSubPixels( width ) - Unit::PixelsToSubPixels( offset_x_ ) )
	{
		/*
		while ( movement_position_x_ < Unit::PixelsToSubPixels( lvmap.widthPixels() ) )
		{
			movement_position_x_ += ( Unit::PixelsToSubPixels( source_.w ) * ( ( repeat_x_ == REPEAT_INFINITE ) ? 1 : ( repeat_x_ + 1 ) ) );
		}
		*/

		// Size o' whole BG ( including all repeats ).
		const int amount_to_push_forward = ( Unit::PixelsToSubPixels( source_.w ) * ( ( repeat_x_ == REPEAT_INFINITE ) ? 1 : ( repeat_x_ + 1 ) ) );
		const int times_to_push_forward = ceil( ( Unit::PixelsToSubPixels( width ) - movement_position_x_ ) / amount_to_push_forward );
		movement_position_x_ += amount_to_push_forward * times_to_push_forward;
	}
};

void MapLayerImage::render( const Camera& camera )
{
	render( camera.rect() );
};

void MapLayerImage::render( const sdl2::SDLRect& container )
{
	renderY( container );
};

void MapLayerImage::renderY( const sdl2::SDLRect& container ) const
{
	int dy = offset_y_ - ( (int)( container.y * scroll_speed_y_ ) ) + Unit::SubPixelsToPixels( movement_position_y_ );
	int dl = offset_x_ - ( (int)( container.x * scroll_speed_x_ ) ) + Unit::SubPixelsToPixels( movement_position_x_ );// + ( source_.w * current_frame_.value() ); // Quite certain commented out part has no reason to be here.

	// If repeat is infinite, don't bother with parts o' background way past screen.
	// Logically, the start o' repeating BG should be the edge o' the screen ( 0 ) or somewhere 'tween that & negative the height o' the BG,
	// based on how its offset, scrolled, & such...
	// ( any lower &, logically, we could nudge it up by its height & still not be beyond the edge ).
	// Basically, we want to have the BG start as forward as possible while keeping the offset & without going past 0 ( & thereby leaving a hole ).
	// Since we do care 'bout where the BG is for noninfinite repeats, we skip this for those.
	if ( repeat_y_ == REPEAT_INFINITE )
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

	if ( repeat_x_ == REPEAT_INFINITE )
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

	// E'en if not repeating, we still want to draw the BG a'least once.
	renderX( container, dest );

	if ( repeat_y_ > 0 )
	{
		// Draw till we reach the end o' repeats ( which'll ne'er happen if it's infinite, 'course )
		// or till we go past the end edge o' the screen ( no point in starting drawing beyond where the player can see ).
		int i = 0;
		while ( dest.y < container.h && ( repeat_y_ == REPEAT_INFINITE || i < repeat_y_ ) )
		{
			dest.x = dl;
			dest.y += source_.h;
			renderX( container, dest );
			++i;
		}
	}
};

void MapLayerImage::renderX( const sdl2::SDLRect& container, sdl2::SDLRect& dest ) const
{
	if ( onscreen( dest, container ) )
	{
		renderFinal( dest );
	}

	if ( repeat_x_ > 0 )
	{
		int i = 0;
		while ( dest.x < container.w && ( repeat_x_ == REPEAT_INFINITE || i < repeat_x_ ) )
		{
			dest.x += source_.w;

			if ( onscreen( dest, container ) )
			{
				renderFinal( dest );
			}
			++i;
		}
	}
};

void MapLayerImage::renderFinal( sdl2::SDLRect& dest ) const
{
	Render::renderObject( texture_, source_, dest, SDL_FLIP_NONE, 0.0, alpha_, nullptr, blend_mode_ );
};
