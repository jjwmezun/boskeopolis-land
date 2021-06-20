#pragma once

class Block;

#include "sprite.hpp"

class BullSprite final : public Sprite
{
	public:
		static BullSprite* generateRegular( int x, int y, Direction::Horizontal dir );
		static BullSprite* generateWaking( int x, int y, Direction::Horizontal dir );
		static BullSprite* generateSuper( int x, int y, Direction::Horizontal dir );
		~BullSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

	private:
		enum class BullState
		{
			SLEEPING,
			AWAKE
		};
		BullSprite( int x, int y, Direction::Horizontal dir, BullState bull_state, int start_speed, int top_speed, CameraMovement camera_movement );
		BullState bull_state_;
		char scratch_[3000];
};
