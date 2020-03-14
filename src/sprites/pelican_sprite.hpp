#pragma once

#include "sprite.hpp"

class PelicanSprite final : public Sprite
{
	public:
		PelicanSprite( int x, int y );
		~PelicanSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
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
