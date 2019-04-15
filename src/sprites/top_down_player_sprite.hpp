#pragma once

#include "sprite.hpp"
#include <vector>

class TopDownPlayerSprite : public Sprite
{
	public:
		TopDownPlayerSprite( int x, int y );
		~TopDownPlayerSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;

	private:
		void handleMovement();
		void handleShooting( SpriteSystem& sprites );
		void testDirectionsList( std::vector<Direction::Simple>&& directions );
		bool testForMovement( Direction::Simple direction );
		void shoot( SpriteSystem& sprites, Direction::Simple direction );
		void updateGraphics();

		Direction::Simple direction_;
		int animation_timer_;
		int shoot_animation_timer_;
		bool is_shooting_;
};
