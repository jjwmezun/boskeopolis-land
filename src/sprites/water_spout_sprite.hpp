#pragma once

#include "sprite.hpp"

class WaterSpoutSprite : public Sprite
{
	public:
		WaterSpoutSprite( int x, int y );
		~WaterSpoutSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

	private:
		enum class SpoutState
		{
			DORMANT,
			RISE,
			TOP,
			FALL
		};

		inline void adjustHitBox()
		{
			hit_box_.y = original_hit_box_.y - hit_box_.h;
		};

		int delay_timer_;
		int grow_speed_;
		SpoutState state_;
		Sprite* barrel_;
};
