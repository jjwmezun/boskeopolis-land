#include "audio.hpp"
#include "treasure_chest_sprite.hpp"
#include "clock.hpp"
#include "collision.hpp"
#include "event_system.hpp"
#include "inventory.hpp"
#include "sprite_graphics.hpp"
#include "sprite_system.hpp"

static constexpr int CHEST_OPENING_SPEED = 2;
static constexpr int NUMBER_OF_TREASURE_CHEST_FRAMES = 8;
static constexpr int TREASURE_CHEST_GFX_WIDTH = 28;
static constexpr int CHEST_OPENING_ANIMATION_END = NUMBER_OF_TREASURE_CHEST_FRAMES * CHEST_OPENING_SPEED;
static constexpr int KEY_CANE_MAIN_X_FRAME = 28;
static constexpr int RANDOM_KEYCANE_MAX_Y = 32;
static constexpr int GEM_FRAME_X = KEY_CANE_MAIN_X_FRAME + 32;

TreasureChestSprite::TreasureChestSprite( int x, int y, ItemType item_type )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/treasure-chest.png", 0, 0, false, false, 0.0, false, 0, 0, 0, -14 ), x + 2, y + 4, 28, 28, { SpriteType::TREASURE_CHEST }, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
	BOTTOM ( { x + 2, y + 18, 28, 14 } ),
	item_type_ ( item_type ),
	key_cane_height_ ( 0 ),
	key_cane_y_ ( y + 20 ),
	key_cane_flash_frame_ ( ( item_type == ItemType::RANDOM_GEMS ) ? GEM_FRAME_X : KEY_CANE_MAIN_X_FRAME ),
	timer_ ( 0 ),
	gem_count_ ( 0 )
{};

TreasureChestSprite::~TreasureChestSprite() {};

void TreasureChestSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	switch ( jump_start_speed_ )
	{
		case( 1 ):
		{
			animateChestOpening();
		}
		break;
		case( 2 ):
		{
			raiseItem();
		}
		break;
	}
};

void TreasureChestSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( them.on_ground_ && events.hasKey() && them.hasType( SpriteType::HERO ) && their_collision.collideAny() )
	{
		Inventory::clock().stop();
		Audio::fadeMusic( 500 );
		sprites.heroOpenTreasureChest();
		events.setPauseDisabled();
	}
};

void TreasureChestSprite::render( Camera& camera, bool priority )
{
	if ( !priority )
	{
		renderChestTopHalf( camera );
		renderKeyCane( camera );
		renderChestBottomHalf( camera );
	}
};

void TreasureChestSprite::renderKeyCane( const Camera& camera )
{
	Render::renderObject( graphics_->texture_, { key_cane_flash_frame_, 14, 16, key_cane_height_ }, getKeyCaneGFXPosition(), false, false, 0, 255, &camera );
};

void TreasureChestSprite::renderChestBottomHalf( const Camera& camera )
{
	Render::renderObject( graphics_->texture_, { 0, 14, 28, 14 }, BOTTOM, false, false, 0, 255, &camera );
};

void TreasureChestSprite::renderChestTopHalf( const Camera& camera )
{
	graphics_->render( Unit::SubPixelsToPixels( hit_box_ ), &camera, false );
};

void TreasureChestSprite::animateChestOpening()
{
	if ( timer_ % CHEST_OPENING_SPEED == 0 )
	{
		if ( timer_ == CHEST_OPENING_ANIMATION_END )
		{
			timer_ = 0;
			jump_start_speed_ = 2;
			return;
		}
		else if ( timer_ == 2 )
		{
			Audio::playSound( Audio::SoundType::CHEST_OPEN );
		}
		else
		{
			const int FRAME = getAnimationFrame( CHEST_OPENING_SPEED ) % NUMBER_OF_TREASURE_CHEST_FRAMES;
			graphics_->current_frame_x_ = TREASURE_CHEST_GFX_WIDTH * FRAME;
		}
	}
	++timer_;
}

void TreasureChestSprite::raiseItem()
{
	switch ( item_type_ )
	{
		case( ItemType::KEY_KEYCANE ):
		case( ItemType::RANDOM_KEYCANE ):
		{
			setKeyCanePosition();
			setKeyCaneFlash();
			++timer_;
			setKeyCaneHeight();
		}
		break;

		case( ItemType::RANDOM_GEMS ):
		{
			if ( key_cane_y_ <= yPixels() - RANDOM_KEYCANE_MAX_Y )
			{
				if ( gem_count_ < 3 )
				{
					Inventory::addFunds( 250 );
				}
				++gem_count_;
				key_cane_y_ = yPixels() + 20;
				key_cane_height_ = 0;
			}
			else
			{
				if ( gem_count_ == 3 ) { return; }

				if ( key_cane_height_ == 0 )
				{
					Audio::playSound( Audio::SoundType::GEM );
				}

				if ( key_cane_y_ > yPixels() - RANDOM_KEYCANE_MAX_Y )
				{
					key_cane_y_ -= 4;
				}
				++timer_;
				setKeyCaneHeight();
			}
		}
		break;
	}
}

int TreasureChestSprite::getAnimationFrame( int speed ) const
{
	return ( int )( floor( timer_ / speed ) );
};

void TreasureChestSprite::setKeyCanePosition()
{
	if ( item_type_ == ItemType::RANDOM_KEYCANE || timer_ % 4 == 0 )
	{
		const int MAX_Y = ( item_type_ == ItemType::KEY_KEYCANE ) ? 8 : RANDOM_KEYCANE_MAX_Y;
		if ( key_cane_y_ > yPixels() - MAX_Y )
		{
			--key_cane_y_;
		}
	}
};

void TreasureChestSprite::setKeyCaneFlash()
{
	if ( timer_ % 4 == 0 && ( item_type_ == ItemType::KEY_KEYCANE || isRandomKeyCaneAtTop() ) )
	{
		const int FLASH = ( int )( floor ( ( int )( floor( timer_ / 4 ) ) / 2 ) ) % 2;
		setKeyCaneGFXFlash( FLASH == 1 );
	}
};

void TreasureChestSprite::setKeyCaneHeight()
{
	key_cane_height_ = std::min( ( yPixels() + 20 ) - key_cane_y_, 16 );
};

bool TreasureChestSprite::isRandomKeyCaneAtTop() const
{
	return key_cane_y_ <= yPixels() - RANDOM_KEYCANE_MAX_Y;
}

sdl2::SDLRect TreasureChestSprite::getKeyCaneGFXPosition() const
{
	return { BOTTOM.x + 6, key_cane_y_, 16, key_cane_height_ };
}

Object TreasureChestSprite::getKeyCanePosition() const
{
	const auto r = getKeyCaneGFXPosition();
	return Object( r.x, r.y, r.w, r.h );
}

void TreasureChestSprite::setItemGFXToGem()
{
	key_cane_flash_frame_ = GEM_FRAME_X;
};

void TreasureChestSprite::setKeyCaneGFXFlash( bool flash )
{
	key_cane_flash_frame_ = ( flash ) ? KEY_CANE_MAIN_X_FRAME + 16 : KEY_CANE_MAIN_X_FRAME;
};
