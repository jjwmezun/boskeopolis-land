#include "collision.hpp"
#include "regular_guard_sprite.hpp"
#include "sprite_graphics.hpp"

RegularGuardSprite::RegularGuardSprite( int x, int y, Direction::Horizontal direction )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/police.png", 0, 0, false, false, 0.0, -2, 0, 4, 0 ), x, y, 10, 24, { SpriteType::ENEMY, SpriteType::BOPPABLE }, 2000, 2000, 0, 0, direction, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
	animation_timer_ ()
{};

RegularGuardSprite::~RegularGuardSprite() {};

void RegularGuardSprite::customUpdate( LevelState& level_state )
{
    moveInDirectionX();
	flipGraphicsOnRight();
    turnOnCollide();
	if ( animation_timer_.update() )
	{
		graphics_->current_frame_x_ = ( graphics_->current_frame_x_ == 0 ) ? 14 : 0;
	}
};

void RegularGuardSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
    if ( them.hasType( SpriteType::SOLID ) )
    {
        switch ( direction_x_ )
        {
            case ( Direction::Horizontal::LEFT ):
            {
                if ( my_collision.collideLeft() )
                {
                    direction_x_ = Direction::Horizontal::RIGHT;
                }
            }
            break;

            case ( Direction::Horizontal::RIGHT ):
            {
                if ( my_collision.collideRight() )
                {
                    direction_x_ = Direction::Horizontal::LEFT;
                }
            }
            break;
        }
    }
};

void RegularGuardSprite::deathAction( LevelState& level_state )
{
    changeMovement( SpriteMovement::Type::FLOATING );
    graphics_->current_frame_x_ = 64;
	acceleration_x_ = 0;
	vx_ = 0;
	block_interact_ = false;
	sprite_interact_ = false;
    hit_box_.y += Unit::PixelsToSubPixels( 1 );
    switch ( direction_x_ )
    {
        case ( Direction::Horizontal::LEFT ):
        {
            graphics_->rotation_ -= 10.0;
            if ( graphics_->rotation_ <= -90.0 )
            {
                death_finished_ = true;
            }
        }
        break;
        case ( Direction::Horizontal::RIGHT ):
        {
            graphics_->rotation_ += 10.0;
            if ( graphics_->rotation_ >= 90.0 )
            {
                death_finished_ = true;
            }
        }
        break;
    }
};
