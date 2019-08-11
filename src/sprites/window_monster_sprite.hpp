#pragma once

#include "sprite.hpp"

class WindowMonsterSprite : public Sprite
{
	public:
		WindowMonsterSprite( int x, int y );
		~WindowMonsterSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;

	private:
		bool activated_;
		int timer_;

		bool animationIsDone() const;
};
