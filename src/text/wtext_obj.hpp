#pragma once

#include "wtext_character.hpp"
#include "texture_box.hpp"
#include "unit.hpp"
#include <vector>
#include "wtext_line.hpp"

class WTextObj
{
	public:
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

		static constexpr WTextCharacter::Color DEFAULT_COLOR = WTextCharacter::Color::BLACK;
		static constexpr int DEFAULT_WIDTH = Unit::WINDOW_WIDTH_PIXELS;
		static constexpr Align DEFAULT_ALIGN = Align::LEFT;
		static constexpr WTextCharacter::Color DEFAULT_SHADOW = WTextCharacter::Color::__NULL;
		static constexpr int DEFAULT_PADDING = 0;
		static constexpr VAlign DEFAULT_VALIGN = VAlign::TOP;
		static constexpr int DEFAULT_HEIGHT = Unit::WINDOW_HEIGHT_PIXELS;

		struct MessageData
		{
			MessageData( const std::u32string& _text, WTextCharacter::Color _color = DEFAULT_COLOR, WTextCharacter::Color _shadow = DEFAULT_SHADOW );
			const std::u32string& text;
			WTextCharacter::Color color;
			WTextCharacter::Color shadow;
		};

		WTextObj();
		WTextObj
		(
			const std::u32string& text,
			int x,
			int y,
			WTextCharacter::Color color = DEFAULT_COLOR,
			int width = DEFAULT_WIDTH,
			Align align = DEFAULT_ALIGN,
			WTextCharacter::Color shadow = DEFAULT_SHADOW,
			int x_padding = DEFAULT_PADDING,
			int y_padding = DEFAULT_PADDING,
			VAlign valign = DEFAULT_VALIGN,
			int height = DEFAULT_HEIGHT
		);
		void render() const;
		void changeColor( WTextCharacter::Color color );
		void generateTexture( TextureBox& texture_box ) const;
		static void generateTexture
		(
    		TextureBox& texture_box,
			const std::u32string& text,
			int x,
			int y,
			WTextCharacter::Color color = DEFAULT_COLOR,
			int width = DEFAULT_WIDTH,
			Align align = DEFAULT_ALIGN,
			WTextCharacter::Color shadow = DEFAULT_SHADOW,
			int x_padding = DEFAULT_PADDING,
			int y_padding = DEFAULT_PADDING,
			VAlign valign = DEFAULT_VALIGN,
			int height = DEFAULT_HEIGHT
		);

		std::vector<WTextLine> lines_;
		bool shadow_;
};