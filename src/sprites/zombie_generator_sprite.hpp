#pragma once

#include "sprite.hpp"

class ZombieGeneratorSprite final : public Sprite
{
	public:
		ZombieGeneratorSprite( int x, int y );
		~ZombieGeneratorSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

    private:
        int timer_;
        bool activated_;
        int prev_x_[ 3 ];
        int player_x_;
        std::vector<int> spawn_points_;
        char scatch_[ 3000 ];

        void spawnForScreen( LevelState& level_state, int i, int max, int min );
        bool validSpawnPoint( int x ) const;
};
