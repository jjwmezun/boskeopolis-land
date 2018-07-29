#pragma once

#include "input.hpp"

class OptionSelection
{
	public:
		OptionSelection( int max, int not_allowed = -1 );
		~OptionSelection();

		void update();
		void setNotAllowed( int not_allowed );
		int selection() const;
		int previousSelection() const;
		void setSelect( int value );


	private:
		const int max_;
		int selection_;
		int previous_selection_;
		int timer_;
		int not_allowed_;

		void selectUp();
		void selectDown();
		bool testInput( Input::Action input ) const;
};
