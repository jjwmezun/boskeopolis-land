
#include "rope_graphics.h"
#include "sprite.h"

constexpr std::pair<int, int> RopeGraphics::frame_body_;
constexpr std::pair<int, int> RopeGraphics::frame_end_;

RopeGraphics::RopeGraphics()
:
	SpriteGraphics ( "tilesets/woods.png", 112, 16 )
{};

RopeGraphics::~RopeGraphics() {};

void RopeGraphics::update( Sprite& sprite )
{
};

void RopeGraphics::render( Graphics& graphics, const sdl2::SDLRect& bound_box, const Camera* camera, bool priority, Uint8 alpha ) const
{
	const int LAST_BLOCK = bound_box.h - 8;

	for ( int i = 0; i < LAST_BLOCK; i += 8 )
	{
		const sdl2::SDLRect dest =
		{
			bound_box.x,
			bound_box.y + i,
			8,
			8
		};

		masterRender( graphics, dest, std::get<X_INDEX> ( frame_body_ ), std::get<Y_INDEX> ( frame_body_ ), camera, priority );
	}

	const sdl2::SDLRect dest =
	{
		bound_box.x,
		LAST_BLOCK,
		8,
		8
	};

	masterRender( graphics, dest, std::get<X_INDEX> ( frame_end_ ), std::get<Y_INDEX> ( frame_end_ ), camera, priority );
};