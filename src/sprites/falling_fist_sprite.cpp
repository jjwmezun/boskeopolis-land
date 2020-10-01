#include "falling_fist_sprite.hpp"
#include "sprite_graphics.hpp"

FallingFistSprite::FallingFistSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/fist.png", 0, 32, false, false, 0, -3, -3, 6, 10 ), x, y, 28, 24, { SpriteType::ENEMY }, 500, 500, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::DOWN, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY, true, true, true, false )
{};

FallingFistSprite::~FallingFistSprite() {};

void FallingFistSprite::customUpdate( LevelState& level_state )
{
	if ( collide_bottom_ ) direction_y_ = Direction::Vertical::DOWN;
	else if ( collide_top_ ) direction_y_ = Direction::Vertical::UP;
	
	switch ( direction_y_ )
	{
		case ( Direction::Vertical::DOWN ):
			top_speed_ = 8000;
			moveDown();
		break;

		case ( Direction::Vertical::UP ):
			top_speed_ = 1500;
			moveUp();
		break;
			
		case ( Direction::Vertical::__NULL ):
			fullStopY();
		break;
	}
	
	hit_box_.x = original_hit_box_.x;
};

void FallingFistSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{};

void FallingFistSprite::render( const Camera& camera ) const
{
	drawCrane( camera );
	graphics_->render( Unit::SubPixelsToPixels( hit_box_ ), &camera );
};

void FallingFistSprite::drawCrane( const Camera& camera ) const
{
	const int crane_y = Unit::SubPixelsToPixels( original_hit_box_.y );
	const int crane_w_change = floor( ( yPixels() - crane_y ) / 16 );
	const sdl2::SDLRect crane_src = { 0, 0, 34, 32 };
	const sdl2::SDLRect crane_dest =
	{
		xPixels() - 3 + ( crane_w_change / 2 ),
		crane_y,
		crane_src.w - crane_w_change,
		yPixels() - crane_y
	};
	Render::renderObject( "sprites/fist.png", crane_src, crane_dest, SDL_FLIP_NONE, 0.0, 255, &camera );
};