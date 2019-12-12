#pragma once

#include "mezun_sdl2.hpp"
#include <string>
#include "texture_box.hpp"
#include "wtext_obj.hpp"

class OptionBox
{
	public:
		static constexpr int VERTICAL_PADDING_MINIBLOCKS = 1;
		static constexpr int VERTICAL_PADDING_PIXELS = VERTICAL_PADDING_MINIBLOCKS * CharFrame::SIZE_PIXELS;
		static constexpr int BOX_HEIGHT = VERTICAL_PADDING_PIXELS * 2 + CharFrame::SIZE_PIXELS;

		OptionBox( const std::u32string& words, int y, int width, int x = -1 );
		~OptionBox();
		OptionBox( OptionBox&& o );
		OptionBox( const OptionBox& ) = delete;
		OptionBox& operator=( const OptionBox& ) = delete;
		OptionBox& operator=( OptionBox&& ) = delete;

		void init();
		void update();
		void render() const;
		void setToNormal();
		void setToCurrent();
		void setToPrevious();
		void setToNullified();
		bool isNullified() const;
		void changeText( const std::u32string& words );

		static int calculateMaxBoxWidthWithPadding( const std::vector<std::u32string>& options );

	private:
		enum class OBState
		{
			NORMAL,
			CURRENT,
			PREVIOUS,
			NULLIFIED
		};

		TextureBox text_[ WTextObj::NUMBER_OF_COLORS ];
		std::u32string words_;
		sdl2::SDLRect box_;
		sdl2::SDLRect shadow_box_;
		int width_;
		int x_;
		int y_;
		int box_color_;
		OBState state_;
		int timer_;

		void renderShadow() const;
		void renderBox() const;
		int currentTextPosition( int value ) const;
};
