#pragma once

#include "sprite.hpp"

class CapnClearbeardSprite final : public Sprite
{
	public:
		CapnClearbeardSprite( int x, int y );
		~CapnClearbeardSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

	private:
		int jump_timer_;
		int something1;
		int something2;
		int something3;
		int something4;
		int something5;
		bool move_;
};
