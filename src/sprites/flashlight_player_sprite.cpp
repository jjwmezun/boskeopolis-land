#include "camera.hpp"
#include "event_system.hpp"
#include "input.hpp"
#include "input_component_player.hpp"
#include "flashlight_player_sprite.hpp"
#include "line.hpp"
#include "mezun_math.hpp"

// Many thanks to Martin Thoma
// @ https://martin-thoma.com/how-to-check-if-two-line-segments-intersect/
// for most o’ this math.

static constexpr int LIGHT_LENGTH = 48;
static constexpr double FLASHLIGHT_SPEED = 0.05;
static constexpr double FLASHLIGHT_HALF_WIDTH = mezun::PI / 11;
static constexpr double FLASHLIGHT_MOVEMENT_LIMIT = mezun::PI / 4;
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
		for ( const Line& box_line : box_lines )
		{
			if ( lineCrossesLine( line, box_line ) && lineCrossesLine( box_line, line ) )
			{
				return true;
			}
		}
	}
	return false;
};

FlashlightPlayerSprite::FlashlightPlayerSprite( int x, int y )
:
	PlayerSprite
	(
		x,
		y,
		1000,
		6000,
		std::unique_ptr<InputComponentPlayer> ( new InputComponentPlayer() ),
		std::make_unique<SpriteGraphics> ( "sprites/flashlight-autumn.png", 0, 0, false, false, 0, false, -1, -2, 2, 4 )
	),
	flashlight_gfx_ ( "sprites/flashlight-autumn.png", 0, 51, false, false, 0.0, false, 0, 0, 0, 0, 255, SDL_BLENDMODE_NONE, { 0, 13 }, true ),
	flash_beam_gfx_ ( "sprites/flashlight-beam.png", 0, 0, false, false, 0.0, false, 0, 0, 0, 0, 128, SDL_BLENDMODE_ADD, { 0, 13 }, true ),
	flashlight_box_ ( x, y, 56, 26 ),
	angle_( 0.0 )
{};

FlashlightPlayerSprite::~FlashlightPlayerSprite() {};

void FlashlightPlayerSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	if ( !events.pause_hero_ )
	{
		resetBopsOnLanding();
		handleRunning();
		handleWalking();
		if ( !input_->right() && !input_->left())
		{
			stopX();
		}
		handleJumpingAndFalling( blocks, events );
		events.on_conveyor_belt_ = false;
		handleLadderBehavior( events );
		adjustJumpSpeed();
		dontDuckWhileSwimming( blocks );
		input_->update();
		handleLookingUp();
		handleCameraMovement( camera );
		handleWaterEnteringAndExiting( lvmap );
		handleDoorBehavior( events );
		handleChasmBehavior( lvmap, events );
		handleDrowningBehavior( health );
		invincibilityFlicker( health );
		boundaries( camera, lvmap );
		camera.adjust( *this, lvmap );
		events.is_sliding_prev_ = events.is_sliding_;
		events.is_sliding_ = false;
		player_gfx_.update( *this, graphics_.get(), &events );
		if ( !on_ladder_ )
		{
			if ( Input::held( Input::Action::MOVE_UP ) )
			{
				angle_ -= FLASHLIGHT_SPEED;
				if ( angle_ < -FLASHLIGHT_MOVEMENT_LIMIT )
				{
					angle_ = -FLASHLIGHT_MOVEMENT_LIMIT;
				}
			}
			else if ( Input::held( Input::Action::MOVE_DOWN ) )
			{
				angle_ += FLASHLIGHT_SPEED;
				if ( angle_ > FLASHLIGHT_MOVEMENT_LIMIT )
				{
					angle_ = FLASHLIGHT_MOVEMENT_LIMIT;
				}
			}
		}
	}
};

void FlashlightPlayerSprite::render( Camera& camera, bool priority )
{
	graphics_->render( Unit::SubPixelsToPixels( hit_box_ ), &camera, priority );

	if ( !on_ladder_ )
	{
		flashlight_box_.x = xPixels() + ( ( direction_x_ == Direction::Horizontal::LEFT ) ? -44 : 1 );
		flashlight_box_.y = yPixels();
		flashlight_gfx_.flip_x_ = flash_beam_gfx_.flip_x_ = direction_x_ == Direction::Horizontal::LEFT;
		flashlight_gfx_.rotation_ = flash_beam_gfx_.rotation_ = ( ( direction_x_ == Direction::Horizontal::LEFT ) ? -1.0 : 1.0 ) * ( 1.0 / mezun::HALF_PI ) * 90.0 * angle_;
		flashlight_gfx_.rotation_center_.x = flash_beam_gfx_.rotation_center_.x = ( direction_x_ == Direction::Horizontal::LEFT ) ? 56 : 0;

		flash_beam_gfx_.render( flashlight_box_, &camera, priority );
		flashlight_gfx_.render( flashlight_box_, &camera, priority );
	}
	
	/*
	const auto lines = getLines();
	for ( const auto& line : lines )
	{
		const auto relative_line = camera.relativeLine( Unit::SubPixelsToPixels( line ) );
		Render::renderLine( relative_line, 6 );
	}
	*/
	
};

int FlashlightPlayerSprite::x2( int center_x, double offset ) const
{
	return Unit::SubPixelsToPixels( center_x ) + ( LIGHT_LENGTH * std::cos( angle() + offset ) );
}

int FlashlightPlayerSprite::y2( int center_y, double offset ) const
{
	return Unit::SubPixelsToPixels( center_y ) + ( LIGHT_LENGTH * std::sin( angle() + offset ) );
}

double FlashlightPlayerSprite::angle() const
{
	return ( direction_x_ == Direction::Horizontal::LEFT ) ? angle_ - amountToChange( angle_ ) : angle_;
}

void FlashlightPlayerSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( them.hasType( SpriteType::MANSION_GHOST ) )
	{
		them.collide_left_ = collideWithFlashLight( them, their_collision );
	}
	playerInteract( my_collision, them, health, events );
}

bool FlashlightPlayerSprite::collideWithFlashLight( const Sprite& them, const Collision& their_collision ) const
{
	const auto lines = getLines();
	return testLineAndBoxCollision( lines[ 0 ], them.hit_box_ )
		|| testLineAndBoxCollision( lines[ 1 ], them.hit_box_ )
		|| testLineAndBoxCollision( lines[ 2 ], them.hit_box_ );
};

std::array<Line, 3> FlashlightPlayerSprite::getLines() const
{
	const Point center_point =
	{
		( ( direction_x_ == Direction::Horizontal::LEFT ) ? centerXSubPixels() + Unit::PixelsToSubPixels( 5 ) : centerXSubPixels() - Unit::PixelsToSubPixels( 6 ) ),
		centerYSubPixels() + Unit::PixelsToSubPixels( 4 )
	};
	return
	{{
		{ center_point, Unit::PixelsToSubPixels( Point{ x2( center_point.x, -FLASHLIGHT_HALF_WIDTH ), y2( center_point.y, -FLASHLIGHT_HALF_WIDTH ) }) },
		{ center_point, Unit::PixelsToSubPixels( Point{ x2( center_point.x, FLASHLIGHT_HALF_WIDTH ), y2( center_point.y, FLASHLIGHT_HALF_WIDTH ) }) },
		{ center_point, Unit::PixelsToSubPixels( Point{ x2( center_point.x ), y2( center_point.y ) }) }
	}};
}