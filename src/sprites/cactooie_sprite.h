#ifndef CACTOOIE_SPRITE_H
#define CACTOOIE_SPRITE_H

class Block;


#include "sprite.h"
#include "timer_simple.h"

class CactooieSprite : public Sprite
{
	public:
		CactooieSprite( int x, int y );
		~CactooieSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap );
		
	private:
		enum class CactooieState
		{
			STILL,
			ROLLING,
			SPITTING
		};
		CactooieState state_;
		
		void updateGraphics();
		bool rollFinished() const;
		
		TimerSimple still_delay_;
		TimerSimple spitting_delay_;
};

#endif // CACTOOIE_SPRITE_H
