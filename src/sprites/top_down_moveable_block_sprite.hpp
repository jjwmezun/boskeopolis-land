#pragma once

#include "sprite.hpp"

class TopDownMoveableBlockSprite final : public Sprite
{
	public:
		TopDownMoveableBlockSprite( int x, int y );
		~TopDownMoveableBlockSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
		void reset() override;

	private:
		bool blocksInTheWay( const BlockSystem& blocks, const sdl2::SDLRect boundary );
		bool spritesOnOtherSide( const SpriteSystem& sprites, const sdl2::SDLRect boundary ) const;
		sdl2::SDLRect getLeftBoundarySpace() const;
		sdl2::SDLRect getRightBoundarySpace() const;
		sdl2::SDLRect getTopBoundarySpace() const;
		sdl2::SDLRect getBottomBoundarySpace() const;
};
