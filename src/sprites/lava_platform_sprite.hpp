#pragma once

#include "sprite.hpp"

class LavaPlatformSprite final : public Sprite
{
	public:
		LavaPlatformSprite( int x, int y );
		~LavaPlatformSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

	private:
		bool activated_;
};
