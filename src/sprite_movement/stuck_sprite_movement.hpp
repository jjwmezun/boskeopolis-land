#pragma once

#include "sprite_movement.hpp"

class StuckSpriteMovement final : public SpriteMovement
{
	public:
		StuckSpriteMovement();
		void moveUp( Sprite& sprite ) const override;
		void moveDown( Sprite& sprite ) const override;
		void moveLeft( Sprite& sprite ) const override;
		void moveRight( Sprite& sprite ) const override;
		void jump( Sprite& sprite ) const override;
		void bounce( Sprite& sprite, int amount ) const override;
		void position( Sprite& sprite ) const override;
		const Collision testBlockCollision( const Sprite& me, const sdl2::SDLRect& them ) const override;
};
