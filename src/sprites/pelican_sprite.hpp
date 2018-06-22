#pragma once

#include "sprite.hpp"

class PelicanSprite : public Sprite
{
	public:
		PelicanSprite( int x, int y );
		~PelicanSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;
		void reset() override;

	private:
		enum class State
		{
			HERE,
			GOING_AWAY,
			THERE,
			COMING_BACK
		};
		State state_;
		int timer_;
		bool stepped_on_;

		bool standOnMe( Collision& their_collision, Sprite& them );
};
