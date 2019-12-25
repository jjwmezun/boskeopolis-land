#include "swamp_pole_sprite.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"
#include "sprite_system.hpp"

SwampPoleSprite::SwampPoleSprite( int x, int y, int id )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/swamp-pole.png" ), x, y, 16, 448, { SpriteType::SWAMP_POLE }, 25, 1000, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
    pair_ ( nullptr )
{
    misc_.id = id;
};

SwampPoleSprite::~SwampPoleSprite() {};

void SwampPoleSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
    if ( pair_ == nullptr )
    {
        for ( const std::unique_ptr<Sprite>& sprite : sprites.getSpritesList() )
        {
            if ( sprite->hasType( SpriteType::SWAMP_POLE ) && sprite->misc_.id == misc_.id && sprite.get() != this )
            {
                pair_ = sprite.get();
                break;
            }
        }
    }

    if ( !collide_bottom_ )
    {
        acceleration_y_ = 0;
        vy_ = ( int )( ( double )( vy_ ) / 1.15 );
    }
    if ( hit_box_.y >= bottomLimit() )
    {
        vy_ = 0;
        hit_box_.y = bottomLimit();
    }
};

void SwampPoleSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( them.hasType( SpriteType::HERO ) && their_collision.overlapYBottom() > 0 && their_collision.overlapYBottom() < 8000 )
	{
        them.collideStopAny( their_collision );
        if ( hit_box_.y < bottomLimit() )
        {
            moveDown();
            collide_bottom_ = true;
            if ( pair_ != nullptr )
            {
                pair_->moveUp();
                pair_->collide_bottom_ = true;
            }
        }
	}
};

int SwampPoleSprite::bottomLimit() const
{
    return std::min( original_hit_box_.y + Unit::BlocksToSubPixels( 3 ), Unit::BlocksToSubPixels( 24 ) );
};