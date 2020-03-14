#ifndef CACTOOIE_SPRITE_H
#define CACTOOIE_SPRITE_H

class Block;


#include "sprite.hpp"
#include "timer_simple.hpp"

class CactooieSprite final : public Sprite
{
	public:
		CactooieSprite( int x, int y );
		~CactooieSprite();
		void customUpdate( LevelState& level_state );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state );
		
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
