#pragma once

class Block;
class Sprite;

#include "sprite.hpp"

class PlayerCartSprite : public Sprite
{
	public:
		PlayerCartSprite( int x, int y );
		~PlayerCartSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
		void deathAction( const Camera& camera, EventSystem& events, const Map& lvmap ) override;

	private:
		void updateGraphics();

		bool reached_height_;
		const DuckData duck_data_;
		const UnDuckData unduck_data_;
};
