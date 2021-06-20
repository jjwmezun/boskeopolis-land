#pragma once

#include "sprite.hpp"

class BelligerantCitizenSprite final : public Sprite
{
	public:
		BelligerantCitizenSprite( int x, int y );
		~BelligerantCitizenSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
        void reset() override;

    private:
        bool is_up_;
        int timer_;
        char scratch_[3000];
};
