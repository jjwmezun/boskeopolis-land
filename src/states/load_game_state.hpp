#pragma once

#include "game_state.hpp"
#include "save.hpp"
#include <vector>

class LoadGameState : public GameState
{
	public:
		LoadGameState();
		~LoadGameState();
        LoadGameState( const LoadGameState& ) = delete;
        LoadGameState( LoadGameState&& ) = delete;
        LoadGameState& operator=( const LoadGameState& ) = delete;
        LoadGameState& operator=( LoadGameState&& ) = delete;

		void stateUpdate() override;
		void stateRender() override;
		void init() override;

	protected:
		std::vector<Save> saves_;
        char scratch_[ 3000 ];
};
