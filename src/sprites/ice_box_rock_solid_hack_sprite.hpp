#pragma once

#include "sprite.hpp"

class IceBoxRockSolidHackSprite final : public Sprite
{
	public:
		IceBoxRockSolidHackSprite( int x, int y );
		~IceBoxRockSolidHackSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

	private:
		static constexpr int NUMBER_OF_EXTRA_SOLIDS = 7;
		const sdl2::SDLRect extra_solids_ [ NUMBER_OF_EXTRA_SOLIDS ];
};