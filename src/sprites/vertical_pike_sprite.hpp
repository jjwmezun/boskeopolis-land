#pragma once

#include "sprite.hpp"

class VerticalPikeSprite final : public Sprite
{
	public:
		VerticalPikeSprite( int x, int y, Direction::Vertical direction );
		~VerticalPikeSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
		void render( Camera& camera, bool priority ) override;

	private:
		void accelerateGrowth();
		void setVerticalY();

		enum class PikeState
		{
			WAIT,
			GROW,
			SHRINK
		};

		int grow_speed_;
		PikeState state_;
		int wait_timer_;
};
