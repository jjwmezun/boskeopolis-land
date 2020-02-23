#ifndef CACTOOIE_SPINE_SPRITE_H
#define CACTOOIE_SPINE_SPRITE_H

class Block;


#include "sprite.hpp"

class CactooieSpineSprite : public Sprite
{
	public:
		CactooieSpineSprite( int x, int y, Direction::Horizontal direction );
		~CactooieSpineSprite();
		void customUpdate( LevelState& level_state );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state );
};

#endif // CACTOOIE_SPINE_SPRITE_H
