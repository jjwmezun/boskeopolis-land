#pragma once

#include "game_state.hpp"
#include "option_selection.hpp"
#include "text_obj_basic.hpp"
#include <vector>

class ControlsOptionState : public GameState
{
	public:
		ControlsOptionState();
		~ControlsOptionState();

		void stateUpdate() override;
		void stateRender() override;
		void init() override;


	private:
		TextObjBasic title_;
		std::vector<TextObjBasic> option_names_;
		std::vector<TextObjBasic> key_names_;
		std::vector<TextObjBasic> button_names_;
		OptionSelection selection_;

		void updateSelection();
		void updateInput();
		void renderOptions() const;
};
