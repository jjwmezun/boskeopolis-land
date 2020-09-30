#include "level_state.hpp"
#include "sprite_graphics.hpp"
#include "toy_soldier_sprite.hpp"

static constexpr int FRAMES[ ToySoldierSprite::NUM_O_FRAMES ] = { 0, 15, 0, 30 };

ToySoldierSprite::ToySoldierSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/toy-soldier.png", 0, 0, false, false, 0.0, -5, -1, 7, 1 ), x, y, 8, 31, { SpriteType::ENEMY, SpriteType::BOPPABLE }, 600, 1400, 0, 0, Direction::Horizontal::LEFT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
	animation_frame_ (),
	animation_timer_ ()
{};

ToySoldierSprite::~ToySoldierSprite() {};

void ToySoldierSprite::customUpdate( LevelState& level_state )
{
	turnOnEdge( level_state.blocks() );
	turnOnCollide();
	moveInDirectionX();
	updateGraphics();
};

void ToySoldierSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{};

void ToySoldierSprite::updateGraphics()
{
	flipGraphicsOnRight();
	if ( animation_timer_.hit() )
	{
		++animation_frame_;
		graphics_->current_frame_x_ = FRAMES[ animation_frame_() ];
	}
	animation_timer_.update();
};
