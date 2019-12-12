#pragma once

class CharFrame
{
    public:
        enum class Type
        {
            NORMAL,
            WHITESPACE,
            NEWLINE
        };

        static constexpr int SIZE_PIXELS = 8;

        constexpr CharFrame( int x, int y, Type type )
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

        int changeColorOffset( int color_offset );

        constexpr bool isWhitespace() const
        {
            return type_ == Type::WHITESPACE;
        };

        constexpr bool isNewline() const
        {
            return type_ == Type::NEWLINE;
        };

    private:
        int x_;
        int y_;
        Type type_;
};