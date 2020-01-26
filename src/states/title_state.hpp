#pragma once

#include "block_system.hpp"
#include "camera.hpp"
#include "game_state.hpp"
#include "event_system.hpp"
#include "health.hpp"
#include "image_graphics.hpp"
#include "inventory_level.hpp"
#include "level.hpp"
#include "map_layer_image.hpp"
#include "sprite.hpp"
#include "option_system.hpp"
#include "sprite_graphics.hpp"
#include "text_obj.hpp"
#include "texture_box.hpp"
#include "player_graphics.hpp"
#include "sprite_system.hpp"

class TitleState : public GameState
{
	public:
		static constexpr int OPTIONS_SIZE = 4;

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

		bool can_load_;
        bool paused_;
        SDL_Texture* screen_texture_;
		TextureBox created_by_;
		Health health_;
		OptionSystem options_;
        sdl2::SDLRect screen_src_;
        sdl2::SDLRect screen_dest_;
		const ImageGraphics curtain_;
		const ImageGraphics logo_;
		MapLayerImage light_gradient_bg_;
		MapLayerImage skyline_bg_;
		MapLayerImage skyscrapers_bg_;
		MapLayerImage cloud_bg_;
		Level level_;
		Camera camera_;
		BlockSystem blocks_;
		SpriteSystem sprites_;
		EventSystem events_;
};
