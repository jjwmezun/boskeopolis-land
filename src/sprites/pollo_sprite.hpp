#pragma once

#include "sprite.hpp"

class PolloSprite : public Sprite
{
	public:
		PolloSprite( int x, int y, Direction::Horizontal dir_x, int map_id, bool despawn_when_dead = false );
		~PolloSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

	private:
		Direction::Horizontal switch_from_;
};
