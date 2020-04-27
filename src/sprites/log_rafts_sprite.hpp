#pragma once

#include "sprite.hpp"

class LogRaftsSprite final : public Sprite
{
	public:
		LogRaftsSprite( int x, int y );
		~LogRaftsSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
        void render( Camera& camera, bool priority ) override;

    private:
        static constexpr int MAX_RAFTS = 5;
        sdl2::SDLRect rafts_[ MAX_RAFTS ];
        char scratch_[ 2500 ];
};
