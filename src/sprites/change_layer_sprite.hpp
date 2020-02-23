#pragma once

#include "sprite.hpp"

class ChangeLayerSprite : public Sprite
{
	public:
		~ChangeLayerSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

		static ChangeLayerSprite* makeForeground( int x );
		static ChangeLayerSprite* makeBackground( int x );
		static ChangeLayerSprite* makeBackgroundDuo( int x, int y );

	private:
		int flash_timer_;
		int frame_size_;
		bool activated_;

		ChangeLayerSprite( int x, int y, int w, int h, int frame_size, Direction::Vertical dir, std::string texture );
};