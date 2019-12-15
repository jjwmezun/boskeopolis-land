#include "localization.hpp"
#include "localization_language.hpp"
#include "render.hpp"
#include <unordered_map>
#include "wtext_obj.hpp"

static int calculateColorOffset( WTextObj::Color color )
{
    return ( color == WTextObj::Color::__NULL ) ? 0 : ( int )( color ) * Localization::getCurrentLanguage().getCharsetHeight() * CharFrame::SIZE_PIXELS;
};

WTextObj::WTextObj()
{

};

WTextObj::WTextObj
(
    const std::u32string& text,
    int x,
    int y,
    Color color,
    int width,
    Align align,
    Color shadow,
    int x_padding,
    int y_padding,
    VAlign valign,
    int height
)
:
    shadow_ ( shadow != Color::__NULL )
{
    // Apply x_padding to width ( pushes in from both sides, so x2 ).
    width -= ( x_padding * 2 );
    height -= ( y_padding * 2 );

    // Setup initial x
    const int start_y = y = y + y_padding;
    const int start_x = x = x + x_padding;
    const int line_end = start_x + width;

    // Create 1st line.
    lines_.push_back({ {}, {}, x, y });

    const int color_offset = calculateColorOffset( color );
    const int shadow_offset = calculateColorOffset( shadow );

    std::vector<CharFrame> frames;
    for ( const char32_t character : text )
    {
        if ( character == '\0' )
        {
            break;
        }

        auto char_frame_list = Localization::getCurrentLanguage().getCharacterFrames( character );
        for ( auto frame : char_frame_list )
        {
            frames.emplace_back( frame );
        }
    }

    int i = 0;
    while ( i < frames.size() )
    {
        int ib = i;
        int xb = x;
        bool look_ahead = true;

        while ( look_ahead )
        {
            if ( frames[ ib ].isNewline() )
            {
                look_ahead = false;
            }
            else if ( frames[ ib ].isWhitespace() )
            {
                look_ahead = false;
            }
            else if ( xb > line_end )
            {
                x = start_x;
                y += CharFrame::SIZE_PIXELS;
                lines_.push_back({ {}, {}, x, y });
                look_ahead = false;
            }
            else if ( ib >= frames.size() )
            {
                look_ahead = false;
                break;
            }

            ++ib;
            xb += CharFrame::SIZE_PIXELS;
        }

        while ( i < ib )
        {
            auto frame = frames[ i ];
            if ( frame.isNewline() || x > line_end )
            {
                x = start_x;
                y += CharFrame::SIZE_PIXELS;
                lines_.push_back({ {}, {}, x, y });
            }
            else
            {
                frame.setColorOffset( color_offset );
                lines_[ lines_.size() - 1 ].frames_.emplace_back( frame );
                if ( shadow_ )
                {
                    auto shadow_frame = frames[ i ];
                    shadow_frame.setColorOffset( shadow_offset );
                    lines_[ lines_.size() - 1 ].shadow_frames_.emplace_back( shadow_frame );
                }
            }
            ++i;
            x += CharFrame::SIZE_PIXELS;
        }
    }

    for ( auto& line : lines_ )
    {
        if ( !empty( line.frames_ ) && line.frames_[ line.frames_.size() - 1 ].isWhitespace() )
        {
            line.frames_.pop_back();
        }
        if ( !empty( line.shadow_frames_ ) && line.shadow_frames_[ line.shadow_frames_.size() - 1 ].isWhitespace() )
        {
            line.shadow_frames_.pop_back();
        }
    }

    if ( align == Align::CENTER )
    {
        for ( auto& line : lines_ )
        {
            line.x_ = start_x + ( ( width - ( line.frames_.size() * CharFrame::SIZE_PIXELS ) ) / 2 );
        }
    }
    else if ( align == Align::RIGHT )
    {
        for ( auto& line : lines_ )
        {
            line.x_ = line_end - ( line.frames_.size() * CharFrame::SIZE_PIXELS );
        }
    }

    if ( valign == VAlign::CENTER )
    {
        int center_y = start_y + ( ( height - ( lines_.size() * CharFrame::SIZE_PIXELS )) / 2 );
        for ( auto& line : lines_ )
        {
            line.y_ = center_y;
            center_y += CharFrame::SIZE_PIXELS;
        }
    }
    else if ( valign == VAlign::BOTTOM )
    {
        int bottom = start_y + height;
        int bottom_y = bottom - ( lines_.size() * CharFrame::SIZE_PIXELS );
        for ( auto& line : lines_ )
        {
            line.y_ = bottom_y;
            bottom_y += CharFrame::SIZE_PIXELS;
        }
    }
};

void WTextObj::render() const
{
    if ( shadow_ )
    {
        for ( const WTextLine& line : lines_ )
        {
            line.renderWithShadow();
        }
    }
    else
    {
        for ( const WTextLine& line : lines_ )
        {
            line.renderWithoutShadow();
        }
    }
};

void WTextObj::changeColor( Color color )
{
    int offset = calculateColorOffset( color );
    for ( auto& line : lines_ )
    {
        line.changeColorOffset( offset );
    }
};

void WTextObj::generateTexture( TextureBox& texture_box ) const
{
    texture_box.init();
    texture_box.startDrawing();
    Render::clearScreenTransparency();
    render();
    texture_box.endDrawing();
};

void WTextObj::generateTexture
(
    TextureBox& texture_box,
    const std::u32string& text,
    int x,
    int y,
    Color color,
    int width,
    Align align,
    Color shadow,
    int x_padding,
    int y_padding,
    VAlign valign,
    int height
)
{
    WTextObj o( text, x, y, color, width, align, shadow, x_padding, y_padding, valign, height );
    o.generateTexture( texture_box );
};