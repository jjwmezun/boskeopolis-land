#include "block_system.hpp"
#include "moveable_block_sprite.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"

MoveableBlockSprite::MoveableBlockSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/box.png" ), x, y, 32, 32, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::RESET_OFFSCREEN_AND_AWAY )
{};

MoveableBlockSprite::~MoveableBlockSprite() {};

void MoveableBlockSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{};

void MoveableBlockSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( their_collision.collideAny() )
	{
		them.collideStopAny( their_collision );
        if ( them.hasType( SpriteType::HERO ) )
        {
            const bool x_condition = them.hit_box_.y < bottomSubPixels() - 4000 && them.bottomSubPixels() > hit_box_.y + 4000 &&
                (
                    ( them.rightSubPixels() <= hit_box_.x + 4000 && them.vx_ > 0 && !blocks.blocksInTheWay( { rightSubPixels() - 4000, hit_box_.y + 4000, 8000, hit_box_.h - 4000 }, BlockComponent::Type::SOLID ) ) ||
                    ( them.hit_box_.x >= rightSubPixels() - 4000 && them.vx_ < 0 && !blocks.blocksInTheWay( { hit_box_.x, hit_box_.y + 4000, 8000, hit_box_.h - 4000 }, BlockComponent::Type::SOLID ) )
                );
            if ( x_condition )
            {
                hit_box_.x += ( double )( them.vx_ ) * 2.0;
            }
        }
	}
};
