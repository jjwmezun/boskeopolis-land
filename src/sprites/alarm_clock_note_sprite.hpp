#pragma once

#include "sprite.hpp"

class AlarmClockNoteSprite final : public Sprite
{
	public:
		AlarmClockNoteSprite( int x, int y, Direction::Horizontal direction );
		~AlarmClockNoteSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
};
