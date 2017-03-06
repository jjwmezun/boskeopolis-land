


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
        std::unique_ptr<SpriteGraphics> graphics,
        std::vector<std::unique_ptr<BlockComponent>> components,
        std::vector<std::vector<std::unique_ptr<BlockCondition>>> conditions
    )
    :
        graphics_   ( std::move( graphics ) ),
		components_ ( std::move( components ) ),
		conditions_ ( std::move( conditions ) )
	{};

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
		if ( graphics_ )
		{
			graphics_->render( graphics, Unit::SubPixelsToPixels( block.hitBox() ), &camera, priority );
		}
    };

    void BlockType::update( EventSystem& events )
    {
        for ( auto& c : components_ )
        {
			c->update( events, *this );
        }

		if ( graphics_ )
		{
			graphics_->update();
		}

    };

	bool BlockType::hasComponentType( BlockComponent::Type type ) const
	{
		for ( auto& t : components_ )
		{
			// True if a'least 1 component has type.
			if ( t->type() == type ) return true;
		}
		
		// If all don't, false.
		return false;
	};