#pragma once

#include "sprite.hpp"

class WindowMonsterSprite : public Sprite
{
	public:
		WindowMonsterSprite( int x, int y );
		~WindowMonsterSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

	private:
		bool activated_;
		int timer_;

		bool animationIsDone() const;
};
