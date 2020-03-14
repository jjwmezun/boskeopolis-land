#pragma once

#include "sprite.hpp"

class PiBlockSprite final : public Sprite
{
	public:
		PiBlockSprite( int x, int y );
		~PiBlockSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
		void reset() override;

	private:
		int fall_timer_;
		bool hero_still_on_block_;
};
