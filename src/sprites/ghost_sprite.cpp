#include "ghost_sprite.hpp"
#include "collision.hpp"
#include "health.hpp"
#include "level_state.hpp"
#include "sprite_graphics.hpp"

static constexpr int VERTICAL_DIST_LIMIT_BLOCKS = 2;
static constexpr int VERTICAL_DIST_LIMIT_SUBPIXELS = Unit::BlocksToSubPixels( VERTICAL_DIST_LIMIT_BLOCKS );
static constexpr int MAX_TONGUE_FRAME = 5;
static constexpr int MAX_OUTLINE_FRAME = 9;

GhostSprite::GhostSprite( int x, int y, Direction::Horizontal direction, int speed )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/kappa-obake-2.png", 0, 0, direction == Direction::Horizontal::RIGHT, false, 0.0, -10, -6, 15, 9 ), x, y, 32, 32, { SpriteType::ENEMY }, 800 * speed, 800 * speed, 0, 0, direction, Direction::Vertical::DOWN, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY, false, false ),
	animation_ ( 0 ),
	tongue_frame_ ( 0 ),
	outline_frame_ ( 0 )
{
	layer_ = Unit::Layer::SPRITES_2;
    sprite_interact_from_this_to_others_only_ = true;
};

GhostSprite::~GhostSprite() {};

void GhostSprite::customUpdate( LevelState& level_state )
{
	if ( animation_ >= Unit::DEFAULT_ANIMATION_SPEED )
	{
		++tongue_frame_;
		if ( tongue_frame_ > MAX_TONGUE_FRAME )
		{
			tongue_frame_ = 0;
		}
		++outline_frame_;
		if ( outline_frame_ > MAX_OUTLINE_FRAME )
		{
			outline_frame_ = 0;
		}
		animation_ = 0;
	}
	++animation_;

	switch ( tongue_frame_ )
	{
		case ( 0 ):
			graphics_->current_frame_x_ = 0;
		break;
		case ( 1 ):
		case ( 5 ):
			graphics_->current_frame_x_ = 47;
		break;
		case ( 2 ):
		case ( 4 ):
			graphics_->current_frame_x_ = 47 * 2;
		break;
		case ( 3 ):
			graphics_->current_frame_x_ = 47 * 3;
		break;
	}

	switch ( outline_frame_ )
	{
		case ( 0 ):
			graphics_->current_frame_y_ = 0;
		break;
		case ( 1 ):
		case ( 9 ):
			graphics_->current_frame_y_ = 41;
		break;
		case ( 2 ):
		case ( 8 ):
			graphics_->current_frame_y_ = 41 * 2;
		break;
		case ( 3 ):
		case ( 7 ):
			graphics_->current_frame_y_ = 41 * 3;
		break;
		case ( 4 ):
		case ( 6 ):
			graphics_->current_frame_y_ = 41 * 4;
		break;
		case ( 5 ):
			graphics_->current_frame_y_ = 41 * 5;
		break;
	}
	graphics_->current_frame_y_ = 41 * 5;

	moveInDirectionX();
	switch ( direction_y_ )
	{
		case ( Direction::Vertical::DOWN ):
			moveDown();
		break;

		case ( Direction::Vertical::UP ):
			moveUp();
		break;
	}
	
	if ( hit_box_.y > original_hit_box_.y + VERTICAL_DIST_LIMIT_SUBPIXELS )
	{
		direction_y_ = Direction::Vertical::UP;
	}
	else if ( hit_box_.y < original_hit_box_.y - VERTICAL_DIST_LIMIT_SUBPIXELS )
	{
		direction_y_ = Direction::Vertical::DOWN;
	}
};

void GhostSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
    if ( them.hasType( SpriteType::HERO ) && their_collision.collideAny() )
    {
        level_state.health().hurt();
    }
};