#pragma once

#include "sprite.hpp"

class RugMonsterSprite final : public Sprite
{
	public:
		RugMonsterSprite( int x, int y );
		~RugMonsterSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
        void deathAction( const Camera& camera, EventSystem& events, const Map& lvmap ) override;
    
    private:
        enum class RugMonsterState
        {
            DORMANT,
            RISING,
            UP,
            FALLING,
            CHANGING
        };
        bool reached_end_;
        int animation_timer_;
        RugMonsterState state_;
        int current_animation_frame_;
        double fakjndfkjsnaf;
        double akjfndskjfn;
        double aflkndlaskmf;
};
