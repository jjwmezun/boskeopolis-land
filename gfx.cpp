#include <cglm/cglm.h>
#include <cglm/call.h>
#include "config.hpp"
#include <cstdio>
#include "gfx.hpp"
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include "layer.hpp"
#include "log.hpp"
#include <string>
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

    struct GraphicCharTemplate
    {
        GraphicCharType type;
        uint_fast8_t x;
        uint_fast8_t y;
        uint_fast8_t w;
        uint_fast8_t h;
    };

    struct GraphicChar
    {
        uint_fast8_t x;
        uint_fast8_t y;
        uint_fast8_t w;
        uint_fast8_t h;
        uint_fast8_t srcx;
        uint_fast8_t srcy;
    };

    struct Rect
    {
        int x;
        int y;
        unsigned int w;
        unsigned int h;
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
        SPRITE_RAW
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
                Rect coords;
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
    static Texture charset_texture;
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

    static void FramebufferSizeCallback( GLFWwindow * window, int width, int height );
    static unsigned int GenerateShaderProgram( const Shader * shaders, int shadersnum );
    static void setMVP( unsigned int shader );
    static RawGraphic & getGraphic( unsigned int id );
    static unsigned int addGraphic
    (
        unsigned int state,
        BSL::Layer layer,
        RawGraphic graphic
    );
    static void changeGraphicLayer( unsigned int id, unsigned int layer );
    static unsigned int getStateLayerIndex( unsigned int layer );
    static bool growGraphics();

    void Graphic::setLayer( BSL::Layer layer )
    {
        getGraphic( id_ ).layer = layer;
        changeGraphicLayer( id_, static_cast<unsigned int> ( layer ) );
    };

    void Sprite::setX( int v )
    {
        getGraphic( id_ ).data.sprite.x = v;
    };

    void Sprite::setY( int v )
    {
        getGraphic( id_ ).data.sprite.y = v;
    };

    void Sprite::setSrcX( int v )
    {
        getGraphic( id_ ).data.sprite.srcx = v;
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
        std::string palette_filename = std::string( "assets/palettes/city-sunrise-palette.png" );
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
                "#version 330 core\nout vec4 final_color;\n\nin vec2 out_position;\n\nin vec2 texture_coords;\n\nuniform sampler2D texture_data;\nuniform sampler2D palette_data;\nuniform float palette_no;\nvoid main()\n{\n    float color = ( texture( texture_data, texture_coords ).r * 256.0 - 1.0 ) / 256.0;\n    final_color = texture( palette_data, vec2( color, palette_no ) );\n}"
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

        return 0;
    };

    void close()
    {
        glfwTerminate();
    };

    void update( float dt )
    {
        const float fps = 60.0 / dt;
        memset( gl_texture_data, 0xFF, BSL::WINDOW_WIDTH_PIXELS * BSL::WINDOW_HEIGHT_PIXELS );

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

                    for ( unsigned int j = 0; j < h; ++j )
                    {
                        const float percent = ( float )( j + 1 ) / h;
                        const float diff = colordiff * percent;
                        const unsigned char color = graphics[ i ].data.rect.color1 + diff;
                        const unsigned int k = y + j;
                        memset( &gl_texture_data[ BSL::WINDOW_WIDTH_PIXELS * k + x ], color, w );
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
                    for ( unsigned int j = 0; j < t.char_count; ++j )
                    {
                        const auto & chr = t.chars[ j ];
                        const int relativex = graphics[ i ].abs ? chr.x : chr.x - camera.x;
                        const int relativey = graphics[ i ].abs ? chr.y : chr.y - camera.y;
                        const int xsub = relativex < 0 ? relativex : 0;
                        const int ysub = relativey < 0 ? relativey : 0;
                        const unsigned int x = relativex < 0 ? 0 : relativex;
                        const unsigned int y = relativey < 0 ? 0 : relativey;
                        int w = std::min( static_cast<unsigned int>( chr.w ), charset_texture.w ) + std::min( 0, relativex );
                        int h = std::min( static_cast<unsigned int>( chr.h ), charset_texture.h ) + std::min( 0, relativey );
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
                            const unsigned int sy = y + yi;
                            for ( unsigned int xi = 0; xi < w; ++xi )
                            {
                                const unsigned char v = charset_texture.data
                                [
                                    ( chr.srcy - ysub + yi ) * charset_texture.w + chr.srcx - xsub + xi
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
                            gl_texture_data[ BSL::WINDOW_WIDTH_PIXELS * sy + sx ] = v;
                        }
                    }
                }
                break;
            }
        }
        unsigned char v = fps < 60.0f ? 0x80 : 0x00;
        for ( unsigned int i = 0; i < 8; ++i )
        {
            memset( &gl_texture_data[ BSL::WINDOW_WIDTH_PIXELS * ( 8 + i ) + 8 ], v, ( unsigned int )( fps ) );
        }

        glClearColor( 0.976f, 0.847f, 0.545f, 1.0f );
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

    int addGraphicRect
    (
        int x,
        int y,
        unsigned int w,
        unsigned int h,
        unsigned char color,
        BSL::ArgList args
    )
    {
        return addGraphicRectGradient
        (
            x,
            y,
            w,
            h,
            color,
            color,
            args
        );
    };

    int addGraphicRectGradient
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
        g.data.rect.coords.x = x;
        g.data.rect.coords.y = y;
        g.data.rect.coords.w = w;
        g.data.rect.coords.h = h;
        g.data.rect.color1 = color1;
        g.data.rect.color2 = color2;
        BSL::Layer layer = BSL::GetArg( "layer", args, BSL::Layer::BG_1 );
        return addGraphic( state, layer, g );
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
        BSL::Layer layer = BSL::GetArg( "layer", args, BSL::Layer::SPRITES_1 );
        return { addGraphic( state, layer, g ) };
    };

    Tilemap addGraphicTilemap
    (
        unsigned int texture,
        Tile * tiles,
        unsigned int w,
        unsigned int h,
        int x,
        int y,
        BSL::ArgList args
    )
    {
        RawGraphic g;
        g.type = GraphicType::TILEMAP;
        g.abs = BSL::GetArg( "abs", args, false );
        g.opacity = 1.0f;
        g.data.tilemap.texture = texture;
        g.data.tilemap.tiles = static_cast<Tile *> ( malloc( w * h * sizeof( Tile ) ) );
        memcpy( g.data.tilemap.tiles, tiles, w * h * sizeof( Tile ) );
        g.data.tilemap.w = w;
        g.data.tilemap.h = h;
        g.data.tilemap.x = x;
        g.data.tilemap.y = y;
        BSL::Layer layer = BSL::GetArg( "layer", args, BSL::Layer::BLOCKS_1 );
        return { addGraphic( state, layer, g ) };
    };

    void setPalette( unsigned char p )
    {
        palette = static_cast<float> ( static_cast<double> ( p ) / 256.0 );
    };

    int addGraphicText
    (
        const char * text
    )
    {
    /*
        graphics[ graphics_count ].type = GraphicType::TEXT;
        graphics[ graphics_count ].abs = 1;
        graphics[ graphics_count ].opacity = 1.0f;
        graphics[ graphics_count ].data.text.char_count = 10;
        graphics[ graphics_count ].data.text.chars = static_cast<GraphicChar *> ( malloc
        (
            graphics[ graphics_count ].data.text.char_count * sizeof( GraphicChar )
        ));

        for ( unsigned int i = 0; i < graphics[ graphics_count ].data.text.char_count; ++i )
        {
            graphics[ graphics_count ].data.text.chars[ i ] =
            {
                i * 8,
                0,
                8,
                8,
                i * 8,
                0
            };
        }*/

        unsigned int dw = 80;
        unsigned int dh = 8;
        RawGraphic g;
        g.type = GraphicType::SPRITE_RAW;
        g.abs = 1;
        g.opacity = 1.0f;
        auto & rawsprite = g.data.rawsprite;
        rawsprite.data = ( unsigned char * )( calloc( dw * dh, 1 ) );
        rawsprite.dataw = dw;
        rawsprite.datah = dh;
        rawsprite.srcx = 0;
        rawsprite.srcy = 0;
        rawsprite.w = dw;
        rawsprite.h = dh;
        rawsprite.x = 0;
        rawsprite.y = 0;
        unsigned int count = 10;
        GraphicChar chars[ count ];
        for ( unsigned int i = 0; i < count; ++i )
        {
            chars[ i ] =
            {
                static_cast<uint_fast8_t> ( i * 8 ),
                0,
                8,
                8,
                static_cast<uint_fast8_t> ( i * 8 ),
                0
            };
        }
        for ( unsigned int j = 0; j < count; ++j )
        {
            const auto & chr = chars[ j ];
            for ( unsigned int yi = 0; yi < chr.h; ++yi )
            {
                const unsigned int sy = chr.y + yi;
                for ( unsigned int xi = 0; xi < chr.w; ++xi )
                {
                    const unsigned char v = charset_texture.data
                    [
                        ( chr.srcy + yi ) * charset_texture.w + chr.srcx + xi
                    ];

                    // Skip transparent.
                    if ( v == 0x00 )
                    {
                        continue;
                    }

                    const unsigned int sx = chr.x + xi;
                    rawsprite.data[ dw * sy + sx ] = v;
                }
            }
        }

        BSL::Layer layer = BSL::Layer::BG_1;
        return addGraphic( state, layer, g );
    };

    int addGraphicSpriteRaw
    (
        uint_fast8_t abs,
        unsigned char * data,
        unsigned int dataw,
        unsigned int datah,
        unsigned int srcx,
        unsigned int srcy,
        unsigned int w,
        unsigned int h,
        int x,
        int y
    )
    {
        RawGraphic g;
        g.type = GraphicType::SPRITE_RAW;
        g.abs = abs;
        g.data.rawsprite.data = ( unsigned char * )( malloc( dataw * datah ) );
        memcpy( g.data.rawsprite.data, data, dataw * datah );
        g.data.rawsprite.dataw = dataw;
        g.data.rawsprite.datah = datah;
        g.data.rawsprite.srcx = srcx;
        g.data.rawsprite.srcy = srcy;
        g.data.rawsprite.w = w;
        g.data.rawsprite.h = h;
        g.data.rawsprite.x = x;
        g.data.rawsprite.y = y;
        BSL::Layer layer = BSL::Layer::BG_1;
        return addGraphic( state, layer, g );
    };

    int loadFileAsTexture( const char * filename )
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
            return -1;
        }

        return texture_count++;
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

    static unsigned int addGraphic
    (
        unsigned int state,
        BSL::Layer layer,
        RawGraphic graphic
    )
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
        unsigned int layern = static_cast<unsigned int> ( layer );
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
}