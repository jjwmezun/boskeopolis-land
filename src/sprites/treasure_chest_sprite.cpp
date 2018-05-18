#include "audio.hpp"
#include "treasure_chest_sprite.hpp"
#include "clock.hpp"
#include "collision.hpp"
#include "event_system.hpp"
#include "inventory.hpp"
#include "sprite_graphics.hpp"
#include "sprite_system.hpp"
#include <iostream>

TreasureChestSprite::TreasureChestSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/treasure-chest.png", 0, 0, false, false, 0.0, false, 0, 0, 0, -14 ), x + 2, y + 4, 28, 28, { SpriteType::TREASURE_CHEST }, y + 20, 28, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
	BOTTOM ( { x + 2, y + 18, 28, 14 } )
{};

TreasureChestSprite::~TreasureChestSprite() {};

void TreasureChestSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{};

void TreasureChestSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( them.on_ground_ && events.key_ && them.hasType( SpriteType::HERO ) && their_collision.collideAny() )
	{
		Inventory::clock().stop();
		Audio::turnOffSong();
		sprites.heroOpenTreasureChest();
	}
};

void TreasureChestSprite::render( Camera& camera, bool priority )
{
	graphics_->render( Unit::SubPixelsToPixels( hit_box_ ), &camera, priority );
	if ( !priority )
	{
		Render::renderObject( graphics_->texture_, { top_speed_, 14, 16, 16 }, { BOTTOM.x + 6, start_speed_, 16, 16 }, false, false, 0, 255, &camera );
		Render::renderObject( graphics_->texture_, { 0, 14, 28, 14 }, BOTTOM, false, false, 0, 255, &camera );
	}
};