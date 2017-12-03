#pragma once

class Block;

#include "sprite.hpp"

class ChoqueSprite : public Sprite
{
	public:
		ChoqueSprite( int x, int y );
		~ChoqueSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;
		void reset() override;
		void render( Camera& camera, bool priority = false ) override;

	private:	
		const sdl2::SDLRect src_;
		sdl2::SDLRect dest_eye_left_;
		sdl2::SDLRect dest_eye_right_;
		Direction::Rotation eye_placement_;
		bool still_;
};