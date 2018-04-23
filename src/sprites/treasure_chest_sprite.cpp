#include "treasure_chest_sprite.hpp"
#include "collision.hpp"
#include "event_system.hpp"
#include "sprite_graphics.hpp"
#include "sprite_system.hpp"

TreasureChestSprite::TreasureChestSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/treasure-chest.png" ), x + 2, y + 4, 28, 28, { SpriteType::TREASURE_CHEST }, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY )
{};

TreasureChestSprite::~TreasureChestSprite() {};

void TreasureChestSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{};

void TreasureChestSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( them.on_ground_ && events.key_ && them.hasType( SpriteType::HERO ) && their_collision.collideAny() )
	{
		sprites.heroOpenTreasureChest();
	}
};