#pragma once

#include "mezun_sdl2.hpp"
#include <string>
#include "wtext_obj.hpp"

class OptionBox
{
	public:
		static constexpr int VERTICAL_PADDING_MINIBLOCKS = 1;
		static constexpr int VERTICAL_PADDING_PIXELS = VERTICAL_PADDING_MINIBLOCKS * Text::CHAR_SIZE_PIXELS;
		static constexpr int BOX_HEIGHT = VERTICAL_PADDING_PIXELS * 2 + 8;

		OptionBox( const char32_t* text, int y, int width, int x = 0, WTextObj::Align align = WTextObj::Align::CENTER );
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

		WTextObj text_;
		const sdl2::SDLRect box_;
		const sdl2::SDLRect shadow_box_;
		int box_color_;
		OBState state_;
		int timer_;

		void renderShadow() const;
		void renderBox() const;
		WTextObj::Color getTextColor( int box_color ) const;
		int currentTextPosition( int value ) const;
};
