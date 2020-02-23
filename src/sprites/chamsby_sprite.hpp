#pragma once

#include "sprite.hpp"
#include "text_obj.hpp"
#include "timer_random.hpp"

class ChamsbySprite : public Sprite
{
	public:
		ChamsbySprite( int x, int y );
		~ChamsbySprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

	private:
		enum class ChamsbyState
		{
			INTRO,
			ATTACK,
			HIT,
			DEFEATED,
			RUNNING
		};

		int health_;
		int invincibility_;
		TimerRandom<100, 10> walk_timer_;
		TimerRandom<10, 30> shoot_timer_;
		TextObj name;
		int health_timer_;
		ChamsbyState state_;
		int saflkjsadlkfj;
		int kjfjklkj;
		int sdfakljaslfkdj;
};
