#ifndef CACTOOIE_SPINE_SPRITE_H
#define CACTOOIE_SPINE_SPRITE_H

class Block;
class Input;

#include "sprite.h"

class CactooieSpineSprite : public Sprite
{
	public:
		CactooieSpineSprite( int x, int y, Direction::Horizontal direction );
		~CactooieSpineSprite();
		void customUpdate( const Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap );
};

#endif // CACTOOIE_SPINE_SPRITE_H
