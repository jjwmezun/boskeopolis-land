#include "icicle_sprite.hpp"
#include "level_state.hpp"
#include <memory>
#include "mezun_math.hpp"
#include "spawn_icicle_sprite.hpp"
#include "sprite_graphics.hpp"
#include "sprite_system.hpp"
#include "unit.hpp"

static constexpr int LEFT_EDGE = Unit::BlocksToSubPixels( 126 );
static constexpr int TOP_EDGE = Unit::BlocksToSubPixels( 13 );
static constexpr int TOP_LEFT_EDGE = Unit::BlocksToSubPixels( 112 );
static constexpr int SPAWN_SPEED = 100;

SpawnIcicleSprite::SpawnIcicleSprite( int x, int y )
:
	Sprite( nullptr, x, y, 16, 16, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT ),
	timer_ ( 0 ),
	go_ ( false )
{};

SpawnIcicleSprite::~SpawnIcicleSprite() {};

void SpawnIcicleSprite::customUpdate( LevelState& level_state )
{
	if ( go_ )
	{
		if ( timer_ == SPAWN_SPEED )
		{
			level_state.sprites().spawn( std::unique_ptr<IcicleSprite>( new IcicleSprite( xPixels(), yPixels() ) ) );
			timer_ = 0;
		}
		else
		{
			++timer_;
		}
	}
};

bool SpawnIcicleTestGo( sdl2::SDLRect& r )
{
	return r.x > LEFT_EDGE || ( r.x > TOP_LEFT_EDGE && r.y < TOP_EDGE );
};

void SpawnIcicleSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( them.hasType( SpriteType::HERO ) )
	{
		go_ = ( SpawnIcicleTestGo( them.hit_box_ ) ) ? true: false;
	}
};
