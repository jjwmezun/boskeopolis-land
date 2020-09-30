#include "level_state.hpp"
#include "main.hpp"
#include "olive_sprite.hpp"
#include "sprite_graphics.hpp"

static constexpr int SPEED = 800;

OliveSprite::OliveSprite( int x, int y, Direction::Horizontal start_dir )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/olive.png", 0, 0, false, false, 0.0, -2, -2, 4, 4 ), x, y, 12, 12, { SpriteType::ENEMY }, SPEED, SPEED, 0, 0, start_dir, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::PERMANENT, true, true, true )
{};

OliveSprite::~OliveSprite() {};

void OliveSprite::customUpdate( LevelState& level_state )
{
	turnOnCollide();
	moveInDirectionX();
	
	switch ( direction_x_ )
	{
		case ( Direction::Horizontal::RIGHT ):
			graphics_->rotation_ += 5;
		break;
		case ( Direction::Horizontal::LEFT ):
			graphics_->rotation_ -= 5;
		break;
	}
	
	if ( fellInBottomlessPit( level_state.currentMap() ) )
	{
		kill();
	}
};

void OliveSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
};