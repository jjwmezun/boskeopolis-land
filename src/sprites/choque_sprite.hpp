#pragma once

class Block;

#include "sprite.hpp"

class ChoqueSprite final : public Sprite
{
	public:
		ChoqueSprite( int x, int y );
		~ChoqueSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
		void reset() override;
		void render( Camera& camera, bool priority = false ) override;

	private:	
		const sdl2::SDLRect src_;
		sdl2::SDLRect dest_eye_left_;
		sdl2::SDLRect dest_eye_right_;
		Direction::Rotation eye_placement_;
		bool still_;
};