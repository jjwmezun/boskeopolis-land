#pragma once

#include "sprite_movement.hpp"

class GroundedSpriteMovement : public SpriteMovement
{
	public:
		GroundedSpriteMovement();

		virtual void moveUp( Sprite& sprite ) const;
		virtual void moveDown( Sprite& sprite ) const;
		virtual void jump( Sprite& sprite ) const;
		virtual void bounce( Sprite& sprite, int amount ) const;
		virtual void position( Sprite& sprite ) const;
		void collideStopYBottom( Sprite& sprite, int overlap ) const;
		void collideStopYTop( Sprite& sprite, int overlap ) const;
		void collideStopAny( Sprite& sprite, const Collision& collision ) const;
		const Collision testBlockCollision( const Sprite& me, const sdl2::SDLRect& them ) const;

	protected:
		GroundedSpriteMovement( Type type );

	private:
		void startJump( Sprite& sprite ) const;
};
