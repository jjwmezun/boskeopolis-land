#pragma once

#include "counter_t.hpp"
#include "player_sprite.hpp"
#include "timer_repeat.hpp"

class ShooterPlayerSprite final : public PlayerSprite
{
	public:
		ShooterPlayerSprite( int x, int y, std::string&& tileset = "sprites/cowgirl_autumn.png" );
		~ShooterPlayerSprite();
		void customUpdate( LevelState& level_state ) override;
		void deathAction( LevelState& level_state ) override;

	private:
		bool is_shooting_;
		bool is_shooting_up_;
		int shoot_delay_count_;
		int is_shooting_count_;
		CounterT<3, 0, 0, true> walk_counter_;
		CounterT<0, 1, 0, true> climb_counter_;
		TimerRepeat animation_timer_;

		void updateGFX();
};
