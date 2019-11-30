#pragma once

#include "char_frame.hpp"
#include <vector>

class WTextObj
{
	public:
		WTextObj();
		WTextObj( const char32_t* text, int x, int y );
		void render();
		void renderChar( const CharFrame character, int x, int y );

	private:
		std::vector<CharFrame> frames_;
		int x_;
		int y_;
};