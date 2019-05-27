#include "collision.hpp"
#include "map.hpp"
#include "spiky_fruit_sprite.hpp"
#include "sprite_graphics.hpp"

static bool testConnectedToFallingBough( int x, int y, const Map& lvmap )
{
	const int index = Unit::PixelsToBlocks( y ) * lvmap.widthBlocks() + Unit::PixelsToBlocks( x );
	return ( lvmap.sprite( index - 1 ) == 409 || lvmap.sprite( index + 1 ) == 408 ) ? true : false;
}

SpikyFruitSprite::SpikyFruitSprite( int x, int y, const Map& lvmap )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/spikyfruit.png" ), x, y, 16, 16, { SpriteType::ENEMY }, 10, 1400, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_INSTANTLY_OFFSCREEN, false, false ),
	connected_to_falling_bough_ ( testConnectedToFallingBough( x, y, lvmap ) ),
	bough_ ( nullptr )
{};

SpikyFruitSprite::~SpikyFruitSprite() {};

void SpikyFruitSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	if ( movement_->hasType( SpriteMovement::Type::GROUNDED ) )
	{
		graphics_->rotation_ += 5;
	}
	else
	{
		// Make it not affected by wind.
		hit_box_.x = original_hit_box_.x;
	}

	if ( bough_ && !fallingBoughIsBack() )
	{
		changeMovement( SpriteMovement::Type::GROUNDED );
	}
};

void SpikyFruitSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{

	// Find falling bough.
	if
	(
		connected_to_falling_bough_ &&
		!bough_ &&
		them.hasType( SpriteType::FALLING_BOUGH ) &&
		them.hit_box_.y == original_hit_box_.y &&
		( original_hit_box_.x == them.original_hit_box_.x - 8000 || original_hit_box_.x == them.original_hit_box_.x + 16000 )
	)
	{
		bough_ = &them;
	}

	if ( them.hasType( SpriteType::HERO ) )
	{
		if ( them.rightSubPixels() > leftSubPixels() - 16000 && them.leftSubPixels() < rightSubPixels() + 16000 )
		{
			changeMovement( SpriteMovement::Type::GROUNDED );
		}
	}
};

void SpikyFruitSprite::reset()
{
	if ( !bough_ || fallingBoughIsBack() )
	{
		resetPosition();
		changeMovement( SpriteMovement::Type::FLOATING );
		vy_ = 0;
		acceleration_y_ = 0;
		graphics_->rotation_ = 0;
	}
};

bool SpikyFruitSprite::fallingBoughIsBack() const
{
	return bough_->hit_box_.y == original_hit_box_.y;
}
