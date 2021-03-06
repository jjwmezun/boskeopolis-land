#pragma once

#include "sprite.hpp"

class PolloPicanteSprite final : public Sprite
{
	public:
		friend class PolloDelAireSprite;
		PolloPicanteSprite( int x, int y, Direction::Horizontal direction );
		~PolloPicanteSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
		void deathAction( LevelState& level_state ) override;
		void reset() override;

	private:
		int animation_counter_;
        int shoot_timer_;
};
