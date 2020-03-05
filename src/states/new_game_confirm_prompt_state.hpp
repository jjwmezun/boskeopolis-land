#pragma once

#include "prompt_state.hpp"

class NewGameConfirmPromptState : public PromptState
{
	public:
		NewGameConfirmPromptState( Save& save, std::u32string question, bool& confirm );
		~NewGameConfirmPromptState();
        NewGameConfirmPromptState( const NewGameConfirmPromptState& ) = delete;
        NewGameConfirmPromptState( NewGameConfirmPromptState&& ) = delete;
        NewGameConfirmPromptState& operator=( const NewGameConfirmPromptState& ) = delete;
        NewGameConfirmPromptState& operator=( NewGameConfirmPromptState&& ) = delete;

		void stateUpdate() override;

	protected:
		bool error_;
		Save& save_;
		std::u32string question_;
		bool& confirm_;
        char scratch_[ 3000 ];
};
