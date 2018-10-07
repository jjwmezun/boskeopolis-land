#include "camera.hpp"
#include "collision.hpp"
#include "map.hpp"
#include "mezun_math.hpp"
#include "sprite_graphics.hpp"
#include "urban_bird_sprite.hpp"

static int getRandomDelay()
{
	return mezun::randInt( 120, 48 );
};

UrbanBirdSprite::UrbanBirdSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/urban-bird.png", 0, 0, false, false, 0.0, true ), x, y, 16, 16, { SpriteType::PHASE_THROUGH }, 200, 3000, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT, false, false ),
	reset_timer_ ( 0 ),
	reset_delay_ ( 0 ),
	remember_x_ ( 0 ),
	remember_y_ ( 0 )
{};

UrbanBirdSprite::~UrbanBirdSprite() {};

void UrbanBirdSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	if ( reset_timer_ >= reset_delay_ )
	{
		setNewPosition( camera, lvmap );
	}
	else if ( camera.offscreen( hit_box_ ) )
	{
		++reset_timer_;
	}
	else
	{
		moveLeft();
	}
};

void UrbanBirdSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( them.hasType( SpriteType::HERO ) )
	{
		if ( remember_x_ == 0 )
		{
			if ( their_collision.collideAny() )
			{
				remember_x_ = hit_box_.x - them.hit_box_.x;
				remember_y_ = them.hit_box_.y;
			}
		}
		else
		{
			// This is where it drops you off.
			if ( hit_box_.x < 64000 )
			{
				remember_x_ = 0;
				remember_y_ = 0;
				them.graphics_->priority_ = false;
			}
			else
			{
				them.graphics_->priority_ = true;
				them.hit_box_.x = hit_box_.x - remember_x_; // Keep their x position relative to bird.
				them.hit_box_.y = remember_y_; // Keep them to their y position when they were grabbed.
			}
		}
	}
};

void UrbanBirdSprite::setNewPosition( const Camera& camera, const Map& lvmap )
{
	// New next delay.
	reset_delay_ = getRandomDelay();

	// Reset reset timer.
	reset_timer_ = 0;

	// Bird respawns just right o' camera.
	hit_box_.x = Unit::PixelsToSubPixels( camera.right() + 64 );

	// Give random vertical position within camera, so long as it's not below ground level.
	hit_box_.y = Unit::PixelsToSubPixels( mezun::randInt( std::min( camera.bottom() - heightPixels(), lvmap.heightPixels() - 80 ), camera.y() ) );
}
