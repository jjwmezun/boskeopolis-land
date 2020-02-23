#pragma once

#include "animation_counter.hpp"
#include "sprite.hpp"

class PedestrianSprite : public Sprite
{
	public:
		PedestrianSprite( int x, int y );
		~PedestrianSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

    private:
        AnimationCounter<2> animation_;
        double akfnsldkamflksmf;
        double aklfndsklnfl;
        double agkjlndsklfgans;
        double alkfjasldjf;
        double alfkndlkfjnsl;
        double asdfmksalkmfla;
        double aldfmnsadlfmns;

        bool isAtTransitionPoint() const;
};
