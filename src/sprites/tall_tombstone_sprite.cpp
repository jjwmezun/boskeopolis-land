#include "tall_tombstone_sprite.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"

TallTombstoneSprite::TallTombstoneSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/tall-tombstone.png" ), x, y, 48, 128, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY )
{};

TallTombstoneSprite::~TallTombstoneSprite() {};

void TallTombstoneSprite::customUpdate( LevelState& level_state )
{};

void TallTombstoneSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	Collision collision = them.testBlockCollision( *this );
	if ( them.hasType( SpriteType::HERO ) && collision.collideAny() )
	{
		them.collideStopAny( collision );
	}
};
