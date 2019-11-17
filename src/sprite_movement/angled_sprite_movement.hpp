#pragma once

#include <array>
#include "sprite_movement.hpp"

class AngledSpriteMovement : public SpriteMovement
{
	public:
		AngledSpriteMovement();
		void position( Sprite& sprite ) const override;
		const Collision testCollision( const Sprite& me, const sdl2::SDLRect& them ) const override;
		void collideStopXLeft( Sprite& sprite, int overlap ) const override;
		void collideStopXRight( Sprite& sprite, int overlap ) const override;
		void collideStopYBottom( Sprite& sprite, int overlap ) const override;
		void collideStopYTop( Sprite& sprite, int overlap ) const override;

		static std::array<Line, 4> getLinesFromBox( const sdl2::SDLRect& box, double angle );
};