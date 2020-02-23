#pragma once

#include "sprite.hpp"

class CircleBlockSprite : public Sprite
{
	public:
		CircleBlockSprite( int x, int y );
		~CircleBlockSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

	private:
		static constexpr int NUMBER_OF_FRAMES = 12;
		const int frames_[ NUMBER_OF_FRAMES ];
		Direction::Simple direction_;
		int animation_timer_;
};
