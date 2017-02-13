


// Name
//===================================
//
// BlockType
//


// DEPENDENCIES
//===================================

    #include "block.h"
    #include "block_component.h"
    #include "block_condition.h"
    #include "block_type.h"
    #include "collision.h"
    #include <iostream>
    #include "sprite_graphics.h"


// STATIC PROPERTIES
//===================================

// METHODS
//===================================

    BlockType::BlockType
    (
        SpriteGraphics* g1,
        SpriteGraphics* g2,
        SpriteGraphics* g3,
        SpriteGraphics* g4,
        std::vector<BlockComponent*> components,
        std::vector<std::vector<BlockCondition*>> conditions
    )
    :
        graphics_
        (
            {
                std::unique_ptr<SpriteGraphics>( g1 ),
                std::unique_ptr<SpriteGraphics>( g2 ),
                std::unique_ptr<SpriteGraphics>( g3 ),
                std::unique_ptr<SpriteGraphics>( g4 )
            }
        )
    {
        for ( int i = 0; i < components.size(); ++i )
        {
            components_.push_back( std::unique_ptr<BlockComponent> ( components.at( i ) ) );
        }

        for ( int j = 0; j < conditions.size(); ++j )
        {
            conditions_.push_back( {} );

            for ( int k = 0; k < conditions.at( j ).size(); ++k )
            {
                conditions_.at( j ).push_back( std::unique_ptr<BlockCondition> ( conditions.at( j ).at( k ) ) );
            }
        }
    };

    BlockType::~BlockType() {};

    void BlockType::interact( Collision& collision, Sprite& sprite, Block& block, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera )
    {
        for ( int i = 0; i < components_.size(); ++i )
        {
            bool can_interact = true;

            if ( i < conditions_.size() )
            {
                for ( int j = 0; j < conditions_[ i ].size(); ++j )
                {
                    if ( conditions_.at( i ).at( j ) )
                    {
                        if ( !conditions_.at( i ).at( j )->condition( collision, sprite, block, events ) )
                        {
                            can_interact = false;
                            break;
                        }
                    }
                }
            }

            if ( can_interact )
                if ( components_.at( i ) )
                    components_.at( i )->interact( collision, sprite, block, *this, level, events, inventory, camera );
        }
    };

    void BlockType::render( Graphics& graphics, Camera& camera, Block& block, bool priority )
    {
        sdl2::SDLRect r = { 0, 0, 8, 8 };

        for ( int i = 0; i < NUM_O_MINI_BLOCKS; ++i )
        {
            int x = ( ( i % 2 ) * 8 );
            int y = ( floor( i / 2 ) * 8 );

            r.x = block.xPixels() + x;
            r.y = block.yPixels() + y;

            if ( graphics_[i] )
                graphics_[i]->render( graphics, r, &camera, priority );
        }
    };

    void BlockType::update( EventSystem& events )
    {
        for ( int j = 0; j < components_.size(); ++j )
        {
            if ( components_.at( j ) )
            {
                components_.at( j )->update( events, *this );
            }
        }

        for ( int i = 0; i < NUM_O_MINI_BLOCKS; ++i )
        {
            if ( graphics_[ i ].get() != nullptr )
            {
                graphics_[ i ]->update();
            }
        }
    };
