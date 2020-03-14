#pragma once

#include "option_selection.hpp"

class OptionSelectionGrid final : public OptionSelection
{
	public:
		OptionSelectionGrid( int rows, int cols, int max );
		~OptionSelectionGrid();

	protected:
		bool customUpdate() override;
		void shoveForward();

	private:
		int rows_;
		int cols_;
        int x_;
        int y_;
		int max_;

		void selectUp();
		void selectDown();
        void selectLeft();
        void selectRight();

		constexpr bool testSelectionIsInvalid() const
		{
			return selection_ == not_allowed_ || selection_ > max_;
		};
};
