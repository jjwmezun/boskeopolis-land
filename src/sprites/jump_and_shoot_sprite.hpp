#pragma once

#include "sprite.hpp"
#include "timer_random.hpp"

class SpriteSystem;

class JumpAndShootSprite : public Sprite
{
	public:
		JumpAndShootSprite( int x, int y, int w, int h, std::unique_ptr<SpriteGraphics>&& gfx, void ( *throw_function )( const Sprite& me, SpriteSystem& sprites ) );
		~JumpAndShootSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events );

	private:
		void handleMovement( const BlockSystem& blocks );
		void handleJumping();
		void handleThrowing( SpriteSystem& sprites );

		TimerRandom<32, 8> move_timer_;
		TimerRandom<80, 10> throw_timer_;
		TimerRandom<180, 30> jump_timer_;
		void ( *throw_function_ )( const Sprite& me, SpriteSystem& sprites );
		Direction::Horizontal move_direction_;
};
