#include "traffic_car_sprite.hpp"
#include "block_system.hpp"
#include "collision.hpp"
#include "health.hpp"
#include "level_state.hpp"
#include "sprite_graphics.hpp"

TrafficCarSprite::TrafficCarSprite( int x, int y, Direction::Horizontal direction, int speed )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/angry_truck.png" ), x, y, 40, 32, { SpriteType::ENEMY }, speed, speed, 0, 0, direction, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::PERMANENT )
{
	graphics_->flip_x_ = direction_x_ == Direction::Horizontal::RIGHT;
};

TrafficCarSprite::~TrafficCarSprite() {};

void TrafficCarSprite::customUpdate( LevelState& level_state )
{
	moveInDirectionX();
	if ( fellInBottomlessPit( level_state.currentMap() ) )
	{
		killNoAnimation();
	}
};

void TrafficCarSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) {};
