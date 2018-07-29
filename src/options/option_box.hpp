#pragma once

#include "mezun_sdl2.hpp"
#include <string>
#include "text.hpp"

class OptionBox
{
	public:
		static constexpr int VERTICAL_PADDING_MINIBLOCKS = 1;
		static constexpr int VERTICAL_PADDING_PIXELS = VERTICAL_PADDING_MINIBLOCKS * Text::CHAR_SIZE_PIXELS;
		static constexpr int BOX_HEIGHT = VERTICAL_PADDING_PIXELS * 2 + 8;

		OptionBox( std::string words, int y, int width, int x = -1 );
		~OptionBox();

		void update();
		void render() const;
		void setToNormal();
		void setToCurrent();
		void setToPrevious();
		void setToNullified();
		bool isNullified() const;


	private:
		enum class OBState
		{
			NORMAL,
			CURRENT,
			PREVIOUS,
			NULLIFIED
		};

		const sdl2::SDLRect box_;
		const sdl2::SDLRect shadow_box_;
		const std::string words_;
		const int text_y_;
		const int text_x_;
		const Text::FontAlign text_align_;
		int box_color_;
		OBState state_;
		int timer_;

		void renderShadow() const;
		void renderBox() const;
		void renderText() const;
		Text::FontColor getTextColor( int box_color ) const;
		int currentTextPosition( int value ) const;
};
