#pragma once

#include "sprite.hpp"

class WallCrawlerSprite : public Sprite
{
	public:
		WallCrawlerSprite( int x, int y );
		~WallCrawlerSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;
        void render( Camera& camera, bool priority ) override;

    private:
        sdl2::SDLRect getFirstRect() const;
        sdl2::SDLRect getSecondRect() const;

        int fall_delay_;
        int asfnkasdnfkjsn;
        double alnmflksdmfkl;
        double klmnwlkrmlwkerm;
        double alkmflksdmf;
        double klmlkmlkm;
        double alkmdsflksmdlkf;
        double alkdmsflksdmfl;
        double asdknflskanflsmf;
};
