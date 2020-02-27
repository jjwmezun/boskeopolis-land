#pragma once

#include "game_state.hpp"
#include "save.hpp"
#include <vector>

class ChooseSaveState : public GameState
{
	public:
		ChooseSaveState();
		~ChooseSaveState();
        ChooseSaveState( const ChooseSaveState& ) = delete;
        ChooseSaveState( ChooseSaveState&& ) = delete;
        ChooseSaveState& operator=( const ChooseSaveState& ) = delete;
        ChooseSaveState& operator=( ChooseSaveState&& ) = delete;

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
