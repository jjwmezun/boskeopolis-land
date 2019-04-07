#pragma once

class Block;

#include "sprite.hpp"
#include "timer_simple_t.hpp"

class SewerMonsterSprite : public Sprite
{
	public:
		SewerMonsterSprite( int x, int y );
		~SewerMonsterSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events );

	private:
		static constexpr int GRAPHIC_WIDTH = 80;
		static constexpr int FRAME_SPEED = 2;
		static constexpr int NUM_O_WAKING_FRAMES = 12;
		static constexpr int NUM_O_ATTACK_FRAMES = 6;
		static constexpr int NUM_O_FALLING_ASLEEP_FRAMES = 9;
		static constexpr int WAKING_FRAMES[ NUM_O_WAKING_FRAMES + 1 ] = { 1, 2, 3, 4, 4, 4, 4, 5, 6, 7, 8, 9, 9 };
		static constexpr int FALLING_ASLEEP_FRAMES[ NUM_O_FALLING_ASLEEP_FRAMES + 1 ] = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 1 };

		enum class MonsterState
		{
			SLEEPING,
			WAKING,
			ATTACK,
			FALLING_ASLEEP
		};

		MonsterState state_;
		TimerSimpleT<NUM_O_WAKING_FRAMES * FRAME_SPEED, false> waking_timer_;
		TimerSimpleT<NUM_O_ATTACK_FRAMES * FRAME_SPEED, false> attack_timer_;
		TimerSimpleT<NUM_O_FALLING_ASLEEP_FRAMES * FRAME_SPEED, false> falling_asleep_timer_;

		void stateGraphics();
		int getFrame( int tick ) const;
		int getXImg( int frame ) const;
};
