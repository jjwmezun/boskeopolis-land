#pragma once

#include "sprite.hpp"
#include "sprite_graphics.hpp"

class DungeonEnemySpreaderSprite : public Sprite
{
	public:
		DungeonEnemySpreaderSprite( int x, int y );
		~DungeonEnemySpreaderSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;
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
