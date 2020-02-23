#include "audio.hpp"
#include "gemmy_sprite.hpp"
#include "collision.hpp"
#include "inventory.hpp"
#include "mezun_math.hpp"
#include "sprite_graphics.hpp"

static constexpr int GFX_TIME_LIMIT = 8;
static constexpr int GFX_FRAME_END = 6;

GemmySprite::GemmySprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/gemmy.png" ), x, y, 10, 19, {}, 500, 3000, 0, 0, Direction::Horizontal::RIGHT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
	gfx_frame_ ( 0 ),
	gfx_timer_ ( 0 )
{};

GemmySprite::~GemmySprite() {};

void GemmySprite::customUpdate( LevelState& level_state )
{
	switch ( direction_x_ )
	{
		case ( Direction::Horizontal::LEFT ):
			graphics_->current_frame_y_ = 19;
			moveLeft();
		break;

		case ( Direction::Horizontal::RIGHT ):
			graphics_->current_frame_y_ = 0;
			moveRight();
		break;
	}

	if ( collidedRight() )
	{
		direction_x_ = Direction::Horizontal::LEFT;
	}
	if ( collidedLeft() )
	{
		direction_x_ = Direction::Horizontal::RIGHT;
	}
	else if ( mezun::testRandomWithinPercentage( 5 ) )
	{
		direction_x_ = Direction::switchHorizontal( direction_x_ );
	}
	
	++gfx_timer_;
	if ( gfx_timer_ >= GFX_TIME_LIMIT )
	{
		gfx_timer_ = 0;
		++gfx_frame_;
		if ( gfx_frame_ >= GFX_FRAME_END ) gfx_frame_ = 0;
		graphics_->current_frame_x_ = gfx_frame_ * 10;
	}
};

void GemmySprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( them.hasType( SpriteType::HERO ) && their_collision.collideAny() )
	{
		Inventory::addFunds( 100 );
		Audio::playSound( Audio::SoundType::GEM );
	}
};