#pragma once

#include "sprite.hpp"

class StrongerCowpokerSprite final : public Sprite
{
	public:
		StrongerCowpokerSprite( int x, int y, int map_id );
		~StrongerCowpokerSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

	private:
		bool jump_;
		bool awake_;
		bool is_shooting_;

		int hp_;
		int invincibility_;

		int move_type_;
		int move_time_;
		int move_counter_;

		int jump_time_;
		int jump_counter_;
		int is_shooting_counter_;

		int throw_time_;
		int throw_counter_;
		
		int moveTime() const;
		int moveType() const;
		void handleMovement();

		int jumpHeight() const;
		int jumpTime() const;
		void handleJumping();
		
		int throwTime() const;
		void handleThrowing( SpriteSystem& sprites );
		
		void hurt();
};