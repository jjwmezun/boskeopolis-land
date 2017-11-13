#pragma once

#include "game_state.hpp"
#include "map_layer_image.hpp"
#include "sprite_graphics.hpp"
#include "text.hpp"
#include <array>

class TitleState : public GameState
{
	public:
		TitleState();
		~TitleState();

		void update() override;
		void stateRender() override;
		void init() override;

	private:
		enum class Option
		{
			NEW,
			LOAD,
			OPTIONS,
			QUIT,
			__END
		};
		static constexpr int OPTIONS_SIZE = ( int )( Option::__END );

		MapLayerImage bg_;
		const std::array<const std::string, ( std::size_t )( OPTIONS_SIZE )> option_text_;
		SpriteGraphics logo_gfx_;
		Text created_by_;
		Counter selection_;
		const sdl2::SDLRect logo_rect_;
		sdl2::SDLRect highlight_rect_;
		bool can_load_;
};