#pragma once

#include "sprite.hpp"
#include <vector>

class TopDownPlayerSprite final : public Sprite
{
	public:
		TopDownPlayerSprite( int x, int y );
		~TopDownPlayerSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

	private:
		void handleMovement();
		void handleShooting( SpriteSystem& sprites );
		void testDirectionsList( std::vector<Direction::Simple>&& directions );
		bool testForMovement( Direction::Simple direction );
		void shoot( SpriteSystem& sprites, Direction::Simple direction );
		void updateGraphics();

		int animation_timer_;
		int shoot_animation_timer_;
		bool is_shooting_;
};
