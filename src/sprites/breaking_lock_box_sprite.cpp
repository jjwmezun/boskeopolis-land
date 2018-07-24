#include "breaking_lock_box_sprite.hpp"
#include "sprite_graphics.hpp"

BreakingLockBoxSprite::BreakingLockBoxSprite( int x, int y, int type )
:
	Sprite( std::make_unique<SpriteGraphics> ( "tilesets/universal.png", 0, 32, false, false, 0, 0, true ), x, y, 16, 16, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::DESPAWN_OFFSCREEN, false, false, false )
{
	graphics_->priority_ = true;
};

BreakingLockBoxSprite::~BreakingLockBoxSprite() {};

void BreakingLockBoxSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
};

void BreakingLockBoxSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{};
