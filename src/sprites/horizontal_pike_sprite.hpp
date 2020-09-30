#pragma once

#include "sprite.hpp"

class HorizontalPikeSprite final : public Sprite
{
	public:
		HorizontalPikeSprite( int x, int y, Direction::Horizontal direction );
		~HorizontalPikeSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
		void render( const Camera& camera ) const override;

	private:
		void accelerateGrowth();
		void setHorizontalX();

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
