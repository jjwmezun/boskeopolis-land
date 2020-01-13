#pragma once

#include "sprite.hpp"
#include "timer_random.hpp"

class FrogSprite : public Sprite
{
	public:
		FrogSprite( int x, int y );
		~FrogSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;
		void render( Camera& camera, bool priority ) override;
		void deathAction( const Camera& camera, EventSystem& events, const Map& lvmap ) override;

    private:
		void setTongueX();
		void setTongueY();

        enum class FrogState
        {
            STATIONARY,
			JUMPING,
			LICKING,
			LICKING_BACK
        };
        TimerRandom<40, 10> jump_timer_;
        sdl2::SDLRect tongue_;
        FrogState state_;
        int tongue_speed_;
};
