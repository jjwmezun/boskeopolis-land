#pragma once

#include "player_sprite.hpp"

class ShooterPlayerSprite : public PlayerSprite
{
	public:
		ShooterPlayerSprite( int x, int y );
		~ShooterPlayerSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void duck() override;
		void unduck( const BlockSystem& blocks ) override;

	
		void deathAction( Camera& camera ) override;

	private:
		bool is_shooting_;
		bool is_shooting_up_;
		int shoot_delay_count_;
		int is_shooting_count_;

		TimerRepeat animation_timer_;
		Counter walk_counter_;
		Counter climb_counter_;

		void updateGFX();
};