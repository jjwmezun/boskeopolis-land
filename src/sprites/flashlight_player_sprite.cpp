#include "camera.hpp"
#include "event_system.hpp"
#include "input.hpp"
#include "flashlight_player_sprite.hpp"
#include "line.hpp"
#include "mezun_math.hpp"

// Many thanks to Martin Thoma
// @ https://martin-thoma.com/how-to-check-if-two-line-segments-intersect/
// for most o’ this math.

static constexpr int LIGHT_LENGTH = 32;
static constexpr double FLASHLIGHT_SPEED = 0.01;
static constexpr double EPSILON = 0.000001;
static constexpr double amountToChange( double angle )
{
	return ( angle + mezun::HALF_PI ) * 2;
};

static constexpr double getCrossProduct( Point a, Point b )
{
	return a.x * b.y - b.x * a.y;
};

static constexpr bool isPointOnLine( const Line& line, Point point )
{
	const Line a = { { 0, 0 }, { line.p2.x - line.p1.x, line.p2.y - line.p1.y } };
	const Point b = { point.x - line.p1.x, point.y - line.p1.y };
	double c = getCrossProduct( a.p2, b );
	return std::abs( c ) < EPSILON;
};

static constexpr bool isPointRightOfLine( const Line& line, Point point )
{
	const Line a = { { 0, 0 }, { line.p2.x - line.p1.x, line.p2.y - line.p1.y } };
	const Point b = { point.x - line.p1.x, point.y - line.p1.y };
	double c = getCrossProduct( a.p2, b );
	return c < 0;
};

static constexpr bool lineCrossesLine( const Line& l1, const Line& l2 )
{
	return isPointOnLine( l1, l2.p1 )
		|| isPointOnLine( l1, l2.p2 )
		|| (
				isPointRightOfLine( l1, l2.p1 )
				^ isPointRightOfLine( l1, l2.p2 )
			);
};

static constexpr bool testLineAndBoxCollision( const Line& line, const sdl2::SDLRect& box )
{
	const sdl2::SDLRect line_box = line.getBox();
	if ( box.testSimpleCollision( line_box ) )
	{
		const auto box_lines = Line::getLinesFromBox( box );
		int i = 0;
		for ( const Line& box_line : box_lines )
		{
			if ( lineCrossesLine( line, box_line ) && lineCrossesLine( box_line, line ) )
			{
				return true;
			}
			++i;
		}
	}
	return false;
};

FlashlightPlayerSprite::FlashlightPlayerSprite( int x, int y )
:
	PlayerSprite( x, y ),
	angle_()
{};

FlashlightPlayerSprite::~FlashlightPlayerSprite() {};

void FlashlightPlayerSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	if ( !events.pause_hero_ )
	{
		heroActions( camera, lvmap, events, sprites, blocks, health );
		player_gfx_.update( *this, graphics_.get(), &events );
		if ( Input::held( Input::Action::MOVE_UP ) )
		{
			angle_ -= FLASHLIGHT_SPEED;
			if ( angle_ < -( mezun::PI / 4 ) )
			{
				angle_ = -( mezun::PI / 4 );
			}
		}
		else if ( Input::held( Input::Action::MOVE_DOWN ) )
		{
			angle_ += FLASHLIGHT_SPEED;
			if ( angle_ > ( mezun::PI / 4 ) )
			{
				angle_ = ( mezun::PI / 4 );
			}
		}
	}
};

void FlashlightPlayerSprite::render( Camera& camera, bool priority )
{
	graphics_->render( Unit::SubPixelsToPixels( hit_box_ ), &camera, priority );
	Render::renderLine( camera.relativeX( centerXPixels() ), camera.relativeY( centerYPixels() ), camera.relativeX( x2( -mezun::PI / 16 ) ), camera.relativeY( y2( -mezun::PI / 16 ) ) );
	Render::renderLine( camera.relativeX( centerXPixels() ), camera.relativeY( centerYPixels() ), camera.relativeX( x2( mezun::PI / 16 ) ), camera.relativeY( y2( mezun::PI / 16 ) ) );
};

int FlashlightPlayerSprite::x2( double offset ) const
{
	return centerXPixels() + ( LIGHT_LENGTH * std::cos( angle() + offset ) );
}

int FlashlightPlayerSprite::y2( double offset ) const
{
	return centerYPixels() + ( LIGHT_LENGTH * std::sin( angle() + offset ) );
}

double FlashlightPlayerSprite::angle() const
{
	return ( direction_x_ == Direction::Horizontal::LEFT ) ? angle_ - amountToChange( angle_ ) : angle_;
}

void FlashlightPlayerSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( them.hasType( SpriteType::MANSION_GHOST ) )
	{
		if ( collideWithFlashLight( them, their_collision ) )
		{
			them.collide_left_ = true;
		}
	}
}

bool FlashlightPlayerSprite::collideWithFlashLight( const Sprite& them, const Collision& their_collision ) const
{
	const Point center_point = { centerXSubPixels(), centerYSubPixels() };
	const Line lines[ 2 ] =
	{
		{ center_point, Unit::PixelsToSubPixels( Point{ x2( -mezun::PI / 16 ), y2( -mezun::PI / 16 ) }) },
		{ center_point, Unit::PixelsToSubPixels( Point{ x2( mezun::PI / 16 ), y2( mezun::PI / 16 ) }) }
	};
	return testLineAndBoxCollision( lines[ 0 ], them.hit_box_ ) || testLineAndBoxCollision( lines[ 1 ], them.hit_box_ );
};