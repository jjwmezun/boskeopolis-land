#pragma once

#include <memory>
#include "option_box.hpp"
#include "option_selection.hpp"
#include <string>
#include <vector>

class OptionSystem
{
	public:
		static OptionSystem generateVerticalOptionSystem( const std::vector<std::u32string>& option_strings, int y = 32, int start_selection = 0 );
		static OptionSystem generateGridOptionSystem( const std::vector<std::u32string>& option_strings );

		void init();
		void update();
		void render() const;
		int selection() const;
		void changeOptions( const std::vector<std::u32string>& options );
		void setSelectedPressedDown();
		void setPressedDown( int option );

	private:
		std::vector<OptionBox> options_;
		std::unique_ptr<OptionSelection> selection_;

		OptionSystem( std::vector<OptionBox>&& options, std::unique_ptr<OptionSelection>&& selection );
		void updateOptions();
};
