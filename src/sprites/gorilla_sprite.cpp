#include "gorilla_sprite.hpp"
#include "collision.hpp"
#include "animated_graphics.hpp"

GorillaSprite::GorillaSprite( int x, int y )
:
	Sprite( std::unique_ptr<SpriteGraphics> ( new AnimatedGraphics( "sprites/gorilla.png", { std::make_pair( 0, 0 ), std::make_pair( 16, 0 ) } ) ), x, y, 16, 16, { SpriteType::ENEMY, SpriteType::BOPPABLE }, 800, 3000, 0, 0, Direction::Horizontal::LEFT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::RESET_OFFSCREEN_AND_AWAY )
{};

GorillaSprite::~GorillaSprite() {};

void GorillaSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	turnOnCollide();
	moveInDirectionX();
	graphics_->flip_x_ = direction_x_ == Direction::Horizontal::RIGHT;
};

void GorillaSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{};