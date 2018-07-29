#pragma once

#include "option_box.hpp"
#include "option_selection.hpp"
#include <string>
#include <vector>

class OptionSystem
{
	public:
		OptionSystem( std::vector<std::string> options, int option_width, int y = 32 );
		~OptionSystem();

		void update();
		void render() const;
		int selection() const;


	private:
		std::vector<OptionBox> options_;
		OptionSelection selection_;

		void updateOptions();
};
