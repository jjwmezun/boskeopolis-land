#ifndef LIL_PIPE_MONSTER_SPRITE_H
#define LIL_PIPE_MONSTER_SPRITE_H

class Block;
class Input;

#include "sprite.h"

class LilPipeMonsterSprite : public Sprite
{
	public:
		LilPipeMonsterSprite( int x, int y, Direction::Vertical direction );
		~LilPipeMonsterSprite();
		void customUpdate( const Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap );
		
	private:
		const Direction::Vertical original_direction_y_;
};

#endif // LIL_PIPE_MONSTER_SPRITE_H