#pragma once

#include "game_state.hpp"

class OptionsState : public GameState
{
	public:
		OptionsState();
		~OptionsState();

		void stateUpdate() override;
		void stateRender() override;
		void init() override;
};
