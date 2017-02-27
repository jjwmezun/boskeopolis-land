



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
    #include "text_component_marquee.h"
    #include "timers/timer_repeat.h"
    #include "unit.h"


// CLASS
//===================================

    class Text
    {
        public:
			friend class TextComponentMarquee;
		
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
				bool center_y = false,
                unsigned int line_limit = DEFAULT_LINE_LENGTH,
                std::unique_ptr<TextComponent> component = nullptr
            );
            ~Text() noexcept;
            Text( Text&& m ) noexcept;
            Text& operator=( Text&& m ) = delete;
            Text( const Text& ) = delete;
            Text& operator= ( const Text& ) = delete;

            void update();
			
            void render
			(
				Graphics& graphics,
				Camera* camera = nullptr,
				FontShade shade = FontShade::__NULL
			) const;
			
            int right() const;
			int width() const;
            int x() const;

            static void renderNumber
			(
				Graphics& graphics,
				int n,
				int x,
				int y,
				int digits = -1,
				FontShade shade = FontShade::BLACK,
				Camera* camera = nullptr
			);
			
            static void renderText
			(
				Graphics& graphics,
				const std::string& text,
				int x,
				int y,
				Camera* camera = nullptr,
				FontShade shade = FontShade::BLACK,
				unsigned int line_limit = DEFAULT_LINE_LENGTH,
				FontAlign align = FontAlign::LEFT,
				bool center_y = false
			);
			
            static std::string stringifyNum( int n );
            static std::string formatNumCommas( int n );
            static std::string formatNumDigitPadding( int n, int digits );
            static std::string formatNumCommas( std::string text );
            static std::string timeToString( int seconds, int minutes, int minutes_padding = 1 );
            static int getDigit( int n, int d, int remain = 10 );
            static int numODigits( int n );
            static int centerX( int line_length );
            static int centerY( int text_length, int line_limit );


		protected:
            std::string words_;
            int x_ = 0;
            int y_ = 0;


        private:
            static constexpr int CHAR_SIZE_PIXELS           = 8;
            static constexpr int NUM_OF_FONT_SHADES         = 6;
            static constexpr int CHARSET_WIDTH_MINI_BLOCKS  = 32;
            static constexpr int CHARSET_HEIGHT_MINI_BLOCKS = 32;
			static constexpr int DEFAULT_LINE_LENGTH        = Unit::WINDOW_WIDTH_MINIBLOCKS;
            static std::map<char, int> char_conversion_;

            FontShade shade_;
            FontAlign align_;
            std::unique_ptr<TextComponent> component_;
            int line_limit_;
			bool center_y_;

            static int frameX( int n );
            static int frameY( int n, FontShade shade );
            static int shadeOffset( FontShade shade );
			static int testLineLength( const std::string& text, int line_length, int letters_so_far );

            int shadeNum() const;
    };

#endif // TEXT_H

