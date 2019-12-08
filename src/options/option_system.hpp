#pragma once

#include "option_box.hpp"
#include "option_selection.hpp"
#include <string>
#include <vector>

class OptionSystem
{
	public:
		OptionSystem( const std::vector<std::u32string>& options, int y = 32, int start_selection = 0 );

		void init();
		void update();
		void render() const;
		int selection() const;
		void changeOptions( const std::vector<std::u32string>& options );

	private:
		std::vector<OptionBox> options_;
		OptionSelection selection_;

		void updateOptions();
};
