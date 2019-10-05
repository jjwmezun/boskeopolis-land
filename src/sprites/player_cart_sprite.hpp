#pragma once

class Block;
class Sprite;

#include "sprite.hpp"

class PlayerCartSprite : public Sprite
{
	public:
		PlayerCartSprite( int x, int y );
		~PlayerCartSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;
		void deathAction( const Camera& camera, EventSystem& events, const Map& lvmap ) override;

	private:
		void unduck();
		void updateGraphics();

		bool reached_height_;
		const DuckData duck_data_;
};
