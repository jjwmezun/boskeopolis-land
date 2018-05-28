#include "collision.hpp"
#include "health.hpp"
#include "lightning_sprite.hpp"
#include "sprite_graphics.hpp"

LightningSprite::LightningSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/lightning.png", 0, 0, false, false, 0, true ), x, y, 4, 160, { SpriteType::LIGHTNING }, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT )
{};

LightningSprite::~LightningSprite() {};

void LightningSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{};

void LightningSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( my_collision.collideAny() )
	{
		health.hurt();
	}
};