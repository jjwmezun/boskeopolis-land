#pragma once

#include "sprite.hpp"

class PolloDelAireSprite : public Sprite
{
	public:
		PolloDelAireSprite( int x, int y, Direction::Horizontal dir_x, Direction::Vertical dir_y = Direction::Vertical::__NULL );
		~PolloDelAireSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;
		void reset() override;
		void deathAction( Camera& camera, EventSystem& events ) override;

		static void polloInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, Sprite& me );
		static void polloDeath( Camera& camera, Sprite& me );
		static void polloReset( Sprite& me );

	private:
		bool vertical_too_;
		Direction::Horizontal switch_from_;
		int animation_counter_;
};