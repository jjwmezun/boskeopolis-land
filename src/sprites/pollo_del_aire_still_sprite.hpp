#pragma once

#include "sprite.hpp"

class PolloDelAireStillSprite final : public Sprite
{
	public:
		friend class PolloDelAireSprite;
		PolloDelAireStillSprite( int x, int y, int map_id, bool despawn_when_dead = false );
		~PolloDelAireStillSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
		void deathAction( const Camera& camera, EventSystem& events, const Map& lvmap ) override;
		void reset() override;

	private:
		int animation_counter_;
};
