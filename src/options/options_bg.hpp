#pragma once

#include "map_layer_image.hpp"

class OptionsBG
{
	public:
		OptionsBG();
		void update();
		void render();

	private:
		MapLayerImage bg_;
};
