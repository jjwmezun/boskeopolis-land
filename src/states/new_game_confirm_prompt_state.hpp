#pragma once

#include "game_state.hpp"

class NewGameConfirmPromptState : public GameState
{
	public:
		NewGameConfirmPromptState( std::u32string save_name );
		~NewGameConfirmPromptState();
        NewGameConfirmPromptState( const NewGameConfirmPromptState& ) = delete;
        NewGameConfirmPromptState( NewGameConfirmPromptState&& ) = delete;
        NewGameConfirmPromptState& operator=( const NewGameConfirmPromptState& ) = delete;
        NewGameConfirmPromptState& operator=( NewGameConfirmPromptState&& ) = delete;

		void stateUpdate() override;
		void stateRender() override;
		void init() override;

	protected:
        std::u32string save_name_;
		bool confirm_;
		bool error_;
        char scratch_[ 3000 ];
};
