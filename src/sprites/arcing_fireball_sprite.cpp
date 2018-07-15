#include "arcing_fireball_sprite.hpp"
#include "mezun_math.hpp"
#include "sprite_graphics.hpp"

ArcingFireballSprite::ArcingFireballSprite( int x, int y, int start_speed, int top_speed, int jump_start_speed, int jump_top_speed, Direction::Horizontal direction )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/arcing-fireball.png", 0, 0, false, false, 0, true ), x, y, 8, 8, { SpriteType::ENEMY }, start_speed, top_speed, jump_start_speed, jump_top_speed, direction, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::DESPAWN_OFFSCREEN )
{
	block_interact_ = false;
	on_ground_ = true;
	jump_lock_ = false;
	jump();
};

ArcingFireballSprite::~ArcingFireballSprite() {};

void ArcingFireballSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	moveInDirectionX();
	jump();
};

void ArcingFireballSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
};
