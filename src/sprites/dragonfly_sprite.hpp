#pragma once

#include "sprite.hpp"

class DragonflySprite final : public Sprite
{
	public:
		DragonflySprite( int x, int y, bool vertical = false );
		~DragonflySprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
    private:
		int animation_timer_;
};
