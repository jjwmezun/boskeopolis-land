#pragma once

#include "sprite.hpp"

class PikeSprite final : public Sprite
{
	public:
		PikeSprite( int x, int y );
		~PikeSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
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
};
