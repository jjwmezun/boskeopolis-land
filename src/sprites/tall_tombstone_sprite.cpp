#include "tall_tombstone_sprite.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"

TallTombstoneSprite::TallTombstoneSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/tall-tombstone.png" ), x, y, 48, 128, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY )
{};

TallTombstoneSprite::~TallTombstoneSprite() {};

void TallTombstoneSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{};

void TallTombstoneSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( them.hasType( SpriteType::HERO ) && their_collision.collideAny() )
	{
		them.collideStopAny( their_collision );
	}
};