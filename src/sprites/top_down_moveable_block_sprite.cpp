#include "block_system.hpp"
#include "top_down_moveable_block_sprite.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"
#include "sprite_system.hpp"

// Note: top_speed_ used as hole trigger 'cause polymorphism is too limited for using sane variable names.

TopDownMoveableBlockSprite::TopDownMoveableBlockSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/moveable-block.png", 0, 0 ), x, y, 16, 16, { SpriteType::MOVEABLE_BLOCK }, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY )
{};

TopDownMoveableBlockSprite::~TopDownMoveableBlockSprite() {};

void TopDownMoveableBlockSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{};

void TopDownMoveableBlockSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( top_speed_ == 0 && them.hasType( SpriteType::HERO ) && their_collision.collideAny() )
	{
		const bool going_right = them.vx_ > 0;
		const bool x_condition =
			( them.hit_box_.y < bottomSubPixels() - 4000 && them.bottomSubPixels() > hit_box_.y + 4000 )
			&&
				(
					( going_right && !blocksInTheWay( blocks, getRightBoundarySpace() ) && !spritesOnOtherSide( sprites, getRightBoundarySpace() )  )
					|| ( !going_right && !blocksInTheWay( blocks, getLeftBoundarySpace() ) && !spritesOnOtherSide( sprites, getLeftBoundarySpace() ) )
				);

		if ( x_condition )
		{
			hit_box_.x += them.vx_ / 2;
		}
		else
		{
			const bool going_down = them.vy_ > 0;
			const bool y_condition =
				( them.hit_box_.x < rightSubPixels() - 4000 && them.rightSubPixels() > hit_box_.x + 4000 )
				&&
					(
						( going_down && !blocksInTheWay( blocks, getBottomBoundarySpace() ) && !spritesOnOtherSide( sprites, getBottomBoundarySpace() ) )
						|| ( !going_down && !blocksInTheWay( blocks, getTopBoundarySpace() ) && !spritesOnOtherSide( sprites, getTopBoundarySpace() ) )
					);
			if ( y_condition )
			{
				hit_box_.y += them.vy_ / 2;
			}
		}

		them.collideStopAny( their_collision );
	}
};

void TopDownMoveableBlockSprite::reset()
{
	if ( top_speed_ == 0 )
	{
		hit_box_ = original_hit_box_;
	}
}

bool TopDownMoveableBlockSprite::spritesOnOtherSide( const SpriteSystem& sprites, const sdl2::SDLRect boundary ) const
{
	const auto& sprites_list = sprites.getSpritesList();
	for ( const auto& sprite : sprites_list )
	{
		const bool condition = sprite.get() != this
			&& !sprite->hasType( SpriteType::HERO )
			&& sprite->inBox( boundary );
		if ( condition )
		{
			return true;
		}
	}
	return false;
}

sdl2::SDLRect TopDownMoveableBlockSprite::getLeftBoundarySpace() const
{
	return { hit_box_.x, hit_box_.y + 4000, 1000, hit_box_.h - 8000 };
}

sdl2::SDLRect TopDownMoveableBlockSprite::getRightBoundarySpace() const
{
	return { rightSubPixels() - 1000, hit_box_.y + 4000, 1000, hit_box_.h - 8000 };
}

sdl2::SDLRect TopDownMoveableBlockSprite::getTopBoundarySpace() const
{
	return { hit_box_.x + 3000, hit_box_.y, hit_box_.w - 6000, 1000 };
}

sdl2::SDLRect TopDownMoveableBlockSprite::getBottomBoundarySpace() const
{
	return { hit_box_.x + 3000, bottomSubPixels() - 1000, hit_box_.w - 6000, 1000 };
}

inline bool TopDownMoveableBlockSprite::blocksInTheWay( const BlockSystem& blocks, const sdl2::SDLRect boundary )
{
	return blocks.blocksInTheWayExcept( boundary, BlockComponent::Type::HOLE );
};
