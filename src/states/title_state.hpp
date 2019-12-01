#pragma once

#include "game_state.hpp"
#include "map_layer_image.hpp"
#include "option_system.hpp"
#include "sprite_graphics.hpp"
#include "text_obj.hpp"
#include "texture_box.hpp"

class TitleState : public GameState
{
	public:
		static constexpr int OPTIONS_SIZE = 4;

		TitleState();
		~TitleState();

		void stateUpdate() override;
		void stateRender() override;
		void init() override;
		void backFromPop() override;


	private:
		enum class Option
		{
			NEW,
			LOAD,
			OPTIONS,
			QUIT
		};

		MapLayerImage light_gradient_bg_;
		MapLayerImage skyline_bg_;
		MapLayerImage skyscrapers_bg_;
		MapLayerImage cloud_bg_;
		SpriteGraphics logo_gfx_;
		OptionSystem options_;
		const sdl2::SDLRect logo_rect_;
		TextureBox created_by_;
		bool can_load_;
};
