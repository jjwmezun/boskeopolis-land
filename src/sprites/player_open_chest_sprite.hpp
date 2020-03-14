#pragma once

#include "sprite.hpp"

class PlayerOpenChestSprite final : public Sprite
{
	public:
		PlayerOpenChestSprite( int x, int y, Direction::Horizontal dir );
		~PlayerOpenChestSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

	private:
		enum class POCState
		{
			INIT,
			START_MOVING,
			MOVE_LEFT,
			MOVE_RIGHT,
			STOP,
			OPENING,
			WALKING_AWAY,
			TURNING_TOWARD_CHEST,
			CHEST_OPENING,
			RISING_TREASURE,
			CELEBRATING
		};
		POCState state_;
		int timer_;

		bool tooFarLeft( const Sprite& them );
		bool tooFarRight( const Sprite& them );
		bool animationChange() const;
		int getAnimationFrame( int speed = Unit::DEFAULT_ANIMATION_SPEED ) const;
		void walkAnimation();
};