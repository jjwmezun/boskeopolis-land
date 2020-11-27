#pragma once

#include "sprite.hpp"

class CuckooSprite final : public Sprite
{
	public:
		CuckooSprite( int x, int y );
		~CuckooSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

    private:
        enum class Status
        {
            DORMANT,
            POPPING_OUT,
            OUT,
            REELING_IN,
            RESTING
        };
        Status status_;
        int timer_;
        char scratch_[3000];
};
