#pragma once

#include "sprite.hpp"

class TrafficSpawnerSprite final : public Sprite
{
	public:
		TrafficSpawnerSprite( int x, int y );
		~TrafficSpawnerSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

    private:
        int timer_;
        int target_;
        char scratch_[3000];
};
