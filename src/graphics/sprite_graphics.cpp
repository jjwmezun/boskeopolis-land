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
	int x_adjustment,
	int y_adjustment,
	int w_adjustment,
	int h_adjustment,
	Uint8 alpha,
	SDL_BlendMode blend_mode,
	Point rotation_center,
	bool default_rotation_point
)
:
	flip_x_ ( flip_x ),
	flip_y_ ( flip_y ),
	visible_ ( true ),
	default_rotation_point_ ( default_rotation_point ),
	alpha_ ( alpha ),
	blend_mode_ ( blend_mode ),
	prev_frame_x_ ( 0 ),
	prev_frame_y_ ( 0 ),
	current_frame_x_ ( current_frame_x ),
	current_frame_y_ ( current_frame_y ),
	x_adjustment_ ( x_adjustment ),
	y_adjustment_ ( y_adjustment ),
	w_adjustment_ ( w_adjustment ),
	h_adjustment_ ( h_adjustment ),
	rotation_ ( rotation ),
	rotation_center_ ( rotation_center ),
	texture_ ( texture )
{};

SpriteGraphics::~SpriteGraphics() {};

void SpriteGraphics::update() {};

void SpriteGraphics::update( Sprite& sprite )
{
	update();
};

void SpriteGraphics::update( const EventSystem& events )
{
	update();
};

void SpriteGraphics::render( const sdl2::SDLRect& bound_box, const Camera* camera ) const
{
	masterRender( bound_box, current_frame_x_, current_frame_y_, camera, alpha_ );
};

void SpriteGraphics::masterRender( const sdl2::SDLRect& bound_box, int current_frame_x, int current_frame_y, const Camera* camera, Uint8 alpha, SDL_Texture* texture ) const
{
	if ( visible_ )
	{
		sdl2::SDLRect dest = adjustBoundBox( bound_box );
		sdl2::SDLRect source =
		{
			current_frame_x,
			current_frame_y,
			dest.w,
			dest.h
		};
		Render::renderObject
		(
			texture_,
			source,
			dest,
			flip_x_,
			flip_y_,
			rotation_,
			alpha,
			camera,
			blend_mode_,
			nullptr,
			( default_rotation_point_ ) ? &rotation_center_ : nullptr
		);
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

void SpriteGraphics::rotate( Direction::Clockwise dir, int amount )
{
	switch ( dir )
	{
		case ( Direction::Clockwise::CLOCKWISE ):
		{
			rotation_ += amount;
		}
		break;

		case ( Direction::Clockwise::COUNTERCLOCKWISE ):
		{
			rotation_ -= amount;
		}
		break;
	}
};
