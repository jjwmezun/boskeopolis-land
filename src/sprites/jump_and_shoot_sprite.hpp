#pragma once

#include "sprite.hpp"
#include "timer_random.hpp"

class SpriteSystem;

class JumpAndShootSprite final : public Sprite
{
	public:
		JumpAndShootSprite( int x, int y, int w, int h, std::unique_ptr<SpriteGraphics>&& gfx, void ( *throw_function )( const Sprite& me, SpriteSystem& sprites ) );
		~JumpAndShootSprite();
		void customUpdate( LevelState& level_state );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state );

	private:
		void handleMovement( const BlockSystem& blocks );
		bool handleJumping();
		void handleThrowing( SpriteSystem& sprites );

		TimerRandom<32, 8> move_timer_;
		TimerRandom<80, 10> throw_timer_;
		TimerRandom<180, 30> jump_timer_;
		void ( *throw_function_ )( const Sprite& me, SpriteSystem& sprites );
		Direction::Horizontal move_direction_;
};
