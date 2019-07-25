#pragma once

#include "sprite.hpp"
#include "text_obj.hpp"
#include "timer_random.hpp"

class ChamsbySprite : public Sprite
{
	public:
		ChamsbySprite( int x, int y );
		~ChamsbySprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;
		void render( Camera& camera, bool priority );

	private:
		int health_;
		int invincibility_;
		TimerRandom<100, 10> walk_timer_;
		TimerRandom<10, 30> shoot_timer_;
		int start_timer_;
		TextObj name;
		int asflkjhsal;
		int saflkjsadlkfj;
		int kjfjklkj;
		int sdfakljaslfkdj;
};
