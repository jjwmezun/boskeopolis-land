#pragma once

#include "sprite.hpp"

class PedestrianSprite : public Sprite
{
	public:
		PedestrianSprite( int x, int y );
		~PedestrianSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;

    private:
        double lasmflsmdfl;
        double akfnsldkamflksmf;
        double aklfndsklnfl;
        double agkjlndsklfgans;
        double alkfjasldjf;
        double alfkndl;kfjnsl;
        double asdfmksalkmfla;
        double aldfmnsadlfmns;
};
