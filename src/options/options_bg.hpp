#pragma once

#include "map_layer_image.hpp"

class OptionsBG final
{
	public:
		OptionsBG();
		void update();
		void render();

	private:
		MapLayerImage bg_;
};
