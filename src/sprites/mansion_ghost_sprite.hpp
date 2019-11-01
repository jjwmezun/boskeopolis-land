#pragma once

#include "animation_counter.hpp"
#include "sprite.hpp"
#include "timer_repeat_t2.hpp"

class MansionGhostSprite : public Sprite
{
	public:
		static constexpr int NUMBER_OF_ANIMATION_FRAMES = 8;

		MansionGhostSprite( int x, int y );
		~MansionGhostSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;
        void deathAction( const Camera& camera, EventSystem& events, const Map& lvmap ) override;

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
