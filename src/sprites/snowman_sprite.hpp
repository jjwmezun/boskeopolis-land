#pragma once

#include "sprite.hpp"

class SnowmanSprite : public Sprite
{
	public:
		SnowmanSprite( int x, int y );
		~SnowmanSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events );
	
	private:
		int move_type_;
		int move_time_;
		int move_counter_;

		int jump_time_;
		int jump_counter_;
		bool jump_;

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
};