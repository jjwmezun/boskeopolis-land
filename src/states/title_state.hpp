#pragma once

#include "game_state.hpp"
#include "map_layer_image.hpp"
#include "sprite_graphics.hpp"
#include "text.hpp"
#include <array>

class TitleState : public GameState
{
	public:
		static constexpr int OPTIONS_SIZE = 4;

		TitleState();
		~TitleState();

		void stateUpdate() override;
		void stateRender() override;
		void init() override;

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
		const std::array<const sdl2::SDLRect, ( std::size_t )( OPTIONS_SIZE )> option_bg_;
		const std::array<const sdl2::SDLRect, ( std::size_t )( OPTIONS_SIZE )> option_bg_shadows_;
		const std::array<const std::string, ( std::size_t )( OPTIONS_SIZE )> option_text_;
		SpriteGraphics logo_gfx_;
		Text created_by_;
		const sdl2::SDLRect logo_rect_;
		int selection_;
		int prev_selection_;
		int selection_timer_;
		bool can_load_;
		
		void addToSelection();
		void subtractFromSelection();
		inline bool invalidOption( int i )
		{
			return i == ( int )( Option::OPTIONS ) || ( !can_load_ && i == ( int )( Option::LOAD ) );
		};
};