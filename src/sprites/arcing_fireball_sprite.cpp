#include "arcing_fireball_sprite.hpp"
#include "mezun_math.hpp"
#include "sprite_graphics.hpp"

ArcingFireballSprite::ArcingFireballSprite( int x, int y, int start_speed, int top_speed, int jump_start_speed, int jump_top_speed, Direction::Horizontal direction )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/arcing-fireball.png", 0, 0, false, false, 0 ), x, y, 8, 8, { SpriteType::ENEMY }, start_speed, top_speed, jump_start_speed, jump_top_speed, direction, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::DESPAWN_OFFSCREEN )
{
	block_interact_ = false;
	on_ground_ = true;
	jump_lock_ = false;
	jump();
	layer_ = Unit::Layer::SPRITES_2;
};

ArcingFireballSprite::~ArcingFireballSprite() {};

void ArcingFireballSprite::customUpdate( LevelState& level_state )
{
	moveInDirectionX();
	jump();
};

void ArcingFireballSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
};
