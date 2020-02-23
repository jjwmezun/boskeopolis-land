#include "camera.hpp"
#include "event_system.hpp"
#include "input.hpp"
#include "input_component_player.hpp"
#include "inventory.hpp"
#include "flashlight_player_sprite.hpp"
#include "level_state.hpp"
#include "line.hpp"
#include "main.hpp"
#include "mansion_ghost_sprite.hpp"
#include "mezun_math.hpp"
#include "test_line_and_box_collision.hpp"

// Many thanks to Martin Thoma
// @ https://martin-thoma.com/how-to-check-if-two-line-segments-intersect/
// for most o’ this math.

static constexpr int LIGHT_LENGTH = 48;
static constexpr double FLASHLIGHT_SPEED = 0.05;
static constexpr double FLASHLIGHT_HALF_WIDTH = mezun::PI / 11;
static constexpr double FLASHLIGHT_MOVEMENT_LIMIT = mezun::PI / 4;

static constexpr double amountToChange( double angle );

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

void FlashlightPlayerSprite::customUpdate( LevelState& level_state )
{
	Camera& camera = level_state.camera();
	BlockSystem& blocks = level_state.blocks();
	EventSystem& events = level_state.events();
	Map& lvmap = level_state.currentMap();
	Health& health = level_state.health();
	resetBopsOnLanding();
	handleRunning();
	handleWalking();
	if ( !input_->right() && !input_->left())
	{
		stopX();
	}
	handleJumpingAndFalling( blocks, events );
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

	if ( MansionGhostSprite::last_ghost_death_frame_ != -1 && Main::stateFrame() - MansionGhostSprite::last_ghost_death_frame_ > 30 )
	{
		Inventory::clearGhostKills();
		MansionGhostSprite::last_ghost_death_frame_ = -1;
	}
};

void FlashlightPlayerSprite::render( Camera& camera, bool priority )
{
	graphics_->render( Unit::SubPixelsToPixels( hit_box_ ), &camera, priority );

	if ( !on_ladder_ && !is_dead_ )
	{
		flashlight_box_.x = xPixels() + ( ( direction_x_ == Direction::Horizontal::LEFT ) ? -44 : 1 );
		flashlight_box_.y = yPixels();
		flashlight_gfx_.flip_x_ = flash_beam_gfx_.flip_x_ = direction_x_ == Direction::Horizontal::LEFT;
		flashlight_gfx_.rotation_ = flash_beam_gfx_.rotation_ = ( ( direction_x_ == Direction::Horizontal::LEFT ) ? -1.0 : 1.0 ) * ( 1.0 / mezun::HALF_PI ) * 90.0 * angle_;
		flashlight_gfx_.rotation_center_.x = flash_beam_gfx_.rotation_center_.x = ( direction_x_ == Direction::Horizontal::LEFT ) ? 56 : 0;

		flash_beam_gfx_.render( flashlight_box_, &camera, priority );
		flashlight_gfx_.render( flashlight_box_, &camera, priority );
	}
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

void FlashlightPlayerSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( them.hasType( SpriteType::MANSION_GHOST ) )
	{
		them.collide_left_ = collideWithFlashLight( them, their_collision );
	}
	playerInteract( my_collision, them, level_state.health(), level_state.events() );
}

bool FlashlightPlayerSprite::collideWithFlashLight( const Sprite& them, const Collision& their_collision ) const
{
	const auto lines = getLines();
	return mezun::testLineAndBoxCollision( lines[ 0 ], them.hit_box_ )
		|| mezun::testLineAndBoxCollision( lines[ 1 ], them.hit_box_ )
		|| mezun::testLineAndBoxCollision( lines[ 2 ], them.hit_box_ );
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
};

static constexpr double amountToChange( double angle )
{
	return ( angle + mezun::HALF_PI ) * 2;
};