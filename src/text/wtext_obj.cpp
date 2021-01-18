#include "localization.hpp"
#include "localization_language.hpp"
#include "render.hpp"
#include <unordered_map>
#include "wtext_obj.hpp"

WTextObj::WTextObj() {};

WTextObj::WTextObj
(
    const std::u32string& text,
    int x,
    int y,
    WTextCharacter::Color color,
    int width,
    Align align,
    WTextCharacter::Color shadow,
    int x_padding,
    int y_padding,
    VAlign valign,
    int height
)
:
    shadow_ ( shadow != WTextCharacter::Color::__NULL ),
    words_ ( text )
{
    // Apply x_padding to width ( pushes in from both sides, so x2 ).
    width -= ( x_padding * 2 );
    height -= ( y_padding * 2 );

    // Setup initial x
    const int start_y = y = y + y_padding;
    const int start_x = x = x + x_padding;
    const int line_end = start_x + width;

    // Create 1st line.
    lines_.emplace_back( x, y, std::vector<WTextCharacter>{}, std::vector<WTextCharacter>{} );

    const int color_offset = WTextCharacter::calculateColorOffset( color );
    const int shadow_offset = WTextCharacter::calculateColorOffset( shadow );

    std::vector<WTextCharacter> frames;
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

    const int frames_size = frames.size();
    int i = 0;
    while ( i < frames_size )
    {
        int ib = i;
        int xb = x;
        bool look_ahead = true;

        while ( look_ahead )
        {
            if ( ib >= frames_size )
            {
                look_ahead = false;
                break;
            }

            if ( frames[ ib ].isNewline() )
            {
                look_ahead = false;
            }
            else if ( frames[ ib ].isWhitespace() )
            {
                look_ahead = false;
            }
            else if ( xb >= line_end )
            {
                x = start_x;
                y += WTextCharacter::SIZE_PIXELS;
                lines_.emplace_back( x, y, std::vector<WTextCharacter>{}, std::vector<WTextCharacter>{} );
                look_ahead = false;
            }
            else if ( ib >= frames_size )
            {
                look_ahead = false;
                break;
            }
            ++ib;
            xb += WTextCharacter::SIZE_PIXELS;
        }

        while ( i < ib )
        {
            auto frame = frames[ i ];
            if ( frame.isNewline() || x > line_end )
            {
                x = start_x;
                y += WTextCharacter::SIZE_PIXELS;
                lines_.emplace_back( x, y, std::vector<WTextCharacter>{}, std::vector<WTextCharacter>{} );
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
            x += WTextCharacter::SIZE_PIXELS;
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
            line.x_ = start_x + ( ( width - ( line.frames_.size() * WTextCharacter::SIZE_PIXELS ) ) / 2 );
        }
    }
    else if ( align == Align::RIGHT )
    {
        for ( auto& line : lines_ )
        {
            line.x_ = line_end - ( line.frames_.size() * WTextCharacter::SIZE_PIXELS );
        }
    }

    if ( valign == VAlign::CENTER )
    {
        int center_y = start_y + ( ( height - ( lines_.size() * WTextCharacter::SIZE_PIXELS )) / 2 );
        for ( auto& line : lines_ )
        {
            line.y_ = center_y;
            center_y += WTextCharacter::SIZE_PIXELS;
        }
    }
    else if ( valign == VAlign::BOTTOM )
    {
        int bottom = start_y + height;
        int bottom_y = bottom - ( lines_.size() * WTextCharacter::SIZE_PIXELS );
        for ( auto& line : lines_ )
        {
            line.y_ = bottom_y;
            bottom_y += WTextCharacter::SIZE_PIXELS;
        }
    }
};

WTextObj::MessageData::MessageData( const std::u32string& _text, WTextCharacter::Color _color, WTextCharacter::Color _shadow )
:
    text ( _text ),
    color ( _color ),
    shadow ( _shadow )
{};

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

void WTextObj::render( int limit ) const
{
    if ( shadow_ )
    {
        for ( const WTextLine& line : lines_ )
        {
            line.renderWithShadow( limit );
            limit -= line.frames_.size();
            if ( limit <= 0 )
            {
                return;
            }
        }
    }
    else
    {
        for ( const WTextLine& line : lines_ )
        {
            line.renderWithoutShadow( limit );
            limit -= line.frames_.size();
            if ( limit <= 0 )
            {
                return;
            }
        }
    }
};

void WTextObj::changeColor( WTextCharacter::Color color )
{
    const int offset = WTextCharacter::calculateColorOffset( color );
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
    WTextCharacter::Color color,
    int width,
    Align align,
    WTextCharacter::Color shadow,
    int x_padding,
    int y_padding,
    VAlign valign,
    int height
)
{
    WTextObj o( text, x, y, color, width, align, shadow, x_padding, y_padding, valign, height );
    o.generateTexture( texture_box );
};

int WTextObj::getNumberOfCharacters() const
{
    int n = 0;
    for ( const auto& line : lines_ )
    {
        n += line.frames_.size();
    }
    return n;
};

int WTextObj::getNumberOfLines() const
{
    return ( int )( lines_.size() );
};

void WTextObj::setX( int x )
{
    for ( auto& line : lines_ )
    {
        line.x_ = x;
    }
};

void WTextObj::addToX( int value )
{
    for ( auto& line : lines_ )
    {
        line.x_ += value;
    }
};