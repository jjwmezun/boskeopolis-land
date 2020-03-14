#ifndef BOUNCY_CLOUD_SPRITE_H
#define BOUNCY_CLOUD_SPRITE_H

class Block;


#include "sprite.hpp"

class BouncyCloudBlockSprite final : public Sprite
{
	public:
		BouncyCloudBlockSprite( int x, int y );
		~BouncyCloudBlockSprite();
		void customUpdate( LevelState& level_state );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state );
};

#endif // BOUNCY_CLOUD_SPRITE_H