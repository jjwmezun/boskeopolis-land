#pragma once

#include "game_state.hpp"

class NewGameState : public GameState
{
	public:
		NewGameState();
		~NewGameState();
        NewGameState( const NewGameState& ) = delete;
        NewGameState( NewGameState&& ) = delete;
        NewGameState& operator=( const NewGameState& ) = delete;
        NewGameState& operator=( NewGameState&& ) = delete;

		void stateUpdate() override;
		void stateRender() override;
		void init() override;

	protected:
        std::u32string name_;
        char scratch_[ 3000 ];
};
