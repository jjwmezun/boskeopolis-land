#pragma once

#include "game_state.hpp"
#include "text_obj_basic.hpp"

class ControlsOptionPromptState : public GameState
{
	public:
		ControlsOptionPromptState();
		~ControlsOptionPromptState();

		void stateUpdate() override;
		void stateRender() override;
		void init() override;

	private:
		TextObjBasic text_;
		sdl2::SDLRect bg_;
};
