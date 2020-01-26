#pragma once

#include "game_state.hpp"
#include "input.hpp"
#include "text_obj_basic.hpp"

class ControlsOptionPromptState : public GameState
{
	public:
		ControlsOptionPromptState( Input::Action action );
		~ControlsOptionPromptState();

		void stateUpdate() override;
		void stateRender() override;
		void init() override;

	private:
		Input::Action action_;
		TextObjBasic text_;
		sdl2::SDLRect bg_;
		sdl2::SDLRect shadow_;
};
