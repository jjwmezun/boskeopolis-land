#include "angry_truck_sprite.hpp"
#include "angry_truck_generator_sprite.hpp"
#include "collision.hpp"
#include "sprite_system.hpp"

static constexpr int TIMER_LIMIT = 60;

AngryTruckGeneratorSprite::AngryTruckGeneratorSprite( int x, int y )
:
	Sprite( nullptr, x, y, 16, 16, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT, false, false, false ),
	timer_ ( 0 )
{};

AngryTruckGeneratorSprite::~AngryTruckGeneratorSprite() {};

void AngryTruckGeneratorSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	if ( timer_ >= TIMER_LIMIT )
	{
		sprites.spawn( std::make_unique<AngryTruckSprite> ( xPixels(), yPixels() ) );
		timer_ = 0;
	}
	++timer_;
};

void AngryTruckGeneratorSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
};
