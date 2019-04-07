#pragma once

class Camera;
class FaucetSprite;
class SpriteGraphics;

#include "timer_repeat.hpp"

class FaucetGraphics
{
	public:
		FaucetGraphics();
		void update( const FaucetSprite& sprite, SpriteGraphics* graphics );
		void render( const SpriteGraphics* graphics, const Camera* camera, bool priority, int x, int y ) const;

	private:
		TimerRepeat animation_timer_;
};
