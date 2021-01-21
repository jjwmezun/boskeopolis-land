#pragma once

#include "sprite.hpp"

class GlassButtonSprite final : public Sprite
{
	public:
		GlassButtonSprite( int x, int y, int switch_number, Direction::Simple direction = Direction::Simple::LEFT );
		~GlassButtonSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

    private:
        int hits_;
		int switch_number_;
        char scratch_[3000];
};
