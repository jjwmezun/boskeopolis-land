#pragma once

#include <SDL2/SDL.h>

class NeonOverlay final
{
	public:
		NeonOverlay();
		~NeonOverlay();
		void render();
		void update();

	private:
		SDL_Color color_;
		int hue_;

		void convertHSVToRGB();
};
