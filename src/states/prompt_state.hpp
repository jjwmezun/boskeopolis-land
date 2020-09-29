#pragma once

#include "game_state.hpp"

class PromptState : public GameState
{
	public:
		PromptState( std::u32string text, int selection = -1 );
		~PromptState();
        PromptState( const PromptState& ) = delete;
        PromptState( PromptState&& ) = delete;
        PromptState& operator=( const PromptState& ) = delete;
        PromptState& operator=( PromptState&& ) = delete;

		void stateRender() override;
		void init() override;
		virtual void stateUpdate() override;
        virtual void extraRender();

	protected:
        int selection_;
        std::u32string text_;
};
