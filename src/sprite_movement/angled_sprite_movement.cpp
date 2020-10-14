#include "collision.hpp"
#include "sprite.hpp"
#include "sprite_graphics.hpp"
#include "angled_sprite_movement.hpp"
#include "test_line_and_box_collision.hpp"

static double findAngleThroughLawOSines( double composite_side, double opposite_side, double known_angle );
static double findSideLengthThroughPythagoreanTheorem( double a, double b );
static Point findBoxCornerByHypotenuseAndAngle( const sdl2::SDLRect& box, double hypotenuse, double angle );

std::array<Line, 4> AngledSpriteMovement::getLinesFromBox( const sdl2::SDLRect& box, double angle )
{
	const double a = ( double )( box.halfWidth() );
	const double b = ( double )( box.halfHeight() );
	const double c = findSideLengthThroughPythagoreanTheorem( a, b );
	const double C = mezun::HALF_PI; // 90°
	const double B = findAngleThroughLawOSines( b, c, C );

	const double top_left_angle = angle - mezun::PI + B;
	const double top_right_angle = angle - B;
	const double bottom_right_angle = angle + B;
	const double bottom_left_angle = angle - mezun::PI - B;

	const Point top_left_point = findBoxCornerByHypotenuseAndAngle( box, c, top_left_angle );
	const Point top_right_point = findBoxCornerByHypotenuseAndAngle( box, c, top_right_angle );
	const Point bottom_right_point = findBoxCornerByHypotenuseAndAngle( box, c, bottom_right_angle );
	const Point bottom_left_point = findBoxCornerByHypotenuseAndAngle( box, c, bottom_left_angle );

	return
	{
		Line{ top_left_point, top_right_point },
		Line{ top_right_point, bottom_right_point },
		Line{ bottom_right_point, bottom_left_point },
		Line{ bottom_left_point, top_left_point }
	};
}

AngledSpriteMovement::AngledSpriteMovement()
:
	SpriteMovement( Type::ANGLED )
{};

const Collision AngledSpriteMovement::testBlockCollision( const Sprite& me, const sdl2::SDLRect& them ) const
{
	const auto collision_lines = getLinesFromBox( me.hit_box_, mezun::convertDegreesToRadians( me.graphics_->rotation_ ) );
	for ( const Line& line : collision_lines )
	{
		if ( mezun::testLineAndBoxCollision( line, them ) )
		{
			return { 1, 1, 1, 1 };
		}
	}
	return { 0, 0, 0, 0 };
};

void AngledSpriteMovement::position( Sprite& sprite ) const
{
};

void AngledSpriteMovement::collideStopXLeft( Sprite& sprite, int overlap ) const
{
	sprite.collide_left_ = ( bool )( overlap );
};

void AngledSpriteMovement::collideStopXRight( Sprite& sprite, int overlap ) const
{
	sprite.collide_right_ = ( bool )( overlap );
};

void AngledSpriteMovement::collideStopYBottom( Sprite& sprite, int overlap ) const
{
	sprite.collide_bottom_ = ( bool )( overlap );
};

void AngledSpriteMovement::collideStopYTop( Sprite& sprite, int overlap ) const
{
	sprite.collide_top_ = ( bool )( overlap );
};

static double findAngleThroughLawOSines( double composite_side, double opposite_side, double known_angle )
{
	return asin( ( sin( known_angle ) / opposite_side ) * composite_side );
};

static double findSideLengthThroughPythagoreanTheorem( double a, double b )
{
	return std::sqrt( ( a * a ) + ( b * b ) );
}

static Point findBoxCornerByHypotenuseAndAngle( const sdl2::SDLRect& box, double hypotenuse, double angle )
{
	return
	{
		box.centerWidth() + ( int )( hypotenuse * std::cos( angle ) ),
		box.centerHeight() + ( int )( hypotenuse * std::sin( angle ) )
	};
}