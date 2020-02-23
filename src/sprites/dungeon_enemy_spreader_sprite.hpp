#pragma once

#include "sprite.hpp"
#include "sprite_graphics.hpp"

class DungeonEnemySpreaderSprite : public Sprite
{
	public:
		DungeonEnemySpreaderSprite( int x, int y );
		~DungeonEnemySpreaderSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
		void render( Camera& camera, bool priority = false ) override;

	private:
		static constexpr int NUMBER_OF_SPIKES = 8;

		void updateInvincibility();
		void hurt();
		bool invincibilityFlickerOff() const;

		sdl2::SDLRect spikes_[ NUMBER_OF_SPIKES ];
		double spike_angles_[ NUMBER_OF_SPIKES ];
		double spike_radius_;
		double spike_rotation_;
		double spread_speed_;
		TimerSimpleT<48, false> invincibility_timer_;
		int hp_;
		bool spikes_going_out_;
};
