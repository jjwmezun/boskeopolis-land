#include "camera.hpp"
#include "sprite_graphics.hpp"

SpriteGraphics::SpriteGraphics
(
	std::string&& texture,
	int current_frame_x,
	int current_frame_y,
	bool flip_x,
	bool flip_y,
	double rotation,
	bool priority,
	int x_adjustment,
	int y_adjustment,
	int w_adjustment,
	int h_adjustment,
	Uint8 alpha
)
:
	flip_x_ ( flip_x ),
	flip_y_ ( flip_y ),
	priority_ ( priority ),
	visible_ ( true ),
	alpha_ ( alpha ),
	current_frame_x_ ( current_frame_x ),
	current_frame_y_ ( current_frame_y ),
	x_adjustment_ ( x_adjustment ),
	y_adjustment_ ( y_adjustment ),
	w_adjustment_ ( w_adjustment ),
	h_adjustment_ ( h_adjustment ),
	rotation_ ( rotation ),
	texture_ ( texture ),
	prev_frame_x_ ( 0 ),
	prev_frame_y_ ( 0 )
{};

SpriteGraphics::~SpriteGraphics() {};

void SpriteGraphics::update( Sprite& sprite )
{
	update();
};

void SpriteGraphics::update()
{
};

void SpriteGraphics::renderAnyPriority( const sdl2::SDLRect& bound_box, const Camera* camera ) const
{
	render( bound_box, camera, priority_ );
};

void SpriteGraphics::render( const sdl2::SDLRect& bound_box, const Camera* camera, bool priority ) const
{
	masterRender( bound_box, current_frame_x_, current_frame_y_, camera, priority, alpha_ );
};

void SpriteGraphics::masterRender( const sdl2::SDLRect& bound_box, int current_frame_x, int current_frame_y, const Camera* camera, bool priority, Uint8 alpha ) const
{
	if ( visible_ && ( ( priority && priority_ ) || ( !priority && !priority_ ) ) )
	{
		sdl2::SDLRect dest = adjustBoundBox( bound_box );

		sdl2::SDLRect source =
		{
			current_frame_x,
			current_frame_y,
			dest.w,
			dest.h
		};

		Render::renderObject( texture_, source, dest, flip_x_, flip_y_, rotation_, alpha, camera );
	}
};

sdl2::SDLRect SpriteGraphics::adjustBoundBox( const sdl2::SDLRect& r ) const
{
	return
	{
		r.x + x_adjustment_,
		r.y + y_adjustment_,
		r.w + w_adjustment_,
		r.h + h_adjustment_
	};
};
