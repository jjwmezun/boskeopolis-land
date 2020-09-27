#pragma once

#include "animation_counter.hpp"
#include "sprite.hpp"
#include "timer_repeat_t2.hpp"

class MansionGhostSprite final : public Sprite
{
	public:
		static constexpr int NUMBER_OF_ANIMATION_FRAMES = 8;

		MansionGhostSprite( int x, int y );
		~MansionGhostSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
        void deathAction( LevelState& level_state ) override;

		static int last_ghost_death_frame_;

	private:
		static constexpr int WAVE_TIMER_LIMIT = 32;

		AnimationCounter<NUMBER_OF_ANIMATION_FRAMES> animation_counter_;
		TimerRepeatT2<WAVE_TIMER_LIMIT> wave_timer_;
		int light_timer_;
		int vertical_acceleration_;
		int vertical_speed_;
		bool being_flashed_;
};
