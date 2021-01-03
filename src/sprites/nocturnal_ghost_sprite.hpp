#pragma once

#include "animation_counter.hpp"
#include "sprite.hpp"
#include "timer_repeat_t2.hpp"

class NocturnalGhostSprite final : public Sprite
{
	public:
		static constexpr int NUMBER_OF_ANIMATION_FRAMES = 8;

		NocturnalGhostSprite( int x, int y );
		~NocturnalGhostSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
        void deathAction( LevelState& level_state ) override;
        void reset() override;

	private:
		static constexpr int WAVE_TIMER_LIMIT = 32;

		AnimationCounter<NUMBER_OF_ANIMATION_FRAMES> animation_counter_;
		TimerRepeatT2<WAVE_TIMER_LIMIT> wave_timer_;
		int vertical_acceleration_;
		int vertical_speed_;
        bool first_;
        bool hit_;
        char scratch_[3000];
};
