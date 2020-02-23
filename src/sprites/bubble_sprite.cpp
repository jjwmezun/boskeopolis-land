#include "bubble_sprite.hpp"
#include "collision.hpp"
#include "health.hpp"
#include "level_state.hpp"
#include "sprite_graphics.hpp"

BubbleSprite::BubbleSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/bubble.png" ), x, y, 32, 32, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY )
{};

BubbleSprite::~BubbleSprite() {};

void BubbleSprite::customUpdate( LevelState& level_state )
{};

void BubbleSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( them.hasType( SpriteType::HERO ) && their_collision.collideAny() )
	{
		level_state.health().refillOxygen();
	}
};
