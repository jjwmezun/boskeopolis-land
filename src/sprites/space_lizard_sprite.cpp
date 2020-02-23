#include "level_state.hpp"
#include "space_lizard_sprite.hpp"
#include "sprite_graphics.hpp"

SpaceLizardSprite::SpaceLizardSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/space-lizard.png", 0, 0, false, false, 0.0, false, -1, -2, 4, 2 ), x, y, 14, 23, { SpriteType::ENEMY, SpriteType::BOPPABLE }, 600, 1400, 0, 0, Direction::Horizontal::LEFT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
	animation_frame_ (),
	animation_timer_ ()
{};

SpaceLizardSprite::~SpaceLizardSprite() {};

void SpaceLizardSprite::customUpdate( LevelState& level_state )
{
	turnOnEdge( level_state.blocks() );
	turnOnCollide();
	moveInDirectionX();
	updateGraphics();
};

void SpaceLizardSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{};

void SpaceLizardSprite::updateGraphics()
{
	flipGraphicsOnRight();
	if ( animation_timer_.hit() )
	{
		++animation_frame_;
		graphics_->current_frame_x_ = animation_frame_() * 18;
	}
	animation_timer_.update();
};
