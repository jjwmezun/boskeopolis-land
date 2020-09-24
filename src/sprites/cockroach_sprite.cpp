#include "level_state.hpp"
#include "cockroach_sprite.hpp"
#include "sprite_graphics.hpp"

CockroachSprite::CockroachSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/cockroach.png", 0, 0, false, false, 0.0, false, -1, -5, 3, 9 ), x, y, 29, 7, { SpriteType::ENEMY, SpriteType::BOPPABLE }, 500, 2000, 0, 0, Direction::Horizontal::RIGHT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
	timer_ ( 0 )
{};

CockroachSprite::~CockroachSprite() {};

void CockroachSprite::customUpdate( LevelState& level_state )
{
	turnOnEdge( level_state.blocks() );
	turnOnCollide();
	moveInDirectionX();
	flipGraphicsOnRight();

	if ( timer_ == 8 )
	{
		timer_ = 0;
		graphics_->current_frame_x_ = ( graphics_->current_frame_x_ == 0 ) ? 32 : 0;
	}
	else
	{
		++timer_;
	}
};

void CockroachSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) {};

void CockroachSprite::deathAction( const Camera& camera, EventSystem& events, const Map& lvmap )
{
	graphics_->priority_ = true;
	defaultDeathAction( camera );
};

void CockroachSprite::render( Camera& camera, bool priority )
{
	if ( !isDead() )
	{
		graphics_->render( Unit::SubPixelsToPixels( hit_box_ ), &camera, priority );
	}
};

void CockroachSprite::renderSuperPriority( Camera& camera )
{
	if ( isDead() )
	{
		graphics_->render( Unit::SubPixelsToPixels( hit_box_ ), &camera, true );
	}
};