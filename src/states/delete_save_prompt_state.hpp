#pragma once

#include "prompt_state.hpp"

class Save;

class DeleteSavePromptState : public PromptState
{
	public:
		DeleteSavePromptState( Save& save );
		~DeleteSavePromptState();
        DeleteSavePromptState( const DeleteSavePromptState& ) = delete;
        DeleteSavePromptState( DeleteSavePromptState&& ) = delete;
        DeleteSavePromptState& operator=( const DeleteSavePromptState& ) = delete;
        DeleteSavePromptState& operator=( DeleteSavePromptState&& ) = delete;

		void stateUpdate() override;

	private:
        Save& save_;
};
