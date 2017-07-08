#ifndef SPRITE_STATUS_H
#define SPRITE_STATUS_H

class SpriteGraphics;

#include "timers/timer_simple.h"

class SpriteStatus
{
	public:
		SpriteStatus( int max_hp = 2, int hp = -1 );

		static const int OXYGEN_LIMIT = 720;

		void update( Sprite& sprite, SpriteGraphics& graphics );
		void reset();

		void kill();
		bool isDead() const;

		int hp() const;
		void hurt( int amount = 1 );
		void heal( int amount = 1 );
		void fullHeal();

		bool drowned() const;
		double oxygenPercent() const;

	private:
		const int max_hp_;
		int hp_;
		bool dead_;

		bool invincible_;
		TimerSimple invincibility_timer_;

		int oxygen_meter_;
};

#endif // SPRITE_STATUS_H
