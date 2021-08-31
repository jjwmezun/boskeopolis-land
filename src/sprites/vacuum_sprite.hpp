#pragma once

#include "sprite.hpp"

class VacuumSprite final : public Sprite
{
	public:
		static VacuumSprite* generateNormal( int x, int y );
		static VacuumSprite* generateFast( int x, int y );

		~VacuumSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

	private:
		VacuumSprite( int x, int y, int speed );
};
