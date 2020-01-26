#pragma once

#include "input.hpp"

class OptionSelection
{
	public:
		OptionSelection( int current = 0, int not_allowed = -1 );
		virtual ~OptionSelection();
		void update();
		int selection() const;
		int previousSelection() const;
		void setSelect( int value );
		void setNotAllowed( int not_allowed );
		void setAllAllowed();

	protected:
		int selection_;
		int previous_selection_;
		int timer_;
		int not_allowed_;

		bool testInput( Input::Action input ) const;
		void playSound() const;
		virtual bool customUpdate() = 0;
		virtual void shoveForward() = 0;
};
