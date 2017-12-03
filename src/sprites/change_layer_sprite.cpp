#include "audio.hpp"
#include "change_layer_sprite.hpp"
#include "collision.hpp"
#include "event_system.hpp"
#include "main.hpp"
#include "sprite_graphics.hpp"

static constexpr int Y = 32;
static constexpr int FLASH_SPEED = 4;
static constexpr int ANIMATION_SPEED = 8;

ChangeLayerSprite* ChangeLayerSprite::makeForeground( int x )
{
	return new ChangeLayerSprite( x, Y, 16, 216, 1, Direction::Vertical::UP, "sprites/change-layer-arrows.png" );
};

ChangeLayerSprite* ChangeLayerSprite::makeBackground( int x )
{
	return new ChangeLayerSprite( x, Y, 16, 216, 1, Direction::Vertical::DOWN, "sprites/change-layer-arrows.png" );
};

ChangeLayerSprite* ChangeLayerSprite::makeBackgroundDuo( int x, int y )
{
	return new ChangeLayerSprite( x, y, 32, 16, 16, Direction::Vertical::DOWN, "sprites/change-layer-arrows-2.png" );
};

ChangeLayerSprite::ChangeLayerSprite( int x, int y, int w, int h, int frame_size, Direction::Vertical dir, std::string texture )
:
	Sprite( std::make_unique<SpriteGraphics> ( std::move( texture ), 0, 4, false, ( dir == Direction::Vertical::UP ), 0, true ), x, y, w, h, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, dir, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
	flash_timer_ ( 0 ),
	frame_size_ ( frame_size ),
	activated_ ( false )
{};

ChangeLayerSprite::~ChangeLayerSprite() {};

void ChangeLayerSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	if ( activated_ )
	{
		events.switch_ = ( direction_y_ == Direction::Vertical::DOWN );

		if ( flash_timer_ > 0 && flash_timer_ % FLASH_SPEED == 0 )
		{
			graphics_->current_frame_x_ = ( graphics_->current_frame_x_ == 0 ) ? widthPixels() : 0;
		}
		++flash_timer_;
		
		if ( flash_timer_ >= FLASH_SPEED * 11 )
		{
			activated_ = false;
			flash_timer_ = 0;
			graphics_->current_frame_x_ = 0;
		}
	}

	if ( Main::nextStateFrame() )
	{
		graphics_->current_frame_y_ -= frame_size_;
		if ( graphics_->current_frame_y_ < 0 ) { graphics_->current_frame_y_ = ( 4 * frame_size_ ); }
	}
};

void ChangeLayerSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( !activated_ && ( events.switch_ == ( direction_y_ == Direction::Vertical::UP ) ) && them.hasType( SpriteType::HERO ) && their_collision.collideAny() )
	{
		activated_ = true;
		Audio::playSound( Audio::SoundType::HEAL );
	}
};