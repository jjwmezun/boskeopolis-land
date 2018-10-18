#pragma once

#include "bump_under_block_sprite.hpp"
#include "sprite.hpp"

class MessageBlockSprite : public Sprite
{
	public:
		MessageBlockSprite( int x, int y );
		~MessageBlockSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;

	private:
		bool is_being_bumped_;
		BumpUnderBlockSprite bump_under_block_component;

		inline bool isBackInPlace() const
		{
			return hit_box_.y == original_hit_box_.y;
		};
};
