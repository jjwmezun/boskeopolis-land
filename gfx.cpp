#include <cglm/cglm.h>
#include <cglm/call.h>
#include "config.hpp"
#include <cstdio>
#include "gfx.hpp"
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include "json.hpp"
#include "layer.hpp"
#include "log.hpp"
#include <string>
#include "text.hpp"
#include <unordered_map>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

namespace BSL::GFX
{
    #define BASE_MATRIX {\
        { 1.0f, 0.0f, 0.0f, 0.0f },\
        { 0.0f, 1.0f, 0.0f, 0.0f },\
        { 0.0f, 0.0f, 1.0f, 0.0f },\
        { 0.0f, 0.0f, 0.0f, 1.0f }\
    }

    // Will evenly divide #s 1-16.
    static constexpr unsigned int ANIMATION_MAX = 2 * 3 * 4 * 5 * 7 * 9 * 11 * 12 * 13;

    static constexpr unsigned int VERTEX_SIZE = 4;
    static constexpr unsigned int GL_TEXTURE_COUNT = 3;
    static constexpr unsigned int MAX_TEXTURES = 1000;

    enum class GraphicCharType
    {
        NORMAL,
        WHITESPACE,
        NEWLINE
    };

    struct CharPixel
    {
        uint_fast8_t x : 4;
        uint_fast8_t y : 4;
    };

    struct GraphicCharTemplate
    {
        GraphicCharType type;
        uint_fast8_t x;
        uint_fast8_t y;
        uint_fast8_t w;
        uint_fast8_t h;
        uint_fast8_t pixel_count;
        CharPixel * pixels;
        uint_fast8_t shadow_count;
        CharPixel * shadow;
    };

    struct GraphicChar
    {
        int x;
        int y;
        uint_fast8_t w;
        uint_fast8_t h;
        uint_fast8_t srcx;
        uint_fast8_t srcy;
        uint_fast8_t pixel_count;
        CharPixel * pixels;
        uint_fast8_t shadow_count;
        CharPixel * shadow;
    };

    enum class ShaderType
    {
        VERTEX,
        FRAGMENT
    };

    typedef struct Shader
    {
        ShaderType type;
        const char * code;
    } Shader;

    enum class GraphicType
    {
        RECT,
        SPRITE,
        TILEMAP,
        TEXT,
        SPRITE_RAW,
        SPRITE_RAW_NO_TRANSPARENCY
    };

    struct Texture
    {
        unsigned int w;
        unsigned int h;
        unsigned char * data;
    };

    struct RawGraphic
    {
        bool abs;
        BSL::Layer layer;
        float opacity;
        GraphicType type;
        union
        {
            struct
            {
                struct
                {
                    int x;
                    int y;
                    unsigned int w;
                    unsigned int h;
                }
                coords;
                unsigned char color1;
                unsigned char color2;
            }
            rect;
            struct
            {
                unsigned int texture;
                unsigned int srcx;
                unsigned int srcy;
                unsigned int w;
                unsigned int h;
                int x;
                int y;
                bool flipx;
                bool flipy;
            }
            sprite;
            struct
            {
                unsigned int texture;
                Tile * tiles;
                unsigned int w;
                unsigned int h;
                int x;
                int y;
            }
            tilemap;
            struct
            {
                unsigned int char_count;
                GraphicChar * chars;
                bool shadow;
                uint_fast8_t color1;
                uint_fast8_t color2;
            }
            text;
            struct
            {
                unsigned char * data;
                unsigned int dataw;
                unsigned int datah;
                unsigned int srcx;
                unsigned int srcy;
                unsigned int w;
                unsigned int h;
                int x;
                int y;
            }
            rawsprite;
        }
        data;
    };

    static int magnification = 1;
    static GLFWwindow * window;
    static unsigned int VAO;
    static unsigned int main_shader;
    static unsigned int border_shader;
    static struct
    {
        GLint texture;
        GLint palette_texture;
        GLfloat palette_no;
        GLfloat canvas_opacity;
    } uniforms;
    static unsigned int max_graphics = 100;
    static unsigned int gl_textures[ GL_TEXTURE_COUNT ];
    static RawGraphic * graphics;
    static unsigned int graphics_count = 0;
    static unsigned char gl_texture_data[ BSL::WINDOW_WIDTH_PIXELS * BSL::WINDOW_HEIGHT_PIXELS ];
    static Texture textures[ MAX_TEXTURES ];
    static unsigned int texture_count = 0;
    static struct
    {
        unsigned int x;
        unsigned int y;
    } camera;
    static std::unordered_map<std::string, unsigned int> texture_map;
    static float animation_timer = 0.0f;
    static unsigned int animation_counter = 0;
    static std::unordered_map<std::string, GraphicCharTemplate> charset;
    static Texture palette_texture;
    static float palette;
    static struct
    {
        GLint x;
        GLint y;
        GLint w;
        GLint h;
    }
    main_viewport;
    static struct
    {
        GLint x;
        GLint y;
        GLint w;
        GLint h;
    }
    border_viewport;
    static Texture border_texture;
    static int * layer_pos;
    static int * gfx_ptrs_id_to_pos;
    static int * gfx_ptrs_pos_to_id;
    static int * state_for_gfx;
    static int * layer_for_gfx;
    static unsigned int state = 0;
    static float canvas_opacity = 1.0f;
    static std::unordered_map<std::string, GraphicCharTemplate> charmap;
    static GraphicCharTemplate fallback =
    {
        GraphicCharType::NORMAL,
        0,
        0,
        8,
        8,
        0,
        nullptr
    };
    static std::vector<float> times;

    static void FramebufferSizeCallback( GLFWwindow * window, int width, int height );
    static unsigned int GenerateShaderProgram( const Shader * shaders, int shadersnum );
    static void setMVP( unsigned int shader );
    static RawGraphic & getGraphic( unsigned int id );
    static unsigned int addGraphic( RawGraphic graphic );
    static void changeGraphicLayer( unsigned int id, unsigned int layer );
    static unsigned int getStateLayerIndex( unsigned int layer );
    static bool growGraphics();
    static int GetCharacterSize( const char * s );

    void Graphic::setLayer( BSL::Layer layer )
    {
        getGraphic( id_ ).layer = layer;
        changeGraphicLayer( id_, static_cast<unsigned int> ( layer ) );
    };

    void Graphic::setOpacity( float opacity )
    {
        getGraphic( id_ ).opacity = opacity;
    };

    void Sprite::setX( int v )
    {
        getGraphic( id_ ).data.sprite.x = v;
    };

    void Sprite::setY( int v )
    {
        getGraphic( id_ ).data.sprite.y = v;
    };

    void Sprite::setSrcX( unsigned int v )
    {
        getGraphic( id_ ).data.sprite.srcx = v;
    };
    
    void RawSprite::setY( int v )
    {
        getGraphic( id_ ).data.rawsprite.y = v;
    };

    void RawSprite::setSrcX( unsigned int v )
    {
        getGraphic( id_ ).data.rawsprite.srcx = v;
    };

    void RawSprite::setSrcY( unsigned int v )
    {
        getGraphic( id_ ).data.rawsprite.srcy = v;
    };

    void Tilemap::setY( int v )
    {
        getGraphic( id_ ).data.tilemap.y = v;
    };

    int init()
    {
        glfwInit();

        // Init window
        glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
        glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
        glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
        glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );

        // Init camera
        camera.x = 0;
        camera.y = 0;

        // Set up window
        window = glfwCreateWindow( BSL::WINDOW_WIDTH_PIXELS * magnification, BSL::WINDOW_HEIGHT_PIXELS * magnification, "Boskeopolis Land", NULL, NULL );
        if ( window == NULL )
        {
            BSL::log( "Failed to create GLFW window." );
            return -1;
        }
        glfwMakeContextCurrent( window );

        // Set up GLAD
        if ( !gladLoadGLLoader( ( GLADloadproc )( glfwGetProcAddress ) ) )
        {
            BSL::log( "Failed to initialize GLAD." );
            return -1;
        }

        // Update viewport on window resize.
        glfwSetFramebufferSizeCallback( window, FramebufferSizeCallback );

        // Turn on blending.
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

        // Allocate textures.
        glGenTextures( GL_TEXTURE_COUNT, gl_textures );

        // Init graphics.
        graphics = static_cast<RawGraphic *> ( calloc( max_graphics, sizeof( RawGraphic ) ) );
        gfx_ptrs_id_to_pos = static_cast<int *> ( calloc( max_graphics, sizeof( int ) ) );
        gfx_ptrs_pos_to_id = static_cast<int *> ( calloc( max_graphics, sizeof( int ) ) );
        state_for_gfx = static_cast<int *> ( calloc( max_graphics, sizeof( int ) ) );
        layer_for_gfx = static_cast<int *> ( calloc( max_graphics, sizeof( int ) ) );
        layer_pos = static_cast<int *> ( calloc( BSL::MAX_STATES * BSL::MAX_LAYERS, sizeof( int ) ) );

        // Initialize these to null values ( since 0 is a valid value, we use -1 ).
        for ( int i = 0; i < max_graphics; ++i )
        {
            gfx_ptrs_id_to_pos[ i ] = gfx_ptrs_pos_to_id[ i ] = state_for_gfx[ i ] = layer_for_gfx[ i ] = -1;
        }

        // Load palette texture.
        std::string palette_filename = std::string( "assets/palettes/main.png" );
        palette_texture.data = stbi_load
        (
            palette_filename.c_str(),
            ( int * )( &palette_texture.w ),
            ( int * )( &palette_texture.h ),
            nullptr, STBI_rgb_alpha
        );

        if ( !palette_texture.data )
        {
            BSL::log( "GFX Init Error: could not load palette data from “%s”.", palette_filename.c_str() );
            return -1;
        }

        glActiveTexture( GL_TEXTURE0 );
        glTexImage2D( GL_TEXTURE_2D, 0, GL_R8, ( unsigned int )( BSL::WINDOW_WIDTH_PIXELS ), ( unsigned int )( BSL::WINDOW_HEIGHT_PIXELS ), 0, GL_RED, GL_UNSIGNED_BYTE, gl_texture_data );
        glBindTexture( GL_TEXTURE_2D, gl_textures[ 0 ] );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

        glActiveTexture( GL_TEXTURE1 );
        glBindTexture( GL_TEXTURE_2D, gl_textures[ 1 ] );
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, ( unsigned int )( palette_texture.w ), ( unsigned int )( palette_texture.h ), 0, GL_RGBA, GL_UNSIGNED_BYTE, palette_texture.data );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

        // Load screen border texture.
        std::string border_filename = std::string( "assets/graphics/misc/screen-border.png" );
        border_texture.data = stbi_load
        (
            border_filename.c_str(),
            ( int * )( &border_texture.w ),
            ( int * )( &border_texture.h ),
            nullptr, STBI_rgb_alpha
        );

        if ( !border_texture.data )
        {
            BSL::log( "GFX Init Error: could not load border image data from “%s”.", border_filename.c_str() );
            return -1;
        }

        glActiveTexture( GL_TEXTURE2 );
        glBindTexture( GL_TEXTURE_2D, gl_textures[ 2 ] );
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, border_texture.w, border_texture.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, border_texture.data );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

        // VBO
        unsigned int VBO;
        glGenBuffers( 1, &VBO );
        glBindBuffer( GL_ARRAY_BUFFER, VBO );
        float vertices[] =
        {
            // Vertices     // Texture coords
            0.5f,  0.5f,   1.0f, 1.0f,
            0.5f, -0.5f,   1.0f, 0.0f,
            -0.5f, -0.5f,   0.0f, 0.0f,
            -0.5f,  0.5f,   0.0f, 1.0f,
        };
        glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

        // VAO
        glGenVertexArrays( 1, &VAO );
        glBindVertexArray( VAO );

        // EBO
        unsigned int EBO;
        glGenBuffers( 1, &EBO );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );
        unsigned int indices[] =
        {  // note that we start from 0!
            0, 1, 3,   // first triangle
            1, 2, 3    // second triangle
        };
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STATIC_DRAW );

        // Bind buffers
        glBindBuffer( GL_ARRAY_BUFFER, VBO );
        glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );
        glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, VERTEX_SIZE * sizeof( float ), 0 );
        glEnableVertexAttribArray( 0 );
        glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, VERTEX_SIZE * sizeof( float ), ( void * )( 2 * sizeof( float ) ) );
        glEnableVertexAttribArray( 1 );

        // Set up shaders
        Shader main_shaders[] =
        {
            {
                ShaderType::VERTEX,
                "#version 330 core\n layout ( location = 0 ) in vec2 in_position;\n layout ( location = 1 ) in vec2 in_texture_coords;\n \n out vec2 texture_coords;\n out vec4 out_color;\n out vec2 out_position;\n \n uniform mat4 model;\n uniform mat4 view;\n uniform mat4 ortho;\n \n void main()\n {\n out_position = in_position;\n gl_Position = ortho * view * model * vec4( in_position, 0.0, 1.0 );\n texture_coords = in_texture_coords;\n }"
            },
            {
                ShaderType::FRAGMENT,
                "#version 330 core\nout vec4 final_color;\n\nin vec2 out_position;\n\nin vec2 texture_coords;\n\nuniform sampler2D texture_data;\nuniform sampler2D palette_data;\nuniform float palette_no;\nuniform float canvas_opacity;\nvoid main()\n{\n    float color = ( texture( texture_data, texture_coords ).r * 256.0 - 1.0 ) / 256.0;\n    final_color = vec4( texture( palette_data, vec2( color, palette_no ) ).rgb, canvas_opacity );\n}"
            }
        };
        Shader border_shaders[] =
        {
            {
                ShaderType::VERTEX,
                "#version 330 core\n layout ( location = 0 ) in vec2 in_position;\n layout ( location = 1 ) in vec2 in_texture_coords;\n \n out vec2 texture_coords;\n out vec4 out_color;\n out vec2 out_position;\n \n uniform mat4 model;\n uniform mat4 view;\n uniform mat4 ortho;\n \n void main()\n {\n out_position = in_position;\n gl_Position = ortho * view * model * vec4( in_position, 0.0, 1.0 );\n texture_coords = in_texture_coords;\n }"
            },
            {
                ShaderType::FRAGMENT,
                "#version 330 core\nout vec4 final_color;\n\nin vec2 out_position;\n\nin vec2 texture_coords;\n\nuniform sampler2D texture_data;\n\nvoid main()\n{\n    final_color = texture( texture_data, texture_coords );\n}"
            }
        };
        border_shader = GenerateShaderProgram( border_shaders, 2 );
        glUseProgram( border_shader );
        GLint border_texture_uniform = glGetUniformLocation( border_shader, "texture_data" );
        glActiveTexture( GL_TEXTURE2 );
        glBindTexture( GL_TEXTURE_2D, gl_textures[ 2 ] );
        glUniform1i( border_texture_uniform, 2 );
        setMVP( border_shader );

        main_shader = GenerateShaderProgram( main_shaders, 2 );
        glUseProgram( main_shader );
        uniforms.texture = glGetUniformLocation( main_shader, "texture_data" );
        uniforms.palette_texture = glGetUniformLocation( main_shader, "palette_data" );
        uniforms.palette_no = glGetUniformLocation( main_shader, "palette_no" );
        uniforms.canvas_opacity = glGetUniformLocation( main_shader, "canvas_opacity" );
        setMVP( main_shader );

        // Setup viewports
        main_viewport =
        {
            0,
            0,
            static_cast<GLint> ( BSL::WINDOW_WIDTH_PIXELS ),
            static_cast<GLint> ( BSL::WINDOW_HEIGHT_PIXELS )
        };
        border_viewport =
        {
            0,
            0,
            static_cast<GLint> ( BSL::WINDOW_WIDTH_PIXELS ),
            static_cast<GLint> ( BSL::WINDOW_HEIGHT_PIXELS )
        };

        // Init charset.
        std::string charset_filename = std::string( "assets/graphics/charset/latin.png" );
        Texture charset_texture;
        charset_texture.data = stbi_load
        (
            charset_filename.c_str(),
            ( int * )( &charset_texture.w ),
            ( int * )( &charset_texture.h ),
            nullptr, STBI_grey
        );

        if ( !charset_texture.data )
        {
            BSL::log( "GFX init Error: could not load data from “%s”.", charset_filename.c_str() );
            return -1;
        }

        BSL::JSON json { "assets/charset/latin.json" };
        auto chars = json.getArray( "characters" );
        chars.forEach
        (
            [ & ]( const JSONItem & c )
            {
                auto o = c.asObject();
                const auto key = o.getString( "key" );
                GraphicCharTemplate data
                {
                    GraphicCharType::NORMAL,
                    0,
                    0,
                    8,
                    8
                };

                if ( o.hasInt( "x" ) )
                {
                    data.x = o.getInt( "x" );
                }
                if ( o.hasInt( "y" ) )
                {
                    data.y = o.getInt( "y" );
                }
                if ( o.hasInt( "w" ) )
                {
                    data.w = o.getInt( "w" );
                }
                if ( o.hasInt( "h" ) )
                {
                    data.h = o.getInt( "h" );
                }

                // Get character pixels.
                std::vector<CharPixel> pixels;
                std::vector<CharPixel> shadow;
                for ( unsigned int y = 0; y < data.h; ++y )
                {
                    for ( unsigned int x = 0; x < data.w; ++x )
                    {
                        const unsigned int dx = data.x + x;
                        const unsigned int dy = data.y + y;
                        const unsigned int i = dy * charset_texture.w + dx;
                        if ( charset_texture.data[ i ] == 0xFF )
                        {
                            pixels.push_back
                            ({
                                static_cast<uint_fast8_t> ( x ),
                                static_cast<uint_fast8_t> ( y )
                            });
                        }
                        else if ( charset_texture.data[ i ] == 0x01 )
                        {
                            shadow.push_back
                            ({
                                static_cast<uint_fast8_t> ( x ),
                                static_cast<uint_fast8_t> ( y )
                            });
                        }
                    }
                }
                data.pixel_count = pixels.size();
                const size_t size = data.pixel_count * sizeof( CharPixel );
                data.pixels = static_cast<CharPixel *> ( malloc( size ) );
                memcpy( data.pixels, &pixels[ 0 ], size );
                data.shadow_count = shadow.size();
                const size_t shadow_size = data.shadow_count * sizeof( CharPixel );
                data.shadow = static_cast<CharPixel *> ( malloc( shadow_size ) );
                memcpy( data.shadow, &shadow[ 0 ], shadow_size );

                if ( o.hasString( "type" ) )
                {
                    const auto type = o.getString( "type" );
                    if ( type == "whitespace" )
                    {
                        data.type = GraphicCharType::WHITESPACE;
                    }
                    else if ( type == "newline" )
                    {
                        data.type = GraphicCharType::NEWLINE;
                    }
                }

                if ( std::string( key ) == "fallback" )
                {
                    fallback = data;
                }
                else
                {
                    charmap.insert( { key, data } );
                }
            }
        );

        free( charset_texture.data );

        return 0;
    };

    void close()
    {
        float sum = 0.0f;
        for ( auto n : times )
        {
            sum += n;
        }
        printf( "Average Render Time: %f\n", sum / times.size() );
        glfwTerminate();
    };

    void update( float dt )
    {
        const float fps = 60.0 / dt;
        memset( gl_texture_data, 0xFF, BSL::WINDOW_WIDTH_PIXELS * BSL::WINDOW_HEIGHT_PIXELS );

        double start = getTime();
        for ( unsigned int i = 0; i < graphics_count; ++i )
        {
            switch ( graphics[ i ].type )
            {
                case ( GraphicType::RECT ):
                {
                    const int cx = graphics[ i ].abs ? graphics[ i ].data.rect.coords.x : graphics[ i ].data.rect.coords.x - camera.x;
                    const int cy = graphics[ i ].abs ? graphics[ i ].data.rect.coords.y : graphics[ i ].data.rect.coords.y - camera.y;
                    const unsigned int x = cx < 0 ? 0 : cx;
                    const unsigned int y = cy < 0 ? 0 : cy;
                    int w = graphics[ i ].data.rect.coords.w;
                    int h = graphics[ i ].data.rect.coords.h;
                    if ( cx < 0 )
                    {
                        w += cx;
                    }
                    const int right = w + ( int )( x );
                    if ( right > ( int )( BSL::WINDOW_WIDTH_PIXELS ) )
                    {
                        w -= right - BSL::WINDOW_WIDTH_PIXELS;
                    }
                    if ( cy < 0 )
                    {
                        h += cy;
                    }
                    const int bottom = h + ( int )( y );
                    if ( bottom > ( int )( BSL::WINDOW_HEIGHT_PIXELS ) )
                    {
                        h -= bottom - BSL::WINDOW_HEIGHT_PIXELS;
                    }

                    if ( w < 0 || h < 0 )
                    {
                        continue;
                    }

                    const float colordiff = graphics[ i ].data.rect.color2 - graphics[ i ].data.rect.color1;

                    for ( unsigned int yi = 0; yi < h; ++yi )
                    {
                        const float percent = ( float )( yi + 1 ) / h;
                        const float diff = colordiff * percent;
                        const unsigned char color = graphics[ i ].data.rect.color1 + diff;
                        const unsigned int desty = y + yi;
                        for ( unsigned int xi = 0; xi < w; ++xi )
                        {
                            const unsigned int destx = x + xi;
                            const int alphadiff = ( ( int )( color ) - ( int )( gl_texture_data[ BSL::WINDOW_WIDTH_PIXELS * desty + destx ] ) ) * graphics[ i ].opacity;
                            gl_texture_data[ BSL::WINDOW_WIDTH_PIXELS * desty + destx ] += alphadiff;
                        }
                    }
                }
                break;
                case ( GraphicType::SPRITE ):
                {
                    const auto & sprite = graphics[ i ].data.sprite;
                    const Texture & texture = textures[ sprite.texture ];
                    const int cx = graphics[ i ].abs ? sprite.x : sprite.x - camera.x;
                    const int cy = graphics[ i ].abs ? sprite.y : sprite.y - camera.y;
                    const int xsub = cx < 0 ? cx : 0;
                    const int ysub = cy < 0 ? cy : 0;
                    const unsigned int x = cx < 0 ? 0 : cx;
                    const unsigned int y = cy < 0 ? 0 : cy;
                    int w = std::min( sprite.w, texture.w ) + std::min( 0, cx );
                    int h = std::min( sprite.h, texture.h ) + std::min( 0, cy );
                    const int right = w + ( int )( x );
                    if ( right > ( int )( BSL::WINDOW_WIDTH_PIXELS ) )
                    {
                        w -= right - BSL::WINDOW_WIDTH_PIXELS;
                    }
                    const int bottom = h + ( int )( y );
                    if ( bottom > ( int )( BSL::WINDOW_HEIGHT_PIXELS ) )
                    {
                        h -= bottom - BSL::WINDOW_HEIGHT_PIXELS;
                    }

                    if ( w < 0 || h < 0 )
                    {
                        continue;
                    }

                    for ( unsigned int yi = 0; yi < h; ++yi )
                    {
                        const unsigned int desty = y + yi;
                        const unsigned int srcy = sprite.flipy ? h - yi - 1 : yi;
                        for ( unsigned int xi = 0; xi < w; ++xi )
                        {
                            const unsigned int srcx = sprite.flipx ? w - xi - 1 : xi;
                            const unsigned char v = texture.data
                            [
                                ( sprite.srcy - ysub + srcy ) * texture.w + sprite.srcx - xsub + srcx
                            ];

                            // Skip transparent.
                            if ( v == 0x00 )
                            {
                                continue;
                            }

                            const unsigned int destx = x + xi;
                            const int diff = ( ( int )( v ) - ( int )( gl_texture_data[ BSL::WINDOW_WIDTH_PIXELS * desty + destx ] ) ) * graphics[ i ].opacity;
                            gl_texture_data[ BSL::WINDOW_WIDTH_PIXELS * desty + destx ] += diff;
                        }
                    }
                }
                break;
                case ( GraphicType::TEXT ):
                {
                    const auto & t = graphics[ i ].data.text;
                    // For each character.
                    for ( unsigned int j = 0; j < t.char_count; ++j )
                    {
                        const auto & chr = t.chars[ j ];

                        // Set coords relative to camera if not set to abs.
                        const int x = graphics[ i ].abs ? chr.x : chr.x - camera.x;
                        const int y = graphics[ i ].abs ? chr.y : chr.y - camera.y;

                        // Skip character if completely offscreen.
                        if ( x > BSL::WINDOW_WIDTH_PIXELS || y > BSL::WINDOW_HEIGHT_PIXELS || x + chr.w < 0 || y + chr.h < 0 )
                        {
                            continue;
                        }

                        // Loop thru character pixels.
                        for ( unsigned int k = 0; k < chr.pixel_count; ++k )
                        {
                            const auto & c = chr.pixels[ k ];
                            const int dy = c.y + y;
                            const int dx = c.x + x;

                            // Skip if pixel is offscreen.
                            if ( dx < 0 || dy < 0 || dx >= BSL::WINDOW_WIDTH_PIXELS || dy >= BSL::WINDOW_HEIGHT_PIXELS )
                            {
                                continue;
                            }

                            // Get gradient color.
                            const float hdiff = static_cast<float> ( static_cast<double> ( c.y ) / static_cast<double> ( chr.h ) );
                            const int colordiff = static_cast<int> ( t.color1 ) - static_cast<int> ( ( static_cast<float> ( t.color1 - t.color2 ) * hdiff ) );

                            // Apply opacity.
                            const int diff = ( colordiff - static_cast<int> ( gl_texture_data[ BSL::WINDOW_WIDTH_PIXELS * dy + dx ] ) ) * graphics[ i ].opacity;

                            // Apply pixel to canvas.
                            gl_texture_data[ BSL::WINDOW_WIDTH_PIXELS * dy + dx ] += diff;
                        }

                        // If shadow is set, loop thru shadow pixels.
                        if ( t.shadow )
                        {
                            for ( unsigned int k = 0; k < chr.shadow_count; ++k )
                            {
                                const auto & c = chr.shadow[ k ];
                                const int dy = c.y + y;
                                const int dx = c.x + x;

                                // Skip if pixel is offscreen.
                                if ( dx < 0 || dy < 0 || dx >= BSL::WINDOW_WIDTH_PIXELS || dy >= BSL::WINDOW_HEIGHT_PIXELS )
                                {
                                    continue;
                                }

                                // Apply opacity.
                                const int diff = ( 1 - static_cast<int> ( gl_texture_data[ BSL::WINDOW_WIDTH_PIXELS * dy + dx ] ) ) * graphics[ i ].opacity;

                                // Apply pixel to canvas.
                                gl_texture_data[ BSL::WINDOW_WIDTH_PIXELS * dy + dx ] += diff;
                            }
                        }
                    }
                }
                break;
                case ( GraphicType::TILEMAP ):
                {
                    const auto & tilemap = graphics[ i ].data.tilemap;
                    const Texture & texture = textures[ tilemap.texture ];
                    for ( unsigned int y = 0; y < tilemap.h; ++y )
                    {
                        for ( unsigned int x = 0; x < tilemap.w; ++x )
                        {
                            const unsigned int n = y * tilemap.w + x;

                            // Skip tiles not set.
                            if ( !tilemap.tiles[ n ].set )
                            {
                                continue;
                            }

                            const unsigned int xp = x * 16 + tilemap.x;
                            const unsigned int yp = y * 16 + tilemap.y;
                            const int cx = graphics[ i ].abs ? xp : xp - camera.x;
                            const int cy = graphics[ i ].abs ? yp : yp - camera.y;
                            const int xsub = cx < 0 ? cx : 0;
                            const int ysub = cy < 0 ? cy : 0;
                            const unsigned int fx = cx < 0 ? 0 : cx;
                            const unsigned int fy = cy < 0 ? 0 : cy;
                            int w = 16;
                            int h = 16;
                            if ( cx < 0 )
                            {
                                w += cx;
                            }
                            const int right = w + ( int )( fx );
                            if ( right > ( int )( BSL::WINDOW_WIDTH_PIXELS ) )
                            {
                                w -= right - BSL::WINDOW_WIDTH_PIXELS;
                            }
                            if ( cy < 0 )
                            {
                                h += cy;
                            }
                            const int bottom = h + ( int )( fy );
                            if ( bottom > ( int )( BSL::WINDOW_HEIGHT_PIXELS ) )
                            {
                                h -= bottom - BSL::WINDOW_HEIGHT_PIXELS;
                            }

                            if ( w < 0 || h < 0 )
                            {
                                continue;
                            }

                            const unsigned int srcx = 
                                tilemap.tiles[ n ].animation > 0
                                ? tilemap.tiles[ n ].x + ( animation_counter % tilemap.tiles[ n ].animation )
                                : tilemap.tiles[ n ].x;
                            for ( unsigned int yi = 0; yi < h; ++yi )
                            {
                                const unsigned int sy = fy + yi;
                                for ( unsigned int xi = 0; xi < w; ++xi )
                                {
                                    const unsigned char v = texture.data
                                    [
                                        ( ( tilemap.tiles[ n ].y * 16 ) - ysub + yi ) * texture.w + ( srcx * 16 ) - xsub + xi
                                    ];

                                    // Skip transparent.
                                    if ( v == 0x00 )
                                    {
                                        continue;
                                    }

                                    const unsigned int sx = fx + xi;
                                    const int diff = ( ( int )( v ) - ( int )( gl_texture_data[ BSL::WINDOW_WIDTH_PIXELS * sy + sx ] ) ) * graphics[ i ].opacity;
                                    gl_texture_data[ BSL::WINDOW_WIDTH_PIXELS * sy + sx ] += diff;
                                }
                            }
                        }
                    }
                }
                break;
                case ( GraphicType::SPRITE_RAW ):
                {
                    const int cx = graphics[ i ].abs ? graphics[ i ].data.rawsprite.x : graphics[ i ].data.rawsprite.x - camera.x;
                    const int cy = graphics[ i ].abs ? graphics[ i ].data.rawsprite.y : graphics[ i ].data.rawsprite.y - camera.y;
                    const unsigned int x = cx < 0 ? 0 : cx;
                    const unsigned int y = cy < 0 ? 0 : cy;
                    int w = graphics[ i ].data.rawsprite.w < graphics[ i ].data.rawsprite.dataw ? graphics[ i ].data.rawsprite.w : graphics[ i ].data.rawsprite.dataw;
                    int h = graphics[ i ].data.rawsprite.h < graphics[ i ].data.rawsprite.datah ? graphics[ i ].data.rawsprite.h : graphics[ i ].data.rawsprite.datah;
                    if ( cx < 0 )
                    {
                        w += cx;
                    }
                    const int right = w + ( int )( x );
                    if ( right > ( int )( BSL::WINDOW_WIDTH_PIXELS ) )
                    {
                        w -= right - BSL::WINDOW_WIDTH_PIXELS;
                    }
                    if ( cy < 0 )
                    {
                        h += cy;
                    }
                    const int bottom = h + ( int )( y );
                    if ( bottom > ( int )( BSL::WINDOW_HEIGHT_PIXELS ) )
                    {
                        h -= bottom - BSL::WINDOW_HEIGHT_PIXELS;
                    }

                    if ( w < 0 || h < 0 )
                    {
                        continue;
                    } 

                    for ( unsigned int yi = 0; yi < h; ++yi )
                    {
                        const unsigned int sy = y + yi;
                        for ( unsigned int xi = 0; xi < w; ++xi )
                        {
                            const unsigned char v = graphics[ i ].data.rawsprite.data
                            [
                                ( graphics[ i ].data.rawsprite.srcy + yi ) * graphics[ i ].data.rawsprite.dataw + graphics[ i ].data.rawsprite.srcx + xi
                            ];

                            // Skip transparent.
                            if ( v == 0x00 )
                            {
                                continue;
                            }

                            const unsigned int sx = x + xi;
                            const int diff = ( ( int )( v ) - ( int )( gl_texture_data[ BSL::WINDOW_WIDTH_PIXELS * sy + sx ] ) ) * graphics[ i ].opacity;
                            gl_texture_data[ BSL::WINDOW_WIDTH_PIXELS * sy + sx ] += diff;
                        }
                    }
                }
                break;
                case ( GraphicType::SPRITE_RAW_NO_TRANSPARENCY ):
                {
                    const int cx = graphics[ i ].abs ? graphics[ i ].data.rawsprite.x : graphics[ i ].data.rawsprite.x - camera.x;
                    const int cy = graphics[ i ].abs ? graphics[ i ].data.rawsprite.y : graphics[ i ].data.rawsprite.y - camera.y;
                    const unsigned int x = cx < 0 ? 0 : cx;
                    const unsigned int y = cy < 0 ? 0 : cy;
                    int w = graphics[ i ].data.rawsprite.w < graphics[ i ].data.rawsprite.dataw ? graphics[ i ].data.rawsprite.w : graphics[ i ].data.rawsprite.dataw;
                    int h = graphics[ i ].data.rawsprite.h < graphics[ i ].data.rawsprite.datah ? graphics[ i ].data.rawsprite.h : graphics[ i ].data.rawsprite.datah;
                    if ( cx < 0 )
                    {
                        w += cx;
                    }
                    const int right = w + ( int )( x );
                    if ( right > ( int )( BSL::WINDOW_WIDTH_PIXELS ) )
                    {
                        w -= right - BSL::WINDOW_WIDTH_PIXELS;
                    }
                    if ( cy < 0 )
                    {
                        h += cy;
                    }
                    const int bottom = h + ( int )( y );
                    if ( bottom > ( int )( BSL::WINDOW_HEIGHT_PIXELS ) )
                    {
                        h -= bottom - BSL::WINDOW_HEIGHT_PIXELS;
                    }

                    if ( w < 0 || h < 0 )
                    {
                        continue;
                    } 

                    for ( unsigned int yi = 0; yi < h; ++yi )
                    {
                        const unsigned int sy = y + yi;
                        const unsigned int sx = ( graphics[ i ].data.rawsprite.srcy + yi ) * graphics[ i ].data.rawsprite.dataw + graphics[ i ].data.rawsprite.srcx;
                        memcpy( &gl_texture_data[ BSL::WINDOW_WIDTH_PIXELS * sy + x ], &graphics[ i ].data.rawsprite.data[ sx ], w );
                    }
                }
                break;
            }
        }
        times.push_back( getTime() - start );

        unsigned char v = fps < 60.0f ? 0x80 : 0x00;
        for ( unsigned int i = 0; i < 8; ++i )
        {
            memset( &gl_texture_data[ BSL::WINDOW_WIDTH_PIXELS * ( 8 + i ) + 8 ], v, ( unsigned int )( fps ) );
        }

        glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT );

        glViewport( main_viewport.x, main_viewport.y, main_viewport.w, main_viewport.h );
        glUseProgram( main_shader );
        glActiveTexture( GL_TEXTURE0 );
        glTexImage2D( GL_TEXTURE_2D, 0, GL_R8, BSL::WINDOW_WIDTH_PIXELS, BSL::WINDOW_HEIGHT_PIXELS, 0, GL_RED, GL_UNSIGNED_BYTE, gl_texture_data );
        glBindTexture( GL_TEXTURE_2D, gl_textures[ 0 ] );
        glUniform1i( uniforms.texture, 0 );
        glActiveTexture( GL_TEXTURE1 );
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, palette_texture.w, palette_texture.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, palette_texture.data );
        glBindTexture( GL_TEXTURE_2D, gl_textures[ 1 ] );
        glUniform1i( uniforms.palette_texture, 1 );
        glUniform1f( uniforms.palette_no, palette );
        glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );

        glViewport( border_viewport.x, border_viewport.y, border_viewport.w, border_viewport.h );
        glUseProgram( border_shader );
        glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );

        glfwSwapBuffers( window );
        glfwPollEvents();

        animation_timer += dt;
        while ( animation_timer >= 8.0f )
        {
            animation_timer -= 8.0f;
            ++animation_counter;
            if ( animation_counter >= ANIMATION_MAX )
            {
                animation_counter = 0;
            }
        }
    };

    int testHasClosed()
    {
        return glfwWindowShouldClose( window );
    };

    void handleEvents()
    {
        glfwPollEvents();
    };

    double getTime()
    {
        return glfwGetTime();
    };

    void registerInputHandler( input_handle_t handler )
    {
        glfwSetKeyCallback( window, ( GLFWkeyfun )( handler ) );
    };

    Rect addGraphicRect
    (
        int x,
        int y,
        unsigned int w,
        unsigned int h,
        unsigned char color,
        BSL::ArgList args
    )
    {
        return { addGraphicRectGradient
        (
            x,
            y,
            w,
            h,
            color,
            color,
            args
        ).id_ };
    };

    RectGradient addGraphicRectGradient
    (
        int x,
        int y,
        unsigned int w,
        unsigned int h,
        unsigned char color1,
        unsigned char color2,
        BSL::ArgList args
    )
    {
        RawGraphic g;
        g.type = GraphicType::RECT;
        g.abs = BSL::GetArg( "abs", args, false );
        g.layer = GetArg( "layer", args, BSL::Layer::BG_1 );
        g.opacity = BSL::GetArg( "opacity", args, 1.0f );
        g.data.rect.coords.x = x;
        g.data.rect.coords.y = y;
        g.data.rect.coords.w = w;
        g.data.rect.coords.h = h;
        g.data.rect.color1 = color1;
        g.data.rect.color2 = color2;
        return { addGraphic( g ) };
    };

    Sprite addGraphicSprite
    (
        unsigned int texture,
        int x,
        int y,
        unsigned int w,
        unsigned int h,
        BSL::ArgList args
    )
    {
        RawGraphic g;
        g.type = GraphicType::SPRITE;
        g.abs = BSL::GetArg( "abs", args, false );
        g.layer = GetArg( "layer", args, BSL::Layer::SPRITES_1 );
        g.opacity = BSL::GetArg( "opacity", args, 1.0f );
        g.data.sprite.texture = texture;
        g.data.sprite.srcx = BSL::GetArg( "srcx", args, 0u );
        g.data.sprite.srcy = BSL::GetArg( "srcy", args, 0u );
        g.data.sprite.w = w;
        g.data.sprite.h = h;
        g.data.sprite.x = x;
        g.data.sprite.y = y;
        g.data.sprite.flipx = BSL::GetArg( "flipx", args, false );
        g.data.sprite.flipy = BSL::GetArg( "flipy", args, false );
        return { addGraphic( g ) };
    };

    Tilemap addGraphicTilemap
    (
        unsigned int texture,
        Tile * tiles,
        unsigned int w,
        unsigned int h,
        BSL::ArgList args
    )
    {
        RawGraphic g;
        g.type = GraphicType::TILEMAP;
        g.abs = BSL::GetArg( "abs", args, false );
        g.layer = GetArg( "layer", args, BSL::Layer::BLOCKS_1 );
        g.opacity = 1.0f;
        g.data.tilemap.texture = texture;
        g.data.tilemap.tiles = static_cast<Tile *> ( malloc( w * h * sizeof( Tile ) ) );
        memcpy( g.data.tilemap.tiles, tiles, w * h * sizeof( Tile ) );
        g.data.tilemap.w = w;
        g.data.tilemap.h = h;
        g.data.tilemap.x = GetArg( "x", args, 0 );
        g.data.tilemap.y = GetArg( "y", args, 0 );
        return { addGraphic( g ) };
    };

    void setPalette( unsigned char p )
    {
        palette = static_cast<float> ( static_cast<double> ( p ) / 256.0 );
    };

    void setCanvasOpacity( float o )
    {
        canvas_opacity = o;
        glUseProgram( main_shader );
        glUniform1f( uniforms.canvas_opacity, canvas_opacity );
    };

    void setState( uint_fast8_t s )
    {
        state = s;
    };

    void clearGraphics()
    {
        // Destroy specific graphic objects.
        for ( unsigned int i = 0; i < graphics_count; ++i )
        {
            //DestroyGraphic( &graphics[ i ] );
        }

        // Reset maps to null values ( since 0 is a valid value, we use -1 ).
        for ( unsigned int i = 0; i < max_graphics; ++i )
        {
            gfx_ptrs_id_to_pos[ i ] = gfx_ptrs_pos_to_id[ i ] = state_for_gfx[ i ] = layer_for_gfx[ i ] = -1;
        }

        for ( unsigned int i = 0; i < BSL::MAX_STATES * BSL::MAX_LAYERS; ++i )
        {
            layer_pos[ i ] = 0;
        }

        graphics_count = 0;
    };

    Text addGraphicText
    (
        const char * text,
        BSL::ArgList args
    )
    {
        int x = GetArg( "x", args, 0 );
        int y = GetArg( "y", args, 0 );
        unsigned int w = GetArg( "w", args, BSL::WINDOW_WIDTH_PIXELS );
        unsigned int h = GetArg( "h", args, BSL::WINDOW_HEIGHT_PIXELS );
        unsigned int padding_left = GetArg( "padding_left", args, 0u );
        unsigned int padding_top = GetArg( "padding_top", args, 0u );
        unsigned int padding_right = GetArg( "padding_right", args, 0u );
        unsigned int padding_bottom = GetArg( "padding_bottom", args, 0u );
        BSL::Align align = GetArg( "align", args, BSL::Align::LEFT );
        BSL::Valign valign = GetArg( "valign", args, BSL::Valign::TOP );

        // Generate char list from string.
        int charw = w - padding_left - padding_right;
        int charh = h - padding_top - padding_bottom;
        int charx = x + padding_left;
        int chary = y + padding_top;
        const int lnend = charx + charw;

        const char * string = text;
        GraphicCharTemplate letters[ strlen( string ) ];
        int lettercount = 0;
        while ( *string )
        {
            const int charlen = GetCharacterSize( string );

            // Generate letter string
            char letter[ charlen + 1 ];
            strncpy( letter, string, charlen );
            letter[ charlen ] = 0;
            std::string letstr = std::string( letter );

            // Find character
            auto it = charmap.find( std::string( letter ) );
            if ( it != charmap.end() )
            {
                letters[ lettercount ] = it->second;
            }
            else
            {
                letters[ lettercount ] = fallback;
            }

            ++lettercount;
            string += charlen;
        }
        int maxlines = ( int )( charh );
        int maxperline = ( int )( charw );
        GraphicCharTemplate lines[ maxlines ][ maxperline ];
        float line_widths[ maxlines ];
        line_widths[ 0 ] = 0;
        int line_character_counts[ maxlines ];
        int line_count = 0;
        int line_character = 0;
        long unsigned int i = 0;
        int lx = ( int )( charx );
        int endswithnewline[ maxlines ];
        while ( i < lettercount )
        {
            long unsigned int ib = i;
            float xb = lx;
            int look_ahead = 1;

            // Look ahead so we can know ahead o’ time whether we need to add a new line.
            // This autobreaks text without cutting midword.
            while ( look_ahead )
            {
                if ( ib >= lettercount )
                {
                    look_ahead = 0;
                    break;
                }

                if ( letters[ ib ].type == GraphicCharType::NEWLINE )
                {
                    look_ahead = 0;
                }
                else if ( letters[ ib ].type == GraphicCharType::WHITESPACE )
                {
                    look_ahead = 0;
                }
                else if ( xb >= lnend )
                {
                    lx = ( int )( charx );
                    line_character_counts[ line_count ] = line_character;
                    endswithnewline[ line_count ] = 0;
                    ++line_count;
                    line_widths[ line_count ] = 0;
                    line_character = 0;
                    look_ahead = 0;
                }
                else if ( ib >= lettercount )
                {
                    look_ahead = 0;
                    break;
                }
                xb += letters[ ib ].w;
                ++ib;
            }

            while ( i < ib )
            {
                if ( letters[ i ].type == GraphicCharType::NEWLINE || lx >= lnend )
                {
                    lx = ( int )( charx );
                    line_character_counts[ line_count ] = line_character;
                    endswithnewline[ line_count ] = letters[ i ].type == GraphicCharType::NEWLINE;
                    ++line_count;
                    line_widths[ line_count ] = 0;
                    line_character = 0;
                }
                else
                {
                    lines[ line_count ][ line_character ] = letters[ i ];
                    line_widths[ line_count ] += letters[ i ].w;
                    ++line_character;
                    lx += letters[ i ].w;
                }
                ++i;
            }
        }
        line_character_counts[ line_count ] = line_character;
        ++line_count;

        int finalcharcount = 0;
        float maxh[ line_count ];

        for ( int l = 0; l < line_count; ++l )
        {
            finalcharcount += line_character_counts[ l ];

            maxh[ l ] = 8.0f;
            for ( int c = 0; c < line_character_counts[ l ]; ++c )
            {
                if ( lines[ l ][ c ].h > maxh[ l ] )
                {
                    maxh[ l ] = lines[ l ][ c ].h;
                }
            }

            // Sometimes the previous loop keeps whitespace @ the end o’ lines.
            // Since this messes with x alignment, remove these.
            if ( lines[ l ][ line_character_counts[ l ] - 1 ].type == GraphicCharType::WHITESPACE )
            {
                --line_character_counts[ l ];
                line_widths[ l ] -= lines[ l ][ line_character_counts[ l ] - 1 ].w;
            }
        }

        GraphicChar chars[ finalcharcount ];
        int count = 0;
        // Final loop: we have all the info we need now to set x & y positions.
        int dy = ( valign == BSL::Valign::MIDDLE )
            ? chary + static_cast<int> ( ( charh - ( line_count * 8 ) ) / 2.0 )
            : ( valign == BSL::Valign::BOTTOM )
                ? chary + charh - ( line_count * 8 )
                : chary;
        for ( int l = 0; l < line_count; ++l )
        {
            int dx = ( align == BSL::Align::CENTER )
                ? charx + static_cast<int> ( ( charw - line_widths[ l ] ) / 2 )
                : ( align == BSL::Align::RIGHT )
                    ? lnend - line_widths[ l ]
                    : charx;

            // Add justified spacing if set to justified & not an endline.
            float letterspace = align == BSL::Align::JUSTIFIED && line_character_counts[ l ] > 0 && l < line_count - 1 && !endswithnewline[ l ]
                ? ( charw - line_widths[ l ] ) / ( float )( line_character_counts[ l ] - 1 )
                : 0.0f;

            for ( int c = 0; c < line_character_counts[ l ]; ++c )
            {
                // Just in case o’ character index misalignment, just copy o’er whole characters.
                if ( lines[ l ][ c ].type != GraphicCharType::WHITESPACE ) {
                    chars[ count ].srcx = lines[ l ][ c ].x;
                    chars[ count ].srcy = lines[ l ][ c ].y;
                    chars[ count ].w = lines[ l ][ c ].w;
                    chars[ count ].h = lines[ l ][ c ].h;
                    chars[ count ].x = dx;
                    chars[ count ].y = dy + static_cast<int> ( ( maxh[ l ] - lines[ l ][ c ].h ) / 2.0 );
                    chars[ count ].pixel_count = lines[ l ][ c ].pixel_count;
                    chars[ count ].pixels = lines[ l ][ c ].pixels;
                    chars[ count ].shadow_count = lines[ l ][ c ].shadow_count;
                    chars[ count ].shadow = lines[ l ][ c ].shadow;
                    ++count;
                }
                if ( lines[ l ][ c ].w > 0 ) {
                    dx += ( lines[ l ][ c ].w + letterspace );
                }
            }
            dy += maxh[ l ];
        }

        RawGraphic g;
        g.type = GraphicType::TEXT;
        g.abs = GetArg( "abs", args, true );
        g.layer = GetArg( "layer", args, BSL::Layer::FG_1 );
        g.opacity = GetArg( "opacity", args, 1.0f );
        g.data.text.char_count = count;
        g.data.text.chars = chars;
        g.data.text.shadow = GetArg( "shadow", args, false );
        g.data.text.color1 = g.data.text.color2 = GetArg( "color", args, 0xFF );
        return { addGraphic( g ) };
    };


    Menu addGraphicMenu
    (
        unsigned int w,
        unsigned int h,
        int x,
        int y,
        BSL::ArgList args
    )
    {
        RawGraphic g;
        g.type = GraphicType::SPRITE_RAW;
        g.abs = true;
        g.layer = GetArg( "layer", args, BSL::Layer::FG_1 );
        g.opacity = 1.0f;
        const unsigned char color = GetArg( "bgcolor", args, 0xFF );
        auto & data = g.data.rawsprite;
        data.w = data.dataw = w - 4;
        data.h = data.datah = h - 4;
        const size_t size = data.dataw * data.datah;
        data.data = static_cast<unsigned char *> ( malloc( size ) );
        memset( data.data, color, size );

        static constexpr unsigned char MENU_TOP_LEFT[ 6 * 6 ] =
        {
            0x00, 0x01, 0x01, 0x01, 0x01, 0x01,
            0x01, 0x01, 0x80, 0x80, 0x80, 0x80,
            0x01, 0x80, 0x80, 0x50, 0x50, 0x50,
            0x01, 0x80, 0x50, 0x50, 0x01, 0x01,
            0x01, 0x80, 0x50, 0x01, 0x01, 0x01,
            0x01, 0x80, 0x50, 0x01, 0x01, 0x01,
        };

        static constexpr unsigned char MENU_TOP_RIGHT[ 6 * 6 ] =
        {
            0x01, 0x01, 0x01, 0x01, 0x01, 0x00,
            0x80, 0x80, 0x80, 0x01, 0x01, 0x01,
            0x50, 0x50, 0x80, 0x30, 0x01, 0x01,
            0x01, 0x50, 0x50, 0x30, 0x01, 0x01,
            0x01, 0x01, 0x50, 0x30, 0x01, 0x01,
            0x01, 0x01, 0x50, 0x30, 0x01, 0x01,
        };

        static constexpr unsigned char MENU_BOTTOM_LEFT[ 6 * 6 ] =
        {
            0x01, 0x80, 0x50, 0x01, 0x01, 0x01,
            0x01, 0x80, 0x50, 0x50, 0x01, 0x01,
            0x01, 0x30, 0x80, 0x50, 0x50, 0x50,
            0x01, 0x01, 0x30, 0x30, 0x30, 0x30,
            0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
            0x00, 0x01, 0x01, 0x01, 0x01, 0x01,
        };

        static constexpr unsigned char MENU_BOTTOM_RIGHT[ 6 * 6 ] =
        {
            0x01, 0x01, 0x50, 0x30, 0x01, 0x01,
            0x01, 0x50, 0x50, 0x30, 0x01, 0x01,
            0x50, 0x50, 0x30, 0x30, 0x01, 0x01,
            0x30, 0x30, 0x30, 0x01, 0x01, 0x01,
            0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
            0x01, 0x01, 0x01, 0x01, 0x01, 0x00,
        };

        static constexpr unsigned char MENU_LEFT_SIDE[ 5 ] = { 0x01, 0x80, 0x050, 0x01, 0x01 };
        static constexpr unsigned char MENU_RIGHT_SIDE[ 5 ] = { 0x01, 0x80, 0x050, 0x01, 0x01 };

        #define COORDS( x, y ) ( &data.data[ ( data.dataw * ( y ) ) + ( x ) ] )

        // Generate corners.
        const unsigned rightx = data.w - 6;
        const unsigned bottomy = data.h - 6;
        for ( unsigned int y = 0; y < 6; ++y )
        {
            memcpy( COORDS( 0, y ), &MENU_TOP_LEFT[ 6 * y ], 6 );
            memcpy( COORDS( rightx, y ), &MENU_TOP_RIGHT[ 6 * y ], 6 );
            memcpy( COORDS( 0, bottomy + y ), &MENU_BOTTOM_LEFT[ 6 * y ], 6 );
            memcpy( COORDS( rightx, bottomy + y ), &MENU_BOTTOM_RIGHT[ 6 * y ], 6 );
        }

        // Generate top center.
        memset( COORDS( 6, 0 ), 0x01, data.dataw - 12 ); // Set 1st row to black
        memset( COORDS( 6, 1 ), 0x80, data.dataw - 12 ); // Set 2nd row to highlight
        memset( COORDS( 6, 2 ), 0x50, data.dataw - 12 ); // Set 3rd row to main
        memset( COORDS( 6, 3 ), 0x01, data.dataw - 12 ); // Set 4th row to black
        memset( COORDS( 6, 4 ), 0x01, data.dataw - 12 ); // Set 5th row to black

        // Generate bottom center.
        memset( COORDS( 6, data.h - 5 ), 0x01, data.dataw - 12 ); // Set 1st row to black
        memset( COORDS( 6, data.h - 4 ), 0x50, data.dataw - 12 ); // Set 2nd row to main
        memset( COORDS( 6, data.h - 3 ), 0x30, data.dataw - 12 ); // Set 3rd row to shadow
        memset( COORDS( 6, data.h - 2 ), 0x01, data.dataw - 12 ); // Set 4th row to black
        memset( COORDS( 6, data.h - 1 ), 0x01, data.dataw - 12 ); // Set 5th row to black

        // Generate left & right middle.
        for ( unsigned int y = 6; y < data.datah - 6; ++y )
        {
            memcpy( COORDS( 0, y ), MENU_LEFT_SIDE, 5 );
            memcpy( COORDS( data.dataw - 5, y ), MENU_LEFT_SIDE, 5 );
        }

        #undef COORDS

        data.srcx = 0;
        data.srcy = 0;
        data.x = x + 2;
        data.y = y + 2;
        return { addGraphic( g ) };
    };

    RawSprite addGraphicSpriteRaw
    (
        const unsigned char * data,
        unsigned int dataw,
        unsigned int datah,
        unsigned int w,
        unsigned int h,
        int x,
        int y,
        BSL::ArgList args
    )
    {
        RawGraphic g;
        g.abs = GetArg( "abs", args, true );
        g.layer = GetArg( "layer", args, BSL::Layer::SPRITES_1 );
        g.opacity = GetArg( "opacity", args, 1.0f );

        // Check if graphic uses transparency in any way.
        bool has_transparency = g.opacity != 1.0f;
        if ( !has_transparency )
        {
            for ( unsigned int i = 0; i < dataw * datah; ++i )
            {
                if ( data[ i ] == 0x00 )
                {
                    has_transparency = true;
                }
            }
        }

        // If graphic uses no transparency, set it to optimized no transparency type.
        g.type = has_transparency ? GraphicType::SPRITE_RAW : GraphicType::SPRITE_RAW_NO_TRANSPARENCY;

        g.data.rawsprite.data = ( unsigned char * )( malloc( dataw * datah ) );
        memcpy( g.data.rawsprite.data, data, dataw * datah );
        g.data.rawsprite.dataw = dataw;
        g.data.rawsprite.datah = datah;
        g.data.rawsprite.srcx = GetArg( "srcx", args, 0u );
        g.data.rawsprite.srcy = GetArg( "srcy", args, 0u );
        g.data.rawsprite.w = w;
        g.data.rawsprite.h = h;
        g.data.rawsprite.x = x;
        g.data.rawsprite.y = y;
        return { addGraphic( g ) };
    };

    unsigned int loadFileAsTexture( const char * filename )
    {
        const auto it = texture_map.find( filename );
        if ( it != texture_map.end() )
        {
            return it->second;
        }
        texture_map.insert( std::pair<std::string, unsigned int> ( filename, texture_count ) );

        std::string charset_filename = std::string( "assets/graphics/" + std::string( filename ) );
        textures[ texture_count ].data = stbi_load
        (
            charset_filename.c_str(),
            ( int * )( &textures[ texture_count ].w ),
            ( int * )( &textures[ texture_count ].h ),
            nullptr, STBI_grey
        );

        if ( !textures[ texture_count ].data )
        {
            BSL::log( "loadFileAsTexture Error: could not load data from “%s”.", charset_filename.c_str() );
            return 0;
        }

        return texture_count++;
    };

    unsigned int getCameraX()
    {
        return camera.x;
    };

    unsigned int getCameraY()
    {
        return camera.y;
    };

    void setCameraX( unsigned int v )
    {
        camera.x = v;
    };

    void setCameraY( unsigned int v )
    {
        camera.y = v;
    };

    static void FramebufferSizeCallback( GLFWwindow * window, int screen_width, int screen_height )
    {
        double screen_aspect_ratio = ( double )( BSL::WINDOW_WIDTH_PIXELS ) / ( double )( BSL::WINDOW_HEIGHT_PIXELS );
        double monitor_aspect_ratio = ( double )( screen_width ) / ( double )( screen_height );

        // Base magnification on max that fits in window.
        magnification = 
            ( int )( floor(
                ( monitor_aspect_ratio > screen_aspect_ratio )
                    ? ( double )( screen_height ) / ( double )( BSL::WINDOW_HEIGHT_PIXELS )
                    : ( double )( screen_width ) / ( double )( BSL::WINDOW_WIDTH_PIXELS )
            ));
        if ( magnification < 1 )
        {
            magnification = 1;
        }

        main_viewport.w = ( float )( BSL::WINDOW_WIDTH_PIXELS ) * magnification;
        main_viewport.h = ( float )( BSL::WINDOW_HEIGHT_PIXELS ) * magnification;
        main_viewport.x = ( int )( floor( ( double )( screen_width - main_viewport.w ) / 2.0 ) );
        main_viewport.y = ( int )( floor( ( double )( screen_height - main_viewport.h ) / 2.0 ) );
        border_viewport.w = ( float )( border_texture.w ) * magnification;
        border_viewport.h = ( float )( border_texture.h ) * magnification;
        border_viewport.x = ( int )( floor( ( double )( screen_width - border_viewport.w ) / 2.0 ) );
        border_viewport.y = ( int )( floor( ( double )( screen_height - border_viewport.h ) / 2.0 ) );
    };

    static unsigned int GenerateShaderProgram( const Shader * shaders, int shadersnum )
    {
        unsigned int program = glCreateProgram();
        unsigned int shader_ids[ shadersnum ];

        // Compile each shader and attach to program
        for ( int i = 0; i < shadersnum; ++i )
        {
            int success;
            char infoLog[ 512 ];

            GLenum type;
            switch ( shaders[ i ].type )
            {
                case ( ShaderType::VERTEX ):
                {
                    type = GL_VERTEX_SHADER;
                }
                break;
                case ( ShaderType::FRAGMENT ):
                {
                    type = GL_FRAGMENT_SHADER;
                }
                break;
                default:
                {
                    BSL::log( "Invalid shader type given to shader!" );
                }
                break;
            }

            shader_ids[ i ] = glCreateShader( type );
            glShaderSource( shader_ids[ i ], 1, ( const GLchar * const * )( &shaders[ i ].code ), NULL );
            glCompileShader( shader_ids[ i ] );
            glGetShaderiv( shader_ids[ i ], GL_COMPILE_STATUS, &success );
            if ( !success )
            {
                glGetShaderInfoLog( shader_ids[ i ], 512, NULL, infoLog );
                BSL::log( "ERROR::SHADER::VERTEX::COMPILATION_FAILED." );
                BSL::log( infoLog );
            }
            glAttachShader( program, shader_ids[ i ] );
        }

        glLinkProgram( program );

        // If there are any errors, log them
        int  success;
        char infoLog[ 512 ];
        glGetProgramiv( program, GL_LINK_STATUS, &success );
        if( !success )
        {
            glGetProgramInfoLog( program, 512, NULL, infoLog );
            BSL::log( "ERROR: Failed to create shader program." );
            BSL::log( infoLog );
        }

        // Delete shaders
        for ( int i = 0; i < shadersnum; ++i )
        {
            glDeleteShader( shader_ids[ i ] );
        }

        return program;
    };

    static void setMVP( unsigned int shader )
    {
        mat4 view = BASE_MATRIX;
        vec3 trans = { ( BSL::WINDOW_WIDTH_PIXELS / 2.0f ), ( BSL::WINDOW_HEIGHT_PIXELS / 2.0f ), 0.0f };
        glm_translate( view, trans );
        unsigned int view_location = glGetUniformLocation( shader, "view" );
        glUniformMatrix4fv( view_location, 1, GL_FALSE, ( float * )( view ) );
        mat4 model = BASE_MATRIX;
        vec3 scale = { static_cast<float> ( BSL::WINDOW_WIDTH_PIXELS ), static_cast<float> ( BSL::WINDOW_HEIGHT_PIXELS ), 0.0 };
        glm_scale( model, scale );
        unsigned int model_location = glGetUniformLocation( shader, "model" );
        glUniformMatrix4fv( model_location, 1, GL_FALSE, ( float * )( model ) );
        mat4 ortho =
        {
            { 1.0f, 1.0f, 1.0f, 1.0f },
            { 1.0f, 1.0f, 1.0f, 1.0f },
            { 1.0f, 1.0f, 1.0f, 1.0f },
            { 1.0f, 1.0f, 1.0f, 1.0f }
        };
        glm_ortho_rh_no( 0.0f, BSL::WINDOW_WIDTH_PIXELS, BSL::WINDOW_HEIGHT_PIXELS, 0.0f, -1.0f, 1.0f, ortho );
        unsigned int ortho_location = glGetUniformLocation( shader, "ortho" );
        glUniformMatrix4fv( ortho_location, 1, GL_FALSE, ( float * )( ortho ) );
    };

    static RawGraphic & getGraphic( unsigned int id )
    {
        return graphics[ gfx_ptrs_id_to_pos[ id ] ];
        //return graphics[ id ];
    };

    static unsigned int addGraphic( RawGraphic graphic )
    {
        if ( graphics_count >= max_graphics )
        {
            if ( !growGraphics() )
            {
                // TODO: Throw error.
                return 0;
            }
        }

        // Find last graphic of current layer.
        unsigned int layern = static_cast<unsigned int> ( graphic.layer );
        const unsigned int pp = getStateLayerIndex( layern );
        const unsigned int p = layer_pos[ pp ];

        // Push forward this & following positions.
        for ( unsigned int i = pp; i < BSL::MAX_LAYERS * BSL::MAX_STATES; ++i )
        {
            ++layer_pos[ i ];
        }

        // Push forward all following graphics.
        for ( unsigned int i = graphics_count++; i > p; --i )
        {
            graphics[ i ] = graphics[ i - 1 ];

            // Update pointers so they still point to correct graphics.
            const unsigned int t = gfx_ptrs_pos_to_id[ i - 1 ];
            ++gfx_ptrs_id_to_pos[ t ];
            gfx_ptrs_pos_to_id[ gfx_ptrs_id_to_pos[ t ] ] = t;
        }

        // Find 1st free graphic ID.
        int current_graphic_id = -1;
        for ( int i = 0; i < max_graphics; ++i )
        {
            if ( gfx_ptrs_id_to_pos[ i ] == -1 )
            {
                current_graphic_id = i;
                break;
            }
        }

        if ( current_graphic_id < 0 )
        {
            BSL::log( "¡Strange error! ¡All graphics IDs filled e’en tho we’re apparently below max graphics!" );
        }

        // Add current graphic & pointer.
        graphics[ p ] = graphic;
        gfx_ptrs_id_to_pos[ current_graphic_id ] = p;
        gfx_ptrs_pos_to_id[ p ] = current_graphic_id;
        state_for_gfx[ current_graphic_id ] = state;
        layer_for_gfx[ current_graphic_id ] = layern;

        return static_cast<unsigned int> ( current_graphic_id );
    };

    static void changeGraphicLayer( unsigned int id, unsigned int layer )
    {
        // Skip if already on layer.
        if ( layer_for_gfx[ id ] == static_cast<int> ( layer ) )
        {
            return;
        }

        // Make copy o’ graphic.
        RawGraphic gfx = graphics[ gfx_ptrs_id_to_pos[ id ] ];

        const unsigned int state = state_for_gfx[ id ];
        const unsigned int current_layer_index = getStateLayerIndex( layer_for_gfx[ id ] );
        const unsigned int target_layer_index = getStateLayerIndex( layer );
        const unsigned int target_layer_pos = layer_pos[ target_layer_index ];

        if ( layer < layer_for_gfx[ id ] )
        {
            // Push forward all following graphics.
            for ( unsigned int i = gfx_ptrs_id_to_pos[ id ]; i > target_layer_pos; --i )
            {
                graphics[ i ] = graphics[ i - 1 ];

                // Update pointers so they still point to correct graphics.
                const unsigned int t = gfx_ptrs_pos_to_id[ i - 1 ];
                ++gfx_ptrs_id_to_pos[ t ];            
                gfx_ptrs_pos_to_id[ gfx_ptrs_id_to_pos[ t ] ] = t;
            }

            graphics[ target_layer_pos ] = gfx;
            gfx_ptrs_id_to_pos[ id ] = target_layer_pos;
            gfx_ptrs_pos_to_id[ target_layer_pos ] = id;

            // Push up all layers from original point to selected layer.
            for ( unsigned int i = target_layer_index; i < current_layer_index; ++i )
            {
                ++layer_pos[ i ];
            }
        }
        else
        {
            // Push down all graphics between them.
            for ( unsigned int i = gfx_ptrs_id_to_pos[ id ]; i < target_layer_pos - 1; ++i )
            {
                graphics[ i ] = graphics[ i + 1 ];

                // Update pointers so they still point to correct graphics.
                const unsigned int t = gfx_ptrs_pos_to_id[ i + 1 ];
                --gfx_ptrs_id_to_pos[ t ];
                gfx_ptrs_pos_to_id[ gfx_ptrs_id_to_pos[ t ] ] = t;
            }

            // Move copied graphic to end of target layer & update pointers.
            graphics[ target_layer_pos - 1 ] = gfx;
            gfx_ptrs_id_to_pos[ id ] = target_layer_pos - 1;
            gfx_ptrs_pos_to_id[ target_layer_pos - 1 ] = id;

            // Push down all layers from original point to selected layer.
            for ( unsigned int i = current_layer_index; i < target_layer_index; ++i )
            {
                --layer_pos[ i ];
            }
        }
        layer_for_gfx[ id ] = layer;
    };

    static unsigned int getStateLayerIndex( unsigned int layer )
    {
        return state * BSL::MAX_LAYERS + layer;
    };

    static bool growGraphics()
    {
        const unsigned int new_max_graphics = max_graphics * 2;

        RawGraphic * new_graphics = static_cast<RawGraphic *>( calloc( new_max_graphics, sizeof( RawGraphic ) ) );
        int * new_gfx_ptrs_id_to_pos = static_cast<int *>( calloc( new_max_graphics, sizeof( int ) ) );
        int * new_gfx_ptrs_pos_to_id = static_cast<int *>( calloc( new_max_graphics, sizeof( int ) ) );
        int * new_state_for_gfx = static_cast<int *>( calloc( new_max_graphics, sizeof( int ) ) );
        int * new_layer_for_gfx = static_cast<int *>( calloc( new_max_graphics, sizeof( int ) ) );
        if
        (
            !new_graphics ||
            !new_gfx_ptrs_id_to_pos ||
            !new_gfx_ptrs_pos_to_id ||
            !new_state_for_gfx ||
            !new_layer_for_gfx
        )
        {
            BSL::log( "AddGraphic Error: ¡Not ’nough memory for graphics!" );
            return false;
        }

        memcpy( new_graphics, graphics, max_graphics * sizeof( RawGraphic ) );
        memcpy( new_gfx_ptrs_id_to_pos, gfx_ptrs_id_to_pos, max_graphics * sizeof( int ) );
        memcpy( new_gfx_ptrs_pos_to_id, gfx_ptrs_pos_to_id, max_graphics * sizeof( int ) );
        memcpy( new_state_for_gfx, state_for_gfx, max_graphics * sizeof( int ) );
        memcpy( new_layer_for_gfx, layer_for_gfx, max_graphics * sizeof( int ) );

        free( graphics );
        free( gfx_ptrs_id_to_pos );
        free( gfx_ptrs_pos_to_id );
        free( state_for_gfx );
        free( layer_for_gfx );

        graphics = new_graphics;
        gfx_ptrs_id_to_pos = new_gfx_ptrs_id_to_pos;
        gfx_ptrs_pos_to_id = new_gfx_ptrs_pos_to_id;
        state_for_gfx = new_state_for_gfx;
        layer_for_gfx = new_layer_for_gfx;

        // Initialize these to null values ( since 0 is a valid value, we use -1 ).
        for ( int i = max_graphics; i < new_max_graphics; ++i )
        {
            gfx_ptrs_id_to_pos[ i ] = gfx_ptrs_pos_to_id[ i ] = state_for_gfx[ i ] = layer_for_gfx[ i ] = -1;
        }

        max_graphics = new_max_graphics;
        return true;
    };

    static int GetCharacterSize( const char * s )
    {
        const int code = ( int )( *s );
        return ( code & ( 1 << 7 ) )
            ? ( code & ( 1 << 5 ) )
                ? ( code & ( 1 << 4 ) )
                    ? 4
                    : 3
                : 2
            : 1;
    };
}