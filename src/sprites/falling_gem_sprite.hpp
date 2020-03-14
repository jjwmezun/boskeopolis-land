#pragma once

class Block;
class Camera;

#include "sprite.hpp"

class FallingGemSprite final : public Sprite
{
	public:
		FallingGemSprite( int x, int y );
		~FallingGemSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

	private:
		bool active_;
		void respawn( const Camera& camera );
		bool testActive( const Sprite& hero );
};