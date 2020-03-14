#ifndef LIL_PIPE_MONSTER_SPRITE_H
#define LIL_PIPE_MONSTER_SPRITE_H

class Block;


#include "sprite.hpp"

class LilPipeMonsterSprite final : public Sprite
{
	public:
		LilPipeMonsterSprite( int x, int y, Direction::Vertical direction );
		~LilPipeMonsterSprite();
		void customUpdate( LevelState& level_state );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state );
		
	private:
		const Direction::Vertical original_direction_y_;
};

#endif // LIL_PIPE_MONSTER_SPRITE_H