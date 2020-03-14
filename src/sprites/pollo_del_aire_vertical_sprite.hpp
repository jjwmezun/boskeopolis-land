#pragma once

#include "sprite.hpp"

class PolloDelAireVerticalSprite final : public Sprite
{
	public:
		friend class PolloDelAireSprite;
		PolloDelAireVerticalSprite( int x, int y, Direction::Vertical dir_y, int map_id, bool despawn_when_dead = false );
		~PolloDelAireVerticalSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
		void deathAction( const Camera& camera, EventSystem& events, const Map& lvmap ) override;
		void reset() override;

	private:
		int animation_counter_;
};
