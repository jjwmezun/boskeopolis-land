#include "audio.hpp"
#include "collision.hpp"
#include "event_system.hpp"
#include "health.hpp"
#include "flying_grave_monster_sprite.hpp"
#include "inventory_level.hpp"
#include "level_state.hpp"
#include "mezun_math.hpp"
#include "sprite_graphics.hpp"

FlyingGraveMonsterSprite::FlyingGraveMonsterSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/box.png" ), x, y, 24, 24, {}, 100, 1000, 0, 0, Direction::Horizontal::LEFT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY )
{};

FlyingGraveMonsterSprite::~FlyingGraveMonsterSprite() {};

void FlyingGraveMonsterSprite::customUpdate( LevelState& level_state )
{
};

void FlyingGraveMonsterSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) {};
