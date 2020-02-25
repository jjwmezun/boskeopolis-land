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

	private:
		int maxSelection() const;
		bool nameLessThanLimit() const;

		enum class State
		{
			SELECT,
			NAMING
		};

		std::vector<Save> saves_;
		int selection_;
		State state_;
		std::u32string name_;
		int timer_;
        char scratch_[ 3000 ];
};
