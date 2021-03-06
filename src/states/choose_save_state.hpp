#pragma once

#include "checkered_background.hpp"
#include "game_state.hpp"
#include "save.hpp"
#include <vector>
#include "wtext_highlight.hpp"

class ChooseSaveState final : public GameState
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
		void backFromPop() override;

	private:
		int maxSelection() const;
		bool nameLessThanLimit() const;
		void exitNaming();
		bool testNameAlreadyInUse( const std::u32string& name ) const;

		enum class State
		{
			SELECT,
			NAMING,
			COPY,
			CREATING_NEW_SAVE,
			CREATING_COPY_SAVE,
			LOADING,
			DELETING
		};

		std::vector<Save> saves_;
		int selection_;
		State state_;
		std::u32string name_;
		int timer_;
		WTextHighlight<> title_;
		CheckeredBackground bg_;
		int bottom_selection_;
		Save temp_save_;
		bool new_save_confirm_;
};
