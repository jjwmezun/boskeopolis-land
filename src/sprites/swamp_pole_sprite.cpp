#include "camera.hpp"
#include "collision.hpp"
#include "level_state.hpp"
#include "sprite_graphics.hpp"
#include "sprite_system.hpp"
#include "swamp_pole_sprite.hpp"

SwampPoleSprite::SwampPoleSprite( int x, int y, int id )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/swamp-pole.png" ), x, y, 16, 448, { SpriteType::SWAMP_POLE }, 25, 1000, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT, false, false ),
    pair_ ( nullptr )
{
    misc_.id = id;
};

SwampPoleSprite::~SwampPoleSprite() {};

void SwampPoleSprite::customUpdate( LevelState& level_state )
{
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

    if ( pair_ == nullptr )
    {
        for ( const std::unique_ptr<Sprite>& sprite : level_state.sprites().getSpritesList() )
        {
            if ( sprite->hasType( SpriteType::SWAMP_POLE ) && sprite->misc_.id == misc_.id && sprite.get() != this )
            {
                pair_ = sprite.get();
                break;
            }
        }
    }
    else
    {
        const Camera& camera = level_state.camera();
        if ( camera.offscreen( hit_box_ ) && camera.offscreen( pair_->hit_box_ ) )
        {
            reset();
            pair_->reset();
        }
    }
};

void SwampPoleSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
    if ( pair_ != nullptr && them.hasType( SpriteType::HERO ) )
    {
        const Collision collision = them.testBlockCollision( *this );
        if ( collision.overlapYBottom() > 0 && collision.overlapYBottom() < 8000 )
        {
            them.collideStopAny( collision );
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
    }
};

int SwampPoleSprite::bottomLimit() const
{
    return original_hit_box_.y + Unit::BlocksToSubPixels( 2 );
    return ( pair_ == nullptr )
        ? original_hit_box_.y + Unit::BlocksToSubPixels( 2 )
        : std::min( original_hit_box_.y + Unit::BlocksToSubPixels( 2 ), pair_->original_hit_box_.y + Unit::BlocksToSubPixels( 2 ) );
};