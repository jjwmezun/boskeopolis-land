#ifndef SEWER_MONSTER_SPRITE_H
#define SEWER_MONSTER_SPRITE_H

class Block;
class Input;

#include "sprite.h"

class SewerMonsterSprite : public Sprite
{
	public:
		SewerMonsterSprite( int x, int y );
		~SewerMonsterSprite();
		void customUpdate( const Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap );

	private:
		static constexpr int GRAPHIC_WIDTH = 80;
		static constexpr int FRAME_SPEED   = 2;
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
		
		TimerSimple waking_timer_;
		TimerSimple attack_timer_;
		TimerSimple falling_asleep_timer_;
		
		void stateGraphics();
		int getFrame( int tick ) const;
		int getXImg( int frame ) const;
};

#endif // SEWER_MONSTER_SPRITE_H