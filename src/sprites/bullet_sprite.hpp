#pragma once

#include "sprite.hpp"

class BulletSprite : public Sprite
{
	public:
		BulletSprite( int x, int y, Direction::Simple dir, bool heros );
		~BulletSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health );

	private:
		const bool heros_;
		
		static std::vector<SpriteType> bulletType( bool heros );
};