#pragma once

#include "sprite.hpp"

class PikeSprite : public Sprite
{
	public:
		PikeSprite( int x, int y );
		~PikeSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;
		void render( Camera& camera, bool priority ) override;

	private:
		void accelerateGrowth();
		void setHorizontalX();
		void setVerticalY();
		int getOriginY() const;

		enum class PikeState
		{
			WAIT,
			GROW,
			SHRINK
		};

		int grow_speed_;
		PikeState state_;
		int wait_timer_;
		sdl2::SDLRect horizontal_pike_;
		int wrehrkjwe;
		int afklhsdlkfjsad;
		int afkljsdlkfj;
		int aklsfjhlksajf;
		int aklfjsadfj;
};
