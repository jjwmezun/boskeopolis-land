#pragma once

#include "game_state.hpp"
#include "image_graphics.hpp"
#include "level.hpp"
#include "level_state.hpp"
#include "map_layer_image.hpp"
#include "sprite.hpp"
#include "option_system.hpp"
#include "sprite_graphics.hpp"
#include "text_obj.hpp"
#include "texture_box.hpp"
#include "player_graphics.hpp"

class TitleState final : public GameState
{
	public:
		static constexpr int OPTIONS_SIZE = 3;

		TitleState();
		~TitleState();
        TitleState( const TitleState& ) = delete;
        TitleState& operator=( const TitleState& ) = delete;
        TitleState( TitleState&& ) = delete;
        TitleState& operator=( TitleState&& ) = delete;
		void stateUpdate() override;
		void stateRender() override;
		void init() override;

	private:
		void renderLevel();
		void generateLevelTexture();
		void renderHeader();

        bool paused_;
        SDL_Texture* screen_texture_;
		TextureBox created_by_;
		OptionSystem options_;
        sdl2::SDLRect screen_src_;
        sdl2::SDLRect screen_dest_;
		const ImageGraphics curtain_;
		const ImageGraphics logo_;
		MapLayerImage light_gradient_bg_;
		MapLayerImage skyline_bg_;
		MapLayerImage skyscrapers_bg_;
		MapLayerImage cloud_bg_;
		LevelState level_;
};
