#include "block.hpp"
#include "block_system.hpp"
#include "camera.hpp"
#include "collision.hpp"
#include "health.hpp"
#include "level_state.hpp"
#include "map.hpp"
#include "render.hpp"
#include "sprite.hpp"
#include "sprite_graphics.hpp"
#include "input.hpp"
#include "unit.hpp"

const SpriteMovement Sprite::floating_ {};
const GroundedSpriteMovement Sprite::grounded_ {};
const FlutteringSpriteMovement Sprite::fluttering_ {};
const SwimmingSpriteMovement Sprite::swimming_ {};
const AngledSpriteMovement Sprite::angled_ {};
const StuckSpriteMovement Sprite::stuck_movement_ {};

double Sprite::traction_ = TRACTION_NORMAL;
int Sprite::resistance_x_ = RESISTANCE_X_NORMAL;
int Sprite::gravity_start_speed_ = GRAVITY_START_SPEED_NORMAL;
int Sprite::gravity_top_speed_ = GRAVITY_TOP_SPEED_NORMAL;

Sprite::Sprite
(
	std::unique_ptr<SpriteGraphics> graphics,
	int x,
	int y,
	int width,
	int height,
	std::vector<SpriteType> type,
	int start_speed,
	int top_speed,
	int jump_start_speed,
	int jump_top_speed,
	Direction::Horizontal direction_x,
	Direction::Vertical direction_y,
	std::unique_ptr<SpriteComponent> component,
	SpriteMovement::Type physics_state,
	CameraMovement camera_movement,
	bool despawn_when_dead,
	bool block_interact,
	bool sprite_interact,
	double bounce,
	int map_id,
	Unit::Layer layer
)
:
	Object( x, y, width, height ),
	original_hit_box_ ( { Unit::PixelsToSubPixels( x ), Unit::PixelsToSubPixels( y ), Unit::PixelsToSubPixels( width ), Unit::PixelsToSubPixels( height ) } ),
	graphics_ ( std::move( graphics ) ),
	types_ ( type ),
	base_jump_start_speed_ ( jump_start_speed ),
	base_jump_top_speed_ ( jump_top_speed ),
	jump_start_speed_ ( jump_start_speed ),
	jump_top_speed_normal_ ( jump_top_speed ),
	jump_top_speed_        ( jump_top_speed ),
	start_speed_walk_ ( start_speed ),
	start_speed_      ( start_speed ),
	start_speed_run_  ( start_speed ),
	// These algorithms are just set so that normal gravity is 1x & 650 gravity is 1.5x, with values in between getting percentages in between.
	top_speed_walk_ ( ( int )( std::floor( ( double )( top_speed ) * ( 1.0 + ( ( double )( GRAVITY_TOP_SPEED_NORMAL - gravity_top_speed_ ) * ( 1.0 / 6700.0 ) ) ) ) ) ),
	top_speed_      ( ( int )( std::floor( ( double )( top_speed ) * ( 1.0 + ( ( double )( GRAVITY_TOP_SPEED_NORMAL - gravity_top_speed_ ) * ( 1.0 / 6700.0 ) ) ) ) ) ),
	top_speed_run_  ( ( int )( std::floor( ( double )( top_speed ) * ( 1.0 + ( ( double )( GRAVITY_TOP_SPEED_NORMAL - gravity_top_speed_ ) * ( 1.0 / 6700.0 ) ) ) ) ) * 2 ),
	direction_x_ ( direction_x ),
	direction_x_orig_ ( direction_x ),
	direction_y_ ( direction_y ),
	direction_y_orig_ ( direction_y ),
	component_ ( std::move( component ) ),
	movement_ ( getMovement( physics_state ) ),
	camera_movement_ ( camera_movement ),
	despawn_when_dead_ ( despawn_when_dead ),
	block_interact_ ( block_interact ), 
	sprite_interact_ ( sprite_interact ),
	bounce_ ( bounce ),
	on_ground_padding_ (),
	direction_ ( Direction::Simple::__NULL ),
	top_speed_downward_ ( 0 ),
	top_speed_upward_ ( 0 ),
	map_id_ ( map_id ),
	vx_ ( 0 ),
	vy_ ( 0 ),
	on_ground_ ( false ),
	on_slope_ ( Direction::Horizontal::__NULL ),
	x_prev_ ( -123456789 ),
	y_prev_ ( -123456789 ),
	jump_lock_ ( false ),
	in_water_ ( false ),
	acceleration_x_ ( 0 ),
	acceleration_y_ ( 0 ),
	death_timer_ (),
	bounce_height_ ( 0 ),
	layer_ ( layer ),
	gravity_modifier_ ( 1.0 ),
	fall_start_speed_ ( gravity_start_speed_ ),
	fall_top_speed_ ( gravity_top_speed_ ),
	sprite_interact_from_this_to_others_only_ ( false )
{};

Sprite::~Sprite() {};

void Sprite::setGravityModifier( double gravity )
{
	if ( gravity > gravity_modifier_ + 0.000001 || gravity < gravity_modifier_ - 0.000001 )
	{
		gravity_modifier_ = gravity;
		jump_start_speed_ = ( int )( std::floor( ( double )( base_jump_start_speed_ ) * gravity ) );
		jump_top_speed_normal_ = jump_top_speed_ = ( int )( std::floor( ( double )( base_jump_top_speed_ ) * gravity ) );
		fall_start_speed_ = ( int )( std::floor( ( double )( gravity_start_speed_ ) * gravity ) );
		fall_top_speed_ = ( int )( std::floor( ( double )( gravity_top_speed_ ) * gravity ) );
		top_speed_upward_ = 0;
		if ( graphics_ )
		{
			graphics_->flip_y_ = isUpsideDown();
		}
	}
};

void Sprite::setGravity( int gravity )
{
	gravity_top_speed_ = gravity;
	gravity_start_speed_ = ( int )( std::floor( ( double )( gravity ) / 4000.0 * ( double )( GRAVITY_START_SPEED_NORMAL ) ) );

	// Basically set so traction is 1.2 with normal gravity & 1.0005 for 650 with middle values getting middle outcomes.
	traction_ += ( ( double )( GRAVITY_TOP_SPEED_NORMAL - gravity ) * ( ( 1.0005 - 1.2 ) / 3350.0 ) );
};

bool Sprite::fellInBottomlessPit( const Map& lvmap ) const
{
	return topSubPixels() > Unit::PixelsToSubPixels( lvmap.heightPixels() );
};

void Sprite::update( LevelState& level_state )
{
	if ( !isDead() )
	{
		if ( component_ )
		{
			component_->update( *this, *graphics_ );
		}
		customUpdate( level_state );
	}
	else
	{
		if ( !dead_no_animation_ )
		{
			deathAction( level_state );
		}
		else
		{
			//changeMovement( SpriteMovement::Type::FLOATING );
			acceleration_x_ = 0;
			//acceleration_y_ = 0;
			vx_ = 0;
			//vy_ = 0;

			if ( death_timer_.done() )
			{
				death_finished_ = true;
			}

			if ( !death_timer_.on() )
			{
				death_timer_.start();
			}
			else
			{
				death_timer_.update();
			}
		}
	}

	position();

	if ( graphics_ )
	{
		graphics_->update( *this );
	}

	on_slope_ = Direction::Horizontal::__NULL;
};

void Sprite::render( Camera& camera, bool priority )
{
	if ( graphics_ != nullptr )
	{
		graphics_->render( Unit::SubPixelsToPixels( hit_box_ ), &camera, priority );
	}
};

void Sprite::render( const Camera& camera ) const
{
	if ( graphics_ != nullptr )
	{
		graphics_->render( Unit::SubPixelsToPixels( hit_box_ ), &camera, true );
	}
};

void Sprite::renderSuperPriority( Camera& camera )
{
	// Do nothing.
};

void Sprite::renderWithHitbox( Camera& camera, bool priority )
{
	if ( graphics_ != nullptr )
	{
		if ( priority == graphics_->priority_ )
		{
			drawHitBox( camera );
		}
		graphics_->render( Unit::SubPixelsToPixels( hit_box_ ), &camera, priority );
	}
};

void Sprite::moveLeft()
{
	if ( movement_ != nullptr )
	{
		movement_->moveLeft( *this );
	}
};

void Sprite::moveRight()
{
	if ( movement_ != nullptr )
	{
		movement_->moveRight( *this );
	}
};

void Sprite::stopX()
{
	acceleration_x_ = 0;
};

void Sprite::stopY()
{
	acceleration_y_ = 0;
};

void Sprite::fullStopX()
{
	acceleration_x_ = 0;
	vx_ = 0;
};

void Sprite::fullStopY()
{
	acceleration_y_ = 0;
	vy_ = 0;
};

void Sprite::stopDucking()
{
	acceleration_x_ = 0;
};

void Sprite::positionX()
{
	vx_ += acceleration_x_;


	if ( on_slope_ == Direction::Horizontal::RIGHT && vx_ > top_speed_ * 2 )
	{
		vx_ = top_speed_ * 2;
	}
	else if ( vx_ > top_speed_ )
	{
		vx_ = top_speed_;
	}
	else if ( on_slope_ == Direction::Horizontal::LEFT && vx_ < -top_speed_ * 2 )
	{
		vx_ = -top_speed_ * 2;
	}
	else if ( vx_ < -top_speed_ )
	{
		vx_ = -top_speed_;
	}

	vx_ += resistance_x_;

	hit_box_.x += vx_;
};

void Sprite::positionY()
{
	vy_ += acceleration_y_;

	if ( isUpsideDown() )
	{
		if ( vy_ < top_speed_downward_ )
		{
			vy_ = top_speed_downward_;
		}
	}
	else
	{
		if ( vy_ > top_speed_downward_ )
		{
			vy_ = top_speed_downward_;
		}
	}

	if ( isUpsideDown() )
	{
		if ( vy_ > -top_speed_upward_ )
		{
			vy_ = -top_speed_upward_;
		}
	}
	else
	{
		if ( vy_ < -top_speed_upward_ )
		{
			vy_ = -top_speed_upward_;
		}
	}
	hit_box_.y += vy_;
};

void Sprite::position()
{
	x_prev_ = hit_box_.x;
	y_prev_ = hit_box_.y;

	if ( movement_ != nullptr )
		movement_->position( *this );

	positionX();
	positionY();

	if ( acceleration_x_ != 0 || acceleration_y_ != 0 )
		is_moving_ = true;
	else
		is_moving_ = false;

	collide_top_prev_ = collide_top_;
	collide_bottom_prev_ = collide_bottom_;
	collide_top_ = false;
	collide_bottom_ = false;
	collide_left_ = false;
	collide_right_ = false;
};

void Sprite::boundaries( Camera& camera, Map& lvmap )
{
	if ( lvmap.camera_type_ != Camera::Type::SCROLL_LOCK )
	{
		if ( lvmap.loop_sides_ )
		{
			if ( hit_box_.x + hit_box_.w < Unit::PixelsToSubPixels( camera.x() ) )
			{
				hit_box_.x = Unit::PixelsToSubPixels( camera.right() );
			}
			if ( hit_box_.x > Unit::PixelsToSubPixels( camera.right() ) )
			{
				hit_box_.x = Unit::PixelsToSubPixels( camera.x() ) - hit_box_.w;
			}
		}
		else
		{
			containCameraX( camera );
		}

		if ( hit_box_.y < -hit_box_.h )
		{
			hit_box_.y = -hit_box_.h;
		}
	}
};

void Sprite::collideStopXLeft( int overlap )
{
	if ( movement_ != nullptr )
	{
		movement_->collideStopXLeft( *this, overlap );
	}
};

void Sprite::collideStopXRight( int overlap )
{
	if ( movement_ != nullptr )
	{
		movement_->collideStopXRight( *this, overlap );
	}
};

void Sprite::collideStopYBottom( int overlap )
{
	if ( movement_ != nullptr )
	{
		movement_->collideStopYBottom( *this, overlap );
	}
};

void Sprite::collideStopYTop( int overlap )
{
	if ( movement_ != nullptr )
	{
		movement_->collideStopYTop( *this, overlap );
	}
};

bool Sprite::onGround() const
{
	return on_ground_;
};

bool Sprite::isJumping() const
{
	return is_jumping_;
};

bool Sprite::isJumpingPrev() const
{
	return is_jumping_prev_;
};

void Sprite::jump()
{
	if ( movement_ != nullptr )
		movement_->jump( *this );
};

void Sprite::bounce( int amount )
{
	if ( movement_ != nullptr )
		movement_->bounce( *this, amount );
};

void Sprite::slowFall()
{
	fall_start_speed_ = ( int )( std::floor( ( double )( gravity_start_speed_ ) * gravity_modifier_ * 0.75 ) );
	fall_top_speed_ = ( int )( std::floor( ( double )( gravity_top_speed_ ) * gravity_modifier_ * 0.85 ) );
};

void Sprite::fastFall()
{
	fall_start_speed_ = ( int )( std::floor( ( double )( gravity_start_speed_ ) * gravity_modifier_ ) );
	fall_top_speed_ = ( int )( std::floor( ( double )( gravity_top_speed_ ) * gravity_modifier_ ) );
};

bool Sprite::isRunning() const
{
	return is_running_;
};

void Sprite::run()
{
	is_running_ = true;
	top_speed_ = top_speed_run_;
	start_speed_ = start_speed_run_;
};

void Sprite::stopRunning()
{
	is_running_ = false;
	top_speed_ = top_speed_walk_;
	start_speed_ = start_speed_walk_;
};

void Sprite::interact( Sprite& them, LevelState& level_state )
{
	Collision my_collision = testCollision( them );
	Collision their_collision = them.testCollision( *this );
	customInteract( my_collision, their_collision, them, level_state );
};

bool Sprite::canJump() const
{
	return onGroundPadding();
};

bool Sprite::isDucking() const
{
	return is_ducking_;
};

Direction::Horizontal Sprite::directionX() const
{
	return direction_x_;
};

Direction::Vertical Sprite::directionY() const
{
	return direction_y_;
};

bool Sprite::onGroundPrev() const
{
	return on_ground_prev_;
};

bool Sprite::isMoving() const
{
	return is_moving_;
};

void Sprite::grabLadder()
{
	if ( !isDucking() )
		on_ladder_ = true;
};

void Sprite::releaseLadder()
{
	on_ladder_ = false;
};

void Sprite::moveUp()
{
	if ( movement_ != nullptr )
	{
		movement_->moveUp( *this );
	}
};

void Sprite::moveDown()
{
	if ( movement_ != nullptr )
	{
		movement_->moveDown( *this );
	}
};

void Sprite::kill()
{
	is_dead_ = true;
};

bool Sprite::isDead() const
{
	return is_dead_;
};

void Sprite::setPosition( int x, int y )
{
	hit_box_.x = Unit::PixelsToSubPixels( x );
	hit_box_.y = Unit::PixelsToSubPixels( y );
};

bool Sprite::hasType( Sprite::SpriteType type ) const
{
	for ( auto& t : types_ )
	{
		if ( type == t ) return true;
	}

	return false;
};

void Sprite::collideStopAny( const Collision& collision )
{
	if ( movement_ != nullptr )
	{
		movement_->collideStopAny( *this, collision );
	}
};

bool Sprite::interactsWithBlocks() const
{
	return block_interact_;
};

bool Sprite::interactsWithSprites() const
{
	return sprite_interact_;
};

bool Sprite::collidedRight() const
{
	return collide_right_;
};

bool Sprite::collidedLeft() const
{
	return collide_left_;
};

bool Sprite::collidedAny() const
{
	return collide_left_ || collide_right_ || collide_top_ || collide_bottom_;
};

void Sprite::reset()
{
	resetPosition();
};

void Sprite::resetPosition()
{
	hit_box_.x = original_hit_box_.x;
	hit_box_.y = original_hit_box_.y;
};

bool Sprite::collideTopOnly( const Collision& collision, const Object& them ) const
{
	return collision.collideTop() && prevBottomSubPixels() >= them.ySubPixels() - 1000;
};

bool Sprite::hasCameraMovement( CameraMovement type ) const
{
	return type == camera_movement_;
};

Sprite::CameraMovement Sprite::cameraMovement() const
{
	return camera_movement_;
};

bool Sprite::despawnWhenDead() const
{
	return despawn_when_dead_;
}

void Sprite::deathAction( LevelState& level_state )
{
	defaultDeathAction( level_state );
};

void Sprite::defaultDeathAction( LevelState& level_state )
{
	changeRenderableLayer( level_state, Unit::Layer::BEFORE_FG_2 );
	acceleration_x_ = 0;
	vx_ = 0;
	block_interact_ = false;
	sprite_interact_ = false;
	movement_ = getMovement( SpriteMovement::Type::GROUNDED );
	on_ladder_ = false;

	if ( level_state.camera().offscreen( hit_box_ ) )
	{
		death_finished_ = true;
	}
};

bool Sprite::deathFinished() const
{
	return death_finished_;
};

void Sprite::changeMovement( SpriteMovement::Type type )
{
	movement_ = getMovement( type );
};

void Sprite::killNoAnimation()
{
	kill();
	dead_no_animation_ = true;
};

bool Sprite::collideBottomOnly( const Collision& collision, const Object& other ) const
{
	return collision.collideBottom() && prevBottomSubPixels() <= other.ySubPixels() + 1000;
};

void Sprite::bounceLeft( int overlap )
{
	stopX();
	vx_ = -vx_ + std::max( overlap, 0 ) * 8;
	hit_box_.x += vx_;
};

void Sprite::bounceRight( int overlap )
{
	stopX();
	vx_ = -vx_ + std::min( overlap, 0 ) * 8;
	hit_box_.x += vx_;
};

void Sprite::bounceDownward( int overlap )
{
	stopY();

	if ( vy_ < 0 )
	{
		hit_box_.y -= vy_;
		vy_ = -( vy_ * ( bounce_ * 3 ) );
	}
};

const Collision Sprite::testCollision( const sdl2::SDLRect& hitbox ) const
{
	if ( movement_ != nullptr )
	{
		return movement_->testCollision( *this, hitbox );
	}
	return { 0, 0, 0, 0 };
};

const Collision Sprite::testCollision( const Object& them ) const
{
	return testCollision( them.hit_box_ );
};

SpriteMovement::Type Sprite::movementType() const
{
	return movement_->type();
};

bool Sprite::hasMovementType( SpriteMovement::Type type ) const
{
	return type == movement_->type();
};

const SpriteMovement* Sprite::getMovement( SpriteMovement::Type type )
{
	switch ( type )
	{
		case ( SpriteMovement::Type::FLOATING ):
			return &floating_;
		break;

		case ( SpriteMovement::Type::GROUNDED ):
			return &grounded_;
		break;

		case ( SpriteMovement::Type::FLUTTERING ):
			return &fluttering_;
		break;

		case ( SpriteMovement::Type::SWIMMING ):
			return &swimming_;
		break;

		case ( SpriteMovement::Type::ANGLED ):
			return &angled_;
		break;

		case ( SpriteMovement::Type::STUCK ):
			return &stuck_movement_;
		break;
	}
};

void Sprite::invincibilityFlicker( const Health& health )
{
	graphics_->visible_ = !health.flickerOff();
};

int Sprite::prevRightPixels() const
{
	return Unit::SubPixelsToPixels( prevRightSubPixels() );
};

int Sprite::prevBottomPixels() const
{
	return Unit::SubPixelsToPixels( prevBottomSubPixels() );
};

int Sprite::originalXSubPixels() const
{
	return original_hit_box_.x;
};

int Sprite::originalYSubPixels() const
{
	return original_hit_box_.y;
};

const sdl2::SDLRect& Sprite::originalHitBox() const
{
	return original_hit_box_;
};

int Sprite::prevLeftSubPixels() const
{
	return xPrevSubPixels();
};

int Sprite::prevRightSubPixels() const
{
	return xPrevSubPixels() + hit_box_.w;
};

int Sprite::prevTopSubPixels() const
{
	return yPrevSubPixels();
};

int Sprite::prevBottomSubPixels() const
{
	return yPrevSubPixels() + hit_box_.h;
};

int Sprite::xPrevSubPixels() const
{
	return x_prev_;
};

int Sprite::yPrevSubPixels() const
{
	return y_prev_;
};

int Sprite::xPrevPixels() const
{
	return Unit::SubPixelsToPixels( x_prev_ );
};

int Sprite::yPrevPixels() const
{
	return Unit::SubPixelsToPixels( y_prev_ );
};

sdl2::SDLRect Sprite::justAbove() const
{
	return { hit_box_.x + 3000, hit_box_.y - 16000, hit_box_.w - 6000, 16000 };
};

bool Sprite::blocksJustAbove( const BlockSystem& blocks ) const
{
	return blocks.blocksInTheWay( justAbove(), BlockComponent::Type::SOLID );
};

sdl2::SDLRect Sprite::justBelow() const
{
	return { hit_box_.x + 3000, bottomSubPixels(), hit_box_.w - 6000, 16000 };
};

bool Sprite::blocksJustBelow( const BlockSystem& blocks ) const
{
	return blocks.blocksInTheWay( justBelow(), BlockComponent::Type::SOLID );
};

sdl2::SDLRect Sprite::justLeft() const
{
	return { hit_box_.x - 16000, hit_box_.y + 3000, 16000, hit_box_.h - 6000 };
};

bool Sprite::blocksJustLeft( const BlockSystem& blocks ) const
{
	return blocks.blocksInTheWay( justLeft(), BlockComponent::Type::SOLID );
};

sdl2::SDLRect Sprite::justRight() const
{
	return { rightSubPixels() + 16000, hit_box_.y + 3000, 16000, hit_box_.h - 6000 };
};

bool Sprite::blocksJustRight( const BlockSystem& blocks ) const
{
	return blocks.blocksInTheWay( justRight(), BlockComponent::Type::SOLID );
};

void Sprite::drawHitBox( const Camera& camera )
{
	auto r = camera.relativeRect( Unit::SubPixelsToPixels( hit_box_ ) );
	Render::renderRect( r, 4, 128 );
};

void Sprite::turnOnCollide()
{
	switch ( direction_x_ )
	{
		case ( Direction::Horizontal::LEFT ):
		{
			if ( collidedLeft() )
			{
				direction_x_ = Direction::Horizontal::RIGHT;
			}
		}
		break;

		case ( Direction::Horizontal::RIGHT ):
		{
			if ( collidedRight() )
			{
				direction_x_ = Direction::Horizontal::LEFT;
			}
		}
		break;
	}
};

void Sprite::turnOnEdge( const BlockSystem& blocks )
{
	switch ( direction_x_ )
	{
		case ( Direction::Horizontal::LEFT ):
		{
			if( !blocks.blocksInTheWayWhetherCollided
			(
				{
					leftSubPixels() - Unit::BlocksToSubPixels( 1 ),
					bottomSubPixels(),
					Unit::BlocksToSubPixels( 1 ),
					Unit::BlocksToSubPixels( 1 )

				},
				BlockComponent::Type::SOLID
			))
			{
				direction_x_ = Direction::Horizontal::RIGHT;
			}
		}
		break;

		case ( Direction::Horizontal::RIGHT ):
		{
			if( !blocks.blocksInTheWayWhetherCollided
			(
				{
					rightSubPixels(),
					bottomSubPixels(),
					Unit::BlocksToSubPixels( 1 ),
					Unit::BlocksToSubPixels( 1 )

				},
				BlockComponent::Type::SOLID
			))
			{
				direction_x_ = Direction::Horizontal::LEFT;
			}
		}
		break;
	}
};

void Sprite::moveInDirection()
{
	switch ( direction_ )
	{
		case ( Direction::Simple::UP ):
		{
			moveUp();
		}
		break;
		case ( Direction::Simple::RIGHT ):
		{
			moveRight();
		}
		break;
		case ( Direction::Simple::DOWN ):
		{
			moveDown();
		}
		break;
		case ( Direction::Simple::LEFT ):
		{
			moveLeft();
		}
		break;
	}
}

void Sprite::moveInDirectionX()
{
	switch ( direction_x_ )
	{
		case ( Direction::Horizontal::__NULL ):
			stopX();
		break;

		case ( Direction::Horizontal::LEFT ):
			moveLeft();
		break;

		case ( Direction::Horizontal::RIGHT ):
			moveRight();
		break;
	}
};

void Sprite::inputMoveAllDirections()
{
	if ( Input::held( Input::Action::MOVE_LEFT ) )
	{
		moveLeft();
	}
	else if ( Input::held( Input::Action::MOVE_RIGHT ) )
	{
		moveRight();
	}
	else
	{
		stopX();
	}

	if ( Input::held( Input::Action::MOVE_UP ) )
	{
		moveUp();
	}
	else if ( Input::held( Input::Action::MOVE_DOWN ) )
	{
		moveDown();
	}
	else
	{
		stopY();
	}
};

void Sprite::containCameraX( const Camera& camera )
{
	if ( hit_box_.x < Unit::PixelsToSubPixels( camera.x() ) )
	{
		collideStopXLeft( abs ( camera.x() - hit_box_.x ) );
	}
	else if ( hit_box_.x + hit_box_.w > Unit::PixelsToSubPixels( camera.right() ) )
	{
		collideStopXRight( ( hit_box_.x + hit_box_.w ) - Unit::PixelsToSubPixels( camera.right() ) );
	}
};

void Sprite::containCameraY( const Camera& camera )
{
	if ( hit_box_.y < Unit::PixelsToSubPixels( camera.y() ) )
	{
		collideStopYTop( abs ( camera.y() - hit_box_.y ) );
	}
	else if ( hit_box_.y + hit_box_.h > Unit::PixelsToSubPixels( camera.bottom() ) )
	{
		collideStopYBottom( ( hit_box_.y + hit_box_.w ) - Unit::PixelsToSubPixels( camera.bottom() ) );
	}
};

bool Sprite::onGroundPadding() const
{
	return on_ground_ || ( on_ground_padding_.on() && !on_ground_padding_.done() );
};

void Sprite::flipGraphicsOnRight()
{
	graphics_->flip_x_ = direction_x_ == Direction::Horizontal::RIGHT;
}

bool Sprite::inBox( const sdl2::SDLRect& box ) const
{
	return
		hit_box_.x < box.right()
		&& rightSubPixels() > box.x
		&& hit_box_.y < box.bottom()
		&& bottomSubPixels() > box.y;
};

void Sprite::duck( const DuckData& duck_data )
{
	// Can continue ducking while in air, but only start duck on ground.
	if ( duck_data.can_duck_while_jumping || isDucking() || onGround() )
	{
		// Hacky way to make player warp to the right position after height changes.
		if ( !isDucking() )
		{
			hit_box_.y += ( isUpsideDown() ) ? 0 : Unit::PixelsToSubPixels( duck_data.y_change );
			if ( !isUpsideDown() )
			{
				graphics_->y_adjustment_ = duck_data.gfx_y_change;
			}
			graphics_->h_adjustment_ = duck_data.gfx_h_change;
		}
		is_ducking_ = true;
		hit_box_.h = original_hit_box_.h - Unit::PixelsToSubPixels( duck_data.h_change );
	}
};

void Sprite::unduck( const UnDuckData& unduck_data )
{
	// Hacky way to keep player from falling through ground after gaining height from unducking.
	if ( isDucking() )
	{
		hit_box_.y -= ( isUpsideDown() ) ? 0 : Unit::PixelsToSubPixels( unduck_data.y_change );
		graphics_->y_adjustment_ = unduck_data.gfx_y_change;
		graphics_->h_adjustment_ = unduck_data.gfx_h_change;
	}

	is_ducking_ = false;
	hit_box_.h = original_hit_box_.h - Unit::PixelsToSubPixels( unduck_data.h_change );
};

void Sprite::moveToward( const Sprite& them )
{
	const auto& their_box = them.hit_box_;
	const auto& my_box = hit_box_;
	if ( their_box.isLeftOf( my_box ) )
	{
		moveLeft();
	}
	else if ( their_box.isRightOf( my_box ) )
	{
		moveRight();
	}
	if ( their_box.isAbove( my_box ) )
	{
		moveUp();
	}
	else if ( their_box.isBelow( my_box ) )
	{
		moveDown();
	}
};

bool Sprite::isLeftOf( const Object& them ) const
{
	return rightSubPixels() < them.hit_box_.x;
};

bool Sprite::isRightOf( const Object& them ) const
{
	return hit_box_.x > them.rightSubPixels();
};

bool Sprite::isUpsideDown() const
{
	return gravity_modifier_ < 0.0;
};

void Sprite::changeRenderableLayer( LevelState& level_state, Unit::Layer layer )
{
	if ( layer_ != layer )
	{
		layer_ = layer;
		level_state.changeRenderableLayer( renderable_id_, layer );
	}
};