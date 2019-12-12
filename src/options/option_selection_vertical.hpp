#pragma once

#include "option_selection.hpp"

class OptionSelectionVertical : public OptionSelection
{
	public:
		OptionSelectionVertical( int max, int not_allowed = -1, int current = 0 );
		~OptionSelectionVertical();

	protected:
		bool customUpdate() override;
		void shoveForward();

	private:
		int max_;

		void selectUp();
		void selectDown();
};
