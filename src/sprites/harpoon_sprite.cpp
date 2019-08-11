#include "audio.hpp"
#include "harpoon_sprite.hpp"
#include "health.hpp"
#include "collision.hpp"

HarpoonSprite::HarpoonSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/pike2.png", 0, 12 ), x, y, 32, 16, { SpriteType::ENEMY }, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
	delay_timer_ ( 0 ),
	grow_speed_ ( 0 ),
	state_ ( HarpoonState::WAIT_TO_GROW ),
	left_harpoon_ ( Unit::PixelsToSubPixels({ x + 2, y + 2, 12, 12 }) ),
	right_harpoon_ ( Unit::PixelsToSubPixels({ x + 18, y + 2, 12, 12 }) ),
	left_harpoon_original_ ( Unit::PixelsToSubPixels({ x + 2, y + 2, 12, 12 }) ),
	right_harpoon_original_ ( Unit::PixelsToSubPixels({ x + 18, y + 2, 12, 12 }) ),
	left_harpoon_gfx_ ( "sprites/pike2.png", 0, 0 ),
	right_harpoon_gfx_ ( "sprites/pike2.png", 0, 0, true ),
	frames_ ({ 0, 0, 0, 0, 32, 64, 96, 96, 96, 96, 64, 32 })
{};

HarpoonSprite::~HarpoonSprite() {};

void HarpoonSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	switch ( state_ )
	{
		case ( HarpoonState::WAIT_TO_GROW ):
		{
			graphics_->current_frame_x_ = 96;
			if ( delay_timer_ >= NUMBER_OF_FRAMES * 4 )
			{
				state_ = HarpoonState::GROW;
				delay_timer_ = 0;
			}
			else
			{
				graphics_->current_frame_x_ = frames_[ delay_timer_ % NUMBER_OF_FRAMES ];
				++delay_timer_;
			}
		}
		break;

		case ( HarpoonState::GROW ):
		{
			grow_speed_ += 250;
			if ( grow_speed_ == 250 * 8 )
			{
				Audio::playSound( Audio::SoundType::CHEST_OPEN );
			}
			left_harpoon_.w += grow_speed_;
			if ( left_harpoon_.w >= 192000 )
			{
				left_harpoon_.w = 192000;
				state_ = HarpoonState::WAIT_TO_SHRINK;
				grow_speed_ = 0;
			}
			right_harpoon_.w = left_harpoon_.w;
			left_harpoon_.x = left_harpoon_original_.x - left_harpoon_.w + 12000;
		}
		break;

		case ( HarpoonState::WAIT_TO_SHRINK ):
		{
			if ( delay_timer_ >= NUMBER_OF_FRAMES * 4 )
			{
				state_ = HarpoonState::SHRINK;
				delay_timer_ = 0;
			}
			else
			{
				const int frame = std::max( 6 - delay_timer_, 0 );
				graphics_->current_frame_x_ = frames_[ frame ];
				++delay_timer_;
			}
		}
		break;

		case ( HarpoonState::SHRINK ):
		{
			grow_speed_ -= 250;
			if ( grow_speed_ < -4000 )
			{
				grow_speed_ = -4000;
			}
			left_harpoon_.w += grow_speed_;
			if ( left_harpoon_.w <= 12000 )
			{
				left_harpoon_.w = 12000;
				state_ = HarpoonState::WAIT_TO_GROW;
			}
			right_harpoon_.w = left_harpoon_.w;
			left_harpoon_.x = left_harpoon_original_.x - left_harpoon_.w + 12000;
		}
		break;
	}
};

void HarpoonSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( them.hasType( SpriteType::HERO ) )
	{
		if ( their_collision.collideAny() )
		{
			them.collideStopAny( their_collision );
		}
		const Collision left_collision = them.testCollision( left_harpoon_ );
		if ( left_collision.collideAny() )
		{
			health.hurt();
		}
		else
		{
			const Collision right_collision = them.testCollision( right_harpoon_ );
			if ( right_collision.collideAny() )
			{
				health.hurt();
			}
		}
	}
};

void HarpoonSprite::render( Camera& camera, bool priority )
{
	left_harpoon_gfx_.render( Unit::SubPixelsToPixels( left_harpoon_ ), &camera, priority );
	right_harpoon_gfx_.render( Unit::SubPixelsToPixels( right_harpoon_ ), &camera, priority );
	graphics_->render( Unit::SubPixelsToPixels( hit_box_ ), &camera, priority );
};
