



// Name
//===================================
//
// Text
//

#ifndef TEXT_H
#define TEXT_H


// FORWARD DECLARATIONS
//===================================

    class Camera;
    class Graphics;
    class TextComponent;


// DEPENDENCIES
//===================================

    #include "counter.h"
    #include <map>
    #include <memory>
    #include <SDL2/SDL.h>
    #include <string>
    #include "text_component.h"
    #include "timers/timer_repeat.h"
    #include "unit.h"


// CLASS
//===================================

    class Text
    {
        public:
            enum class FontShade
            {
                BLACK = 0,
                DARK_GRAY = 1,
                DARK_MID_GRAY = 2,
                LIGHT_MID_GRAY = 3,
                LIGHT_GRAY = 4,
                WHITE = 5,
                __NULL = 6
            };

            enum class FontAlign
            {
                LEFT,
                CENTER,
                RIGHT
            };

            Text
            (
                std::string words,
                int x = 0,
                int y = 0,
                FontShade shade = FontShade::BLACK,
                FontAlign align = FontAlign::LEFT,
                bool blink = false,
                std::unique_ptr<TextComponent> component = nullptr,
                unsigned int line_limit = Unit::WINDOW_WIDTH_MINIBLOCKS
            );
            ~Text() noexcept;
            Text( Text&& m ) noexcept;
            Text& operator=( Text&& m ) = delete;
            Text( const Text& ) = delete;
            Text& operator= ( const Text& ) = delete;

            std::string words_;
            int x_ = 0;
            int y_ = 0;

            void update();
            void render( Graphics& graphics, Camera* camera = nullptr, FontShade shade = FontShade::__NULL ) const;
            void moveLeft( int n = 1 );
            void moveRight( int n = 1 );
            int right() const;
            int x() const;
            bool flipped() const;

            static void renderNumber( Graphics& graphics, int n, int x, int y, int digits = -1, FontShade shade = FontShade::BLACK, Camera* camera = nullptr );
            static void renderText( Graphics& graphics, const std::string& text, int x, int y, Camera* camera = nullptr, FontShade shade = FontShade::BLACK, int line_limit = Unit::WINDOW_WIDTH_MINIBLOCKS, FontAlign align = FontAlign::LEFT );
            static std::string stringifyNum( int n );
            static std::string formatNumCommas( int n );
            static std::string formatNumDigitPadding( int n, int digits );
            static std::string formatNumCommas( std::string text );
            static std::string timeToString( int seconds, int minutes, int minutes_padding = 1 );
            static int getDigit( int n, int d, int remain = 10 );
            static int numODigits( int n );
            static int centerX( const std::string& words );

        private:
            static const int CHAR_SIZE_PIXELS = 8;
            static const int NUM_OF_FONT_SHADES = 6;
            static const int CHARSET_WIDTH_MINI_BLOCKS = 32;
            static std::map<char, int> char_conversion_;

            FontShade shade_;
            FontAlign align_;
            bool blink_;
            Counter blink_counter_ = Counter( 0, NUM_OF_FONT_SHADES-1, 0, true );
            TimerRepeat blink_timer_;
            Direction::Vertical blink_dir_ = Direction::Vertical::UP;
            std::unique_ptr<TextComponent> component_;
            bool flipped_ = false;
            int line_limit_;

            static int frameX( int n );
            static int frameY( int n, FontShade shade );
            static int shadeOffset( FontShade shade );

            int shadeNum() const;
    };

#endif // TEXT_H

