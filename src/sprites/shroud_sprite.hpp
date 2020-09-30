#pragma once

#include "counter.hpp"
#include "sprite.hpp"

class Block;

class ShroudSprite final : public Sprite
{
	public:
		ShroudSprite( int x, int y );
		~ShroudSprite();
		void customUpdate( LevelState& level_state );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state );
		void render( Camera& camera );
		
	private:
		enum class ShroudState
		{
			VISIBLE,
			HIDING,
			HIDDEN,
			RETURNING
		};
		
		static constexpr int COLOR = 5;
		static constexpr int TRANS_SPEED = 10;
	
		const sdl2::SDLRect top_;
		const sdl2::SDLRect bottom_;
		ShroudState state_;
		Counter alpha_;
		bool activated_;
};