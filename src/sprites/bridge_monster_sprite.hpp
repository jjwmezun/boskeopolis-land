#pragma once

#include "sprite.hpp"
#include "timer_random.hpp"

class BridgeMonsterSprite final : public Sprite
{
	public:
		BridgeMonsterSprite( int x, int y );
		~BridgeMonsterSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

    private:
        TimerRandom<120, 40> jump_timer_;
        TimerRandom<60, 20> walk_timer_;
        bool shoot_;
        double aksjdnfks;
        double akjnfkjlasdnf;
        double akfjndksfn;
        double akjfndskjnf;
        double anflkflksa;
        double asfdsdf;
        double afdsfsafd;
        double affsdfsdfdsf;
};
