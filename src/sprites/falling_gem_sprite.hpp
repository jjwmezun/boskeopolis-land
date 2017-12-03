#pragma once

class Block;
class Camera;

#include "sprite.hpp"

class FallingGemSprite : public Sprite
{
	public:
		FallingGemSprite( int x, int y );
		~FallingGemSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;

	private:
		bool active_;
		void respawn( const Camera& camera );
		bool testActive( const Sprite& hero );
};