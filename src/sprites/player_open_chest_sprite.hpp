#pragma once

#include "sprite.hpp"

class PlayerOpenChestSprite : public Sprite
{
	public:
		PlayerOpenChestSprite( int x, int y, Direction::Horizontal dir );
		~PlayerOpenChestSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;

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