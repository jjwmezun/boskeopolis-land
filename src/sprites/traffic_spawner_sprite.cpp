#include "camera.hpp"
#include "level_state.hpp"
#include "mezun_math.hpp"
#include "sprite_system.hpp"
#include "traffic_car_sprite.hpp"
#include "traffic_spawner_sprite.hpp"

static int generateRandomTarget()
{
    return mezun::randInt( 45, 30 );
};

TrafficSpawnerSprite::TrafficSpawnerSprite( int x, int y )
:
	Sprite( nullptr, x, y, 32, 32, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT ),
    timer_ ( 0 ),
    target_ ( generateRandomTarget() )
{};

TrafficSpawnerSprite::~TrafficSpawnerSprite() {};

void TrafficSpawnerSprite::customUpdate( LevelState& level_state )
{
    if ( timer_ == target_ )
    {
        const Camera& camera = level_state.camera();
        target_ = generateRandomTarget();
        timer_ = 0;
        const Direction::Horizontal direction = ( mezun::randBool() ) ? Direction::Horizontal::LEFT : Direction::Horizontal::RIGHT;
        const int x = ( direction == Direction::Horizontal::LEFT ) ? camera.right() : camera.left() - 40;
        const int speed = mezun::randInt( 12000, 6000 );
        level_state.sprites().spawn( std::unique_ptr<TrafficCarSprite>( new TrafficCarSprite( x, Unit::BlocksToPixels( 32 ), direction, speed ) ) );
    }
    ++timer_;
};

void TrafficSpawnerSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) {};
