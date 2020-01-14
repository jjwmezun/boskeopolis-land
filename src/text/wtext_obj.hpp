#pragma once

#include "char_frame.hpp"
#include "texture_box.hpp"
#include "unit.hpp"
#include <vector>
#include "wtext_line.hpp"

class WTextObj
{
	public:
		enum class Color
		{
			BLACK = 0,
			DARK_GRAY = 1,
			DARK_MID_GRAY = 2,
			LIGHT_MID_GRAY = 3,
			LIGHT_GRAY = 4,
			WHITE = 5,
			__NULL = 6
		};
		static constexpr int NUMBER_OF_COLORS = ( int )( Color::__NULL );

		enum class Align
		{
			LEFT,
			CENTER,
			RIGHT
		};

		enum class VAlign
		{
			TOP,
			CENTER,
			BOTTOM
		};

		static constexpr Color DEFAULT_COLOR = Color::BLACK;
		static constexpr int DEFAULT_WIDTH = Unit::WINDOW_WIDTH_PIXELS;
		static constexpr Align DEFAULT_ALIGN = Align::LEFT;
		static constexpr Color DEFAULT_SHADOW = Color::__NULL;
		static constexpr int DEFAULT_PADDING = 0;
		static constexpr VAlign DEFAULT_VALIGN = VAlign::TOP;
		static constexpr int DEFAULT_HEIGHT = Unit::WINDOW_HEIGHT_PIXELS;

		struct MessageData
		{
			MessageData( const std::u32string& _text, Color _color = DEFAULT_COLOR, Color _shadow = DEFAULT_SHADOW );
			const std::u32string& text;
			Color color;
			Color shadow;
		};

		WTextObj();
		WTextObj
		(
			const std::u32string& text,
			int x,
			int y,
			Color color = DEFAULT_COLOR,
			int width = DEFAULT_WIDTH,
			Align align = DEFAULT_ALIGN,
			Color shadow = DEFAULT_SHADOW,
			int x_padding = DEFAULT_PADDING,
			int y_padding = DEFAULT_PADDING,
			VAlign valign = DEFAULT_VALIGN,
			int height = DEFAULT_HEIGHT
		);
		void render() const;
		void changeColor( Color color );
		void generateTexture( TextureBox& texture_box ) const;
		static void generateTexture
		(
    		TextureBox& texture_box,
			const std::u32string& text,
			int x,
			int y,
			Color color = DEFAULT_COLOR,
			int width = DEFAULT_WIDTH,
			Align align = DEFAULT_ALIGN,
			Color shadow = DEFAULT_SHADOW,
			int x_padding = DEFAULT_PADDING,
			int y_padding = DEFAULT_PADDING,
			VAlign valign = DEFAULT_VALIGN,
			int height = DEFAULT_HEIGHT
		);

	private:
		std::vector<WTextLine> lines_;
		bool shadow_;
};