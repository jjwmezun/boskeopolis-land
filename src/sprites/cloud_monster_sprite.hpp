#pragma once

class Block;


#include "lightning_sprite.hpp"
#include "sprite.hpp"

class CloudMonsterSprite : public Sprite
{
	public:
		CloudMonsterSprite( int x, int y );
		~CloudMonsterSprite();
		void customUpdate( LevelState& level_state );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state );
		void render( Camera& camera, bool priority ) override;

	private:
		bool lightning_is_on_;
		int lightning_timer_;
		LightningSprite lightning_;

		int lightningPosition() const;
		void lightningUpdate();
};