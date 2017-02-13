




// Name
//===================================
//
// Text
//


// DEPENDENCIES
//===================================

    #include "camera.h"
    #include "graphics.h"
    #include <sstream>
    #include "text.h"


// STATIC PROPERTIES
//===================================

    std::map<char, int> Text::char_conversion_ =
    {
        { '0', 0 },
        { '1', 1 },
        { '2', 2 },
        { '3', 3 },
        { '4', 4 },
        { '5', 5 },
        { '6', 6 },
        { '7', 7 },
        { '8', 8 },
        { '9', 9 },
        { 'a', 10 },
        { 'A', 10 },
        { 'b', 11 },
        { 'B', 11 },
        { 'c', 12 },
        { 'C', 12 },
        { 'd', 13 },
        { 'D', 13 },
        { 'e', 14 },
        { 'E', 14 },
        { 'f', 15 },
        { 'F', 15 },
        { 'g', 16 },
        { 'G', 16 },
        { 'h', 17 },
        { 'H', 17 },
        { 'i', 18 },
        { 'I', 18 },
        { 'j', 19 },
        { 'J', 19 },
        { 'k', 20 },
        { 'K', 20 },
        { 'l', 21 },
        { 'L', 21 },
        { 'm', 22 },
        { 'M', 22 },
        { 'n', 23 },
        { 'N', 23 },
        { 'o', 24 },
        { 'O', 24 },
        { 'p', 25 },
        { 'P', 25 },
        { 'q', 26 },
        { 'Q', 26 },
        { 'r', 27 },
        { 'R', 27 },
        { 's', 28 },
        { 'S', 28 },
        { 't', 29 },
        { 'T', 29 },
        { 'u', 30 },
        { 'U', 30 },
        { 'v', 31 },
        { 'V', 31 },
        { 'w', 32 },
        { 'W', 32 },
        { 'x', 33 },
        { 'X', 33 },
        { 'y', 34 },
        { 'Y', 34 },
        { 'z', 35 },
        { 'Z', 35 },
        { '?', 36 },
        { '!', 37 },
        { '¡', 38 },
        { '¿', 39 },
        { '@', 40 },
        { '#', 41 },
        { '$', 42 },
        { '%', 43 },
        { '^', 44 },
        { '&', 45 },
        { '*', 46 },
        { '(', 47 },
        { ')', 48 },
        { '[', 49 },
        { ']', 50 },
        { ',', 51 },
        { '.', 52 },
        { '/', 53 },
        { '\'', 54 },
        { '’', 54 },
        { ':', 55 },
        { ';', 56 },
        { '{', 57 },
        { '}', 58 },
        { '-', 59 },
        { '_', 60 },
        { '=', 61 },
        { '"', 62 },
        { '/', 63 },
        { '\\', 64 },
        { '~', 65 },
        { '|', 66 },
        { '+', 67 },
        { 'í', 74 },
        { '\t', 68 },
        { ' ', 127 }
    };


// METHODS
//===================================

    Text::Text
    (
        std::string words,
        int x,
        int y,
        FontShade shade,
        FontAlign align,
        bool blink,
        std::unique_ptr<TextComponent> component,
        unsigned int line_limit
    )
    :
        words_ ( words ),
        x_ ( x ),
        y_ ( y ),
        shade_ ( shade ),
        align_ ( align ),
        blink_ ( blink ),
        component_ ( std::move( component ) ),
        line_limit_ ( line_limit )
    {
    };

    Text::~Text() noexcept {};

    Text::Text( Text&& m ) noexcept
    :
        words_ ( m.words_ ),
        x_ ( m.x_ ),
        y_ ( m.y_ ),
        shade_ ( m.shade_ ),
        align_ ( m.align_ ),
        blink_ ( m.blink_ ),
        component_ ( std::move( m.component_ ) ),
        line_limit_ ( m.line_limit_ )
    {};

    /*Text& Text::operator=( Text&& m ) noexcept
    :
        Text( m )
    {
        //return ( m.words_, m.x_, m.y_, m.shade_, m.align_, m.blink_, std::move( m.component_ ), m.line_limit_ );
    };*/

    void Text::renderText( Graphics& graphics, const std::string& text, int x, int y, Camera* camera, FontShade shade, int line_limit, FontAlign align )
    {

        if ( align == FontAlign::CENTER )
        {
            x = centerX( text );
        }

        //int char_y = y;
        int line = 0;
        int chars_on_this_line = 0;

        for ( int i = 0; i < text.length(); ++i )
        {


            // For some reason, extended ASCII that are numerically below 0 are 2 chars, the 1st coming out as a tacked-on 0 beforehand..
            // Skipping to the 2nd half o' the ASCII fixes this.
            if ( text[ i ] < 0 )
                ++i;

            const int frame = char_conversion_[ text.at( i ) ];
            //int char_x = x + ( CHAR_SIZE_PIXELS * i );

            //char_x = ( ( i % line_limit ) * CHAR_SIZE_PIXELS ) + x;
            //char_y = ( floor( i / line_limit ) * CHAR_SIZE_PIXELS ) + y;

            if ( chars_on_this_line == line_limit || text.at( i ) == '\n' )
            {
                ++line;
                chars_on_this_line = 0;
            }

            if ( text.at( i ) == '\n' )
            {
                continue;
            }

            const int char_x = ( CHAR_SIZE_PIXELS * chars_on_this_line ) + x;
            const int char_y = ( CHAR_SIZE_PIXELS * line ) + y;

            ++chars_on_this_line;

            sdl2::SDLRect dest =
            {
                char_x,
                char_y,
                CHAR_SIZE_PIXELS,
                CHAR_SIZE_PIXELS
            };

            if ( camera != nullptr )
            {
                dest.x = camera->relativeX( dest );
                dest.y = camera->relativeY( dest );
            }

            graphics.renderObject
            (
                Graphics::SpriteSheet::CHARSET,
                {
                frameX( frame ),
                frameY( frame, shade ),
                CHAR_SIZE_PIXELS,
                CHAR_SIZE_PIXELS
                },
                dest
            );
        }
    };

    void Text::render( Graphics& graphics, Camera* camera, FontShade shade ) const
    {
        if ( shade == FontShade::__NULL )
            shade = shade_;

        renderText( graphics, words_, x_, y_, camera, shade, line_limit_, align_ );
    };

    int Text::centerX( const std::string& words )
    {
        return Unit::MiniBlocksToPixels( floor ( Unit::WINDOW_WIDTH_MINIBLOCKS / 2 ) - floor( words.length() / 2 ) );
    };

    int Text::shadeOffset( FontShade shade )
    {
        if ( shade == FontShade::__NULL )
        {
            return 0;
        }
        else
        {
            return (int)shade * 32;
        }
    };

    int Text::shadeNum() const
    {
        if ( shade_ == FontShade::__NULL )
        {
            return 0;
        }
        else
        {
            return (int)shade_;
        }
    };

    int Text::frameX( int n )
    {
        return Unit::MiniBlocksToPixels( n % CHARSET_WIDTH_MINI_BLOCKS );
    };

    int Text::frameY( int n, FontShade shade )
    {
        return shadeOffset( shade ) + Unit::MiniBlocksToPixels( floor( n / CHARSET_WIDTH_MINI_BLOCKS ) );
    };

    void Text::renderNumber( Graphics& graphics, int n, int x, int y, int digits, FontShade shade, Camera* camera )
    {
        renderText( graphics, formatNumDigitPadding( n, digits ), x, y, camera, shade );
    };

    int Text::getDigit( int n, int d, int remain )
    {
        return int( floor( n / pow( 10, d-1 ) ) ) % remain;
    };

    int Text::numODigits( int n )
    {
        if ( n == 0 )
        {
            return 1;
        }
        else
        {
            for ( int i = 0; i < 99; ++i )
            {
                if ( abs( n ) < pow( 10, i ) )
                {
                    return i;
                }
            }
        }

        return -1;
    };

    void Text::update()
    {
        flipped_ = false;

        if ( component_ )
        {
            component_->update( *this );
        }

        if ( blink_ )
        {
            if ( blink_timer_.hit() )
            {
                if ( blink_dir_ == Direction::Vertical::DOWN )
                {
                    --blink_counter_;

                    if ( blink_counter_.value() == 0 )
                        blink_dir_ = Direction::Vertical::UP;
                }
                else
                {
                    ++blink_counter_;

                    if ( blink_counter_.value() == NUM_OF_FONT_SHADES-1 )
                        blink_dir_ = Direction::Vertical::DOWN;
                }
            }

            blink_timer_.update();

            if ( blink_counter_.value() >= 0 && blink_counter_.value() < NUM_OF_FONT_SHADES )
                shade_ = (FontShade)blink_counter_.value();
        }
    };

    std::string Text::formatNumCommas( int n )
    {
        return formatNumCommas( stringifyNum( n ) );
    };
/*
    std::string Text::formatNumDigitPadding( int n, int digits )
    {
        return formatNumDigitPadding( stringifyNum( n ), digits );
    };*/

    std::string Text::formatNumCommas( std::string text )
    {
        int difference = text.size() - 3;

        while ( difference > 0 )
        {
            text.insert( difference, "," );
            difference -= 3;
        }

        return text;
    };

    std::string Text::formatNumDigitPadding( int n, int digits )
    {
        std::string text = stringifyNum( std::abs( n ) );

        if ( digits > 0 )
        {
            int difference = digits - text.size();

            if ( n < 0 )
                --difference;

            if ( difference > 0 )
            {
                for ( int i = 0; i < difference; ++i )
                {
                    text.insert( 0, "0" );
                }
            }
        }

        if ( n < 0 )
            text.insert( 0, "-" );

        return text;
    };

    std::string Text::stringifyNum( int n )
    {
        std::ostringstream oss;
        oss << n;
        std::string text = oss.str();
        return text;
    };

    void Text::moveLeft( int n )
    {
        x_ -= n;

        if ( right() < 0 )
        {
            x_ = Unit::WINDOW_WIDTH_PIXELS;
            flipped_ = true;
        }
    };

    void Text::moveRight( int n )
    {
        x_ += n;

        if ( x_ > Unit::WINDOW_WIDTH_PIXELS )
        {
            flipped_ = true;
            x_ = -( words_.size() * CHAR_SIZE_PIXELS );
        }
    };

    int Text::right() const
    {
        return x_ + ( words_.size() * CHAR_SIZE_PIXELS );
    };

    int Text::x() const
    {
        return x_;
    };

    bool Text::flipped() const
    {
        return flipped_;
    };

    /*std::string Text::timeToString( std::string seconds, std::string minutes, int minutes_padding )
    {
        return formatNumDigitPadding( minutes, minutes_padding ) + ":" + formatNumDigitPadding( seconds, 2 );
    };*/

    std::string Text::timeToString( int seconds, int minutes, int minutes_padding )
    {
        return formatNumDigitPadding( minutes, minutes_padding ) + ":" + formatNumDigitPadding( seconds, 2 );
        //return timeToString( stringifyNum( seconds ), stringifyNum( minutes ), minutes_padding );
    };
