#include "vacuum_sprite.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"

VacuumSprite* VacuumSprite::generateNormal( int x, int y )
{
    return new VacuumSprite( x, y, 2000 );
};

VacuumSprite* VacuumSprite::generateFast( int x, int y )
{
    return new VacuumSprite( x, y, 2800 );
};

VacuumSprite::VacuumSprite( int x, int y, int speed )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/vacuum.png", 0, 0, false, false, 0.0, -32, 0, 80, 0 ), x, y, 16, 181, {}, speed, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT, false, false )
{
    layer_ = Unit::Layer::BEFORE_FG_1;
};

VacuumSprite::~VacuumSprite() {};

void VacuumSprite::customUpdate( LevelState& level_state )
{
    hit_box_.x += start_speed_;
};

void VacuumSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( them.hasType( SpriteType::HERO ) && their_collision.collideAny() )
	{
        if ( them.graphics_ != nullptr )
        {
            them.graphics_->visible_ = false;
        }
        them.killNoAnimation();
	}
};
