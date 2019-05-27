#include "bouncing_spike_fruit_sprite.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"

static constexpr int decideSpikeFruitJumpHeight( int x )
{
	constexpr int min = 5000;
	constexpr int max = 8000;
	constexpr int difference = max - min;
	constexpr int interval_size = 4;
	constexpr int interval_difference = difference / interval_size;
	x = Unit::PixelsToBlocks( x );
	return min + ( ( x % interval_size ) * interval_difference );
};

BouncingSpikeFruitSprite::BouncingSpikeFruitSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/spikyfruit.png" ), x, y, 16, 16, { SpriteType::ENEMY }, 3000, 3000, 1000, decideSpikeFruitJumpHeight( x ), Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::RESET_OFFSCREEN_AND_AWAY )
{};

BouncingSpikeFruitSprite::~BouncingSpikeFruitSprite() {};

void BouncingSpikeFruitSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	moveLeft();
	jump();
};

void BouncingSpikeFruitSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) {};
