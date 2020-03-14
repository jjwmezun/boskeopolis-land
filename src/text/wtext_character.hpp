#pragma once

class WTextCharacter final
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

        enum class Type
        {
            NORMAL,
            WHITESPACE,
            NEWLINE
        };

        static constexpr int SIZE_PIXELS = 8;

        constexpr WTextCharacter( int x, int y, Type type )
        :
            x_ ( x * SIZE_PIXELS ),
            y_ ( y * SIZE_PIXELS ),
            type_ ( type )
        {};

        constexpr int x() const { return x_; };
        constexpr int y() const { return y_; };

        constexpr void setColorOffset( int color_offset )
        {
            y_ += color_offset;
        };

        void changeColorOffset( int color_offset );
        void changeColor( WTextCharacter::Color color );

        constexpr bool isWhitespace() const
        {
            return type_ == Type::WHITESPACE;
        };

        constexpr bool isNewline() const
        {
            return type_ == Type::NEWLINE;
        };

        static int calculateColorOffset( Color color );

    private:
        int x_;
        int y_;
        Type type_;
};