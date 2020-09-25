#include "block_layer.hpp"
#include "camera.hpp"
#include "level_state.hpp"

BlockLayer::BlockLayer( int position, RenderType render_type )
:
    Renderable(),
    position_ ( position ),
    render_type_ ( render_type ),
    blocks_ ( {} )
{};

BlockLayer::~BlockLayer() {};

BlockLayer::BlockLayer( BlockLayer&& m )
:
    position_ ( m.position_ ),
    render_type_ ( m.render_type_ ),
    blocks_ ( m.blocks_ )
{};

void BlockLayer::render( const LevelState& level_state ) const
{
    const Camera& camera = level_state.camera();
    switch ( render_type_ )
    {
        case ( RenderType::NORMAL ):
        {
            for ( const auto& block : blocks_ )
            {
                block.render( camera, true );
            }
        }
        break;

        case ( RenderType::OFFSCREEN ):
        {
            // Only render blocks in-camera.
            const int first_x = Unit::PixelsToBlocks( camera.x() ) - 2; // Block x just left o' camera.
            const int first_y = Unit::PixelsToBlocks( camera.y() ) - 2; // Block y just 'bove camera.
            const int last_x  = Unit::PixelsToBlocks( camera.screenRight() ) + 2; // Block x just right o' screen.
            const int last_y  = Unit::PixelsToBlocks( camera.screenBottom() ) + 2; // Block y just below screen.
            for ( int y = first_y; y < last_y; ++y )
            {
                for ( int x = first_x; x < last_x; ++x )
                {
                    int n = y * level_state.currentMap().widthBlocks() + x;
                    if ( n < blocks_.size() )
                    {
                        const Block& block = blocks_[ n ];
                        block.render( camera, true );
                    }
                }
            }
        }
        break;
    }
};