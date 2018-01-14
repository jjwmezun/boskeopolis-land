#include "carrot_sprite.hpp"
#include "sprite_graphics.hpp"

CarrotSprite::CarrotSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/carrot.png" ), x, y, 16, 24, { SpriteType::ENEMY }, 600, 1400, 0, 0, Direction::Horizontal::LEFT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::RESET_OFFSCREEN_AND_AWAY )
{};

CarrotSprite::~CarrotSprite() {};

void CarrotSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	turnOnEdge( blocks );
	turnOnCollide();
	moveInDirectionX();
};

void CarrotSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{};