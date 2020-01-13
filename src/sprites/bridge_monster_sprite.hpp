#pragma once

#include "sprite.hpp"
#include "timer_random.hpp"

class BridgeMonsterSprite : public Sprite
{
	public:
		BridgeMonsterSprite( int x, int y );
		~BridgeMonsterSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;

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
