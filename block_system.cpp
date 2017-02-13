
// Name
//===================================
//
// BlockSystem
//


// DEPENDENCIES
//===================================

    #include "animated_graphics.h"
    #include "block_component_bouncy.h"
    #include "block_component_change_block.h"
    #include "block_component_change_direction.h"
    #include "block_component_change_palette.h"
    #include "block_component_climbable.h"
    #include "block_component_conveyor.h"
    #include "block_component_diamond.h"
    #include "block_component_goal.h"
    #include "block_component_high_slope_left.h"
    #include "block_component_high_slope_right.h"
    #include "block_component_heal.h"
    #include "block_component_hurt.h"
    #include "block_component_key.h"
    #include "block_component_lose.h"
    #include "block_component_low_slope_left.h"
    #include "block_component_low_slope_right.h"
    #include "block_component_message.h"
    #include "block_component_money.h"
    #include "block_component_single_use.h"
    #include "block_component_solid.h"
    #include "block_component_swimmable.h"
    #include "block_component_switch.h"
    #include "block_component_switch_flip.h"
    #include "block_component_warp.h"
    #include "block_condition_collide_any.h"
    #include "block_condition_collide_bottom.h"
    #include "block_condition_collide_left.h"
    #include "block_condition_collide_right.h"
    #include "block_condition_collide_top.h"
    #include "block_condition_enemy.h"
    #include "block_condition_hero.h"
    #include "block_condition_key.h"
    #include "block_condition_not_ducking.h"
    #include "block_condition_rival.h"
    #include "block_system.h"
    #include "block_type.h"
    #include "camera.h"
    #include "graphics.h"
    #include <iostream>
    #include "map.h"
    #include "rotating_graphics.h"
    #include "sprite.h"
    #include "sprite_graphics.h"
    #include <utility>


// STATIC VARIABLES
//===================================


// METHODS
//===================================

    BlockSystem::~BlockSystem()
    {
        clearBlockTypes();
    };

    void BlockSystem::update( EventSystem& events )
    {
        for ( int i = 0; i < block_types_.size(); ++i )
        {
            if ( block_types_.at( i ) != nullptr )
            {
                block_types_.at( i )->update( events );
            }
        }
    };

    void BlockSystem::render( Graphics& graphics, Camera& camera, bool priority )
    {
        for ( auto& b : blocks_ )
        {
			if ( camera.onscreen( b.hitBox() ) )
			{
            	b.render( graphics, camera, priority );
			}
        }
    };

    void BlockSystem::interact( Sprite& sprite, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera )
    {
        for ( auto& b : blocks_ )
        {
            b.interact( sprite, level, events, inventory, camera );
        }
    };

    void BlockSystem::blocksFromMap( Map& lvmap, Camera& camera )
    {
		if ( !lvmap.blocksWorkOffscreen() )
		{
			if ( camera.changed() || lvmap.changed() )
			{
				blocks_.clear();

				const int first_x = floor( camera.x() / Unit::PIXELS_PER_BLOCK ) - CAMERA_PADDING;
				const int first_y = floor( camera.y() / Unit::PIXELS_PER_BLOCK ) - CAMERA_PADDING;
				const int last_x = ceil( camera.right() / Unit::PIXELS_PER_BLOCK ) + CAMERA_PADDING;
				const int last_y = ceil( camera.bottom() / Unit::PIXELS_PER_BLOCK ) + CAMERA_PADDING;

				for ( int y = first_y; y < last_y; ++y )
				{
					const int yp = Unit::BlocksToPixels( y );

					for ( int x = first_x; x < last_x; ++x )
					{
						const int xp = Unit::BlocksToPixels( x );

						const int i = lvmap.indexFromXAndY( x, y );
						const int type = lvmap.block( i ) - 1;

						addBlock( xp, yp, i, type );
					}
				}

			}
		}
		else
		{
			if ( blocks_.empty() )
			{
				for ( int i = 0; i < lvmap.blocksSize(); ++i )
				{
					const int type = lvmap.block( i ) - 1;

					if ( type > -1 )
					{
						const int x = Unit::BlocksToPixels( lvmap.mapX( i ) );
						const int y = Unit::BlocksToPixels( lvmap.mapY( i ) );

						addBlock( x, y, i, type );

					}
				}
			}
			else
			{
				for ( int i = 0; i < blocks_.size(); ++i )
				{
					if ( blocks_.at( i ).destroyed_ ) blocks_.erase( blocks_.begin() + i );
				}
			}
		}
    };

	void BlockSystem::addBlock( int x, int y, int i, int type )
	{
		if ( type > -1 )
		{
			blocks_.push_back( { x, y, blockType( type ), i, type } );
		}
	}

    BlockType* BlockSystem::blockType( int n ) const
    {
        if ( -1 < n && n < block_types_.size() )
        {
            return block_types_.at( n );
        }
        else
        {
            return nullptr;
        }
    };

    void BlockSystem::clearBlockTypes()
    {
        for ( int i = 0; i < block_types_.size(); ++i )
        {
            delete block_types_.at( i );
            block_types_.at( i ) = nullptr;
        }
    };

    bool BlockSystem::blocksInTheWay( int left, int right, int top, int bottom, std::vector<int> type_ids ) const
    {
        for ( auto& b : blocks_ )
        {
            if
            (
                b.rightSubPixels() > left &&
                b.leftSubPixels() < right &&
                b.topSubPixels() < bottom &&
                b.bottomSubPixels() > top
            )
            {
                if ( !type_ids.empty() )
                {
                    for ( int j = 0; j < type_ids.size(); ++j )
                    {
                        if ( b.typeID() == type_ids.at( j ) )
                        {
                            return true;
                        }
                    }
                }
				// If type_ids empty, treat as "wildcard" wherein any blocks apply.
                else
                {
                    return true;
                }
            }
        }

        return false;
    };



	// Rest o' document massive mess o' block-generation code.
    BlockSystem::BlockSystem()
    {
        block_types_ =
        {
            new BlockType
            (
                new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 12*8, 0 ),
                new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 13*8, 0 ),
                new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 12*8, 8 ),
                new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 13*8, 8 ),
                { new BlockComponentMoney( 100 ), new BlockComponentSingleUse(), new BlockComponentSingleUse() }, { { new BlockConditionCollideAny(), new BlockConditionHero() }, { new BlockConditionCollideAny(), new BlockConditionHero() }, { new BlockConditionCollideAny(), new BlockConditionRival() } }
            ),
            new BlockType
            (
                new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 120, 32 ),
                new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 128, 32 ),
                new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 136, 32 ),
                new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 144, 32 ),
                { new BlockComponentMoney( 250 ), new BlockComponentSingleUse() }, { { new BlockConditionCollideAny(), new BlockConditionHero() }, { new BlockConditionCollideAny(), new BlockConditionHero() } }
            ),
            new BlockType
            (
                new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 152, 32 ),
                new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 160, 32 ),
                new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 168, 32 ),
                new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 176, 32 ),
                { new BlockComponentMoney( 500 ), new BlockComponentSingleUse() }, { { new BlockConditionCollideAny(), new BlockConditionHero() }, { new BlockConditionCollideAny(), new BlockConditionHero() } }
            ),
            new BlockType
            (
                new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 184, 32 ),
                new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 192, 32 ),
                new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 200, 32 ),
                new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 208, 32 ),
                { new BlockComponentMessage(), new BlockComponentSolid() }, { { new BlockConditionCollideTop(), new BlockConditionHero() } }
            ),
            new BlockType
            (
                new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 216, 32 ),
                new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 224, 32 ),
                new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 232, 32 ),
                new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 240, 32 ),
                { new BlockComponentHeal(), new BlockComponentSingleUse() }, { { new BlockConditionCollideAny(), new BlockConditionHero() }, { new BlockConditionCollideAny(), new BlockConditionHero() } }
            ),
            nullptr,
            nullptr,
            nullptr,
            nullptr,
            nullptr,
            nullptr,
            nullptr,
            nullptr,
            nullptr,
            nullptr,
            nullptr,
        };

        block_types_.push_back( new BlockType
        (
            nullptr,
            nullptr,
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 128, 24, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 128, 24, false, false, 0, true ),
            {}//{}//BlockType::BlockAction::__NULL
        ));
        block_types_.push_back( new BlockType
        (
            nullptr,
            nullptr,
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 128, 24, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 168, 24, false, false, 0, true ),
            {}//{}//BlockType::BlockAction::__NULL
        ));
        block_types_.push_back( new BlockType
        (
            nullptr,
            nullptr,
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 168, 24, true, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 128, 24, false, false, 0, true ),
            {}//{}//BlockType::BlockAction::__NULL
        ));

        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 200, 24 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 208, 24 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 200, 16 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 208, 16 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 216, 24 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 232, 24 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 248, 24 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 0, 32 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 232, 24 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 240, 24 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 8, 32 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 16, 32 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 64, 88 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY,  8, 40 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 72, 88 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 56, 40 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY,  0, 40 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 80, 88 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 48, 40 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 88, 88 ),
            { new BlockComponentSolid() }
        ));

        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 112, 40, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 120, 40, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 112, 40, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 120, 40, false, false, 0, true ),
            {}
        ));


        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 112, 40, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 120, 40, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 112, 40, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 120, 40, false, false, 0, true ),
            { new BlockComponentSolid() }
        ));



        block_types_.push_back( new BlockType
        (
            nullptr,
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 128, 40, false, false, 0, true ),
            nullptr,
            nullptr,
            {}
        ));

        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 136, 40, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 144, 40, false, false, 0, true ),
            nullptr,
            nullptr,
            {}
        ));


        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 144, 40, true, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 136, 40, true, false, 0, true ),
            nullptr,
            nullptr,
            {}
        ));

        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 128, 40, true, false, 0, true ),
            nullptr,
            nullptr,
            nullptr,
            {}
        ));

        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 152, 40, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 184, 40, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 168, 40, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 176, 40, false, false, 0, true ),
            {}
        ));

        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 192, 40, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 160, 40, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 168, 40, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 176, 40, false, false, 0, true ),
            {}
        ));


        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 112, 24, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 120, 24, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 136, 24, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 136, 24, false, false, 0, true ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 112, 24, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 160, 24, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 136, 24, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 152, 24, false, false, 0, true ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 160, 24, true,  false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 120, 24, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 144, 24, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 136, 24, false, false, 0, true ),
            { new BlockComponentSolid() }
        ));

        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 24, 32 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 32, 32 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 72, 32 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 80, 32 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 40, 32 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 48, 32 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 88, 32 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 96, 32 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 56, 32 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 64, 32 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 104, 32 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 112, 32 ),
            { new BlockComponentSolid() }
        ));

        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 80, 40 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 88, 40 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 96, 40 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 104, 40 ),
            { }
        ));

        block_types_.push_back( new BlockType
        (
            nullptr,
            nullptr,
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 32, 40 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 40, 40 ),
            { new BlockComponentLowSlopeLeft() }
        ));

        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 32, 40 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 40, 40 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 16, 40 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 24, 40 ),
            { new BlockComponentHighSlopeLeft() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 40, 40, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 32, 40, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 24, 40, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 16, 40, true ),
            { new BlockComponentHighSlopeRight() }
        ));

        block_types_.push_back( new BlockType
        (
            nullptr,
            nullptr,
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 40, 40, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 32, 40, true ),
            { new BlockComponentLowSlopeRight() }
        ));

        block_types_.push_back( nullptr );
        block_types_.push_back( nullptr );


        block_types_.push_back( new BlockType
        (
            nullptr,
            nullptr,
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 216, 40, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 224, 40, false, false, 0, true ),
            {}
        ));

        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 136, 24, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 136, 24, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 176, 112, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 176, 112, false, false, 0, true ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 136, 24, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 152, 24, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 176, 112, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 192, 112, false, false, 0, true ),
            { new BlockComponentSolid() }
        ));

        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 136, 24, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 136, 24, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 136, 24, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 136, 24, false, false, 0, true ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 136, 24, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 152, 24, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 136, 24, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 152, 24, false, false, 0, true ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 144, 24, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 136, 24, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 144, 24, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 136, 24, false, false, 0, true ),
            { new BlockComponentSolid() }
        ));

        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 184, 16, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 192, 16, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 184, 24, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 192, 24, false, false, 0, true ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 200, 24 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 208, 24 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 72, 32, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 80, 32, false, false, 0, true ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 232, 24 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 240, 24 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 104, 32, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 112, 32, false, false, 0, true ),
            { new BlockComponentSolid() }
        ));

        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 0, 40 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 0, 40 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 48, 40 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 48, 40 ),
            { new BlockComponentSolid() }, { { new BlockConditionCollideBottom() } }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 16, 40 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 16, 40 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 64, 40 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 72, 40 ),
            {}
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 64, 40 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 72, 40 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 72, 40 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 72, 40 ),
            {}
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 72, 40, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 64, 40, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 72, 40, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 72, 40, true ),
            {}
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 16, 40, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 16, 40, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 72, 40, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 64, 40, true ),
            { }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 72, 40 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 72, 40 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 72, 40 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 72, 40 ),
            { new BlockComponentSolid() }
        ));

        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 216, 40 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 224, 40 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 200, 40 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 208, 40 ),
            { new BlockComponentLowSlopeLeft() }
        ));

        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 200, 40, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 208, 40, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 232, 40, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 136, 24, false, false, 0, true ),
            { new BlockComponentHighSlopeLeft() }
        ));

        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 232, 40, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 136, 24, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 136, 24, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 136, 24, false, false, 0, true ),
            { new BlockComponentSolid() }
        ));

        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 144, 24 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 136, 24 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 184, 112 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 176, 112 ),
            { new BlockComponentSolid() }
        ));

        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 0, 0 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 8, 0 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 16, 0 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 16, 0 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 16, 8 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 16, 8 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 16, 8 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 16, 8 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 24, 0 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 24, 0 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 24, 0 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 24, 0 ),
            {}//{}//BlockType::BlockAction::__NULL
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 32, 0 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 40, 0 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 32, 8 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 40, 8 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 64, 8 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 72, 8 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 64, 8 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 72, 8 ),
            { new BlockComponentClimbable() }, { { new BlockConditionCollideAny() } }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 80, 0 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 88, 0 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 80, 8 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 88, 8 ),
            {}//{}//BlockType::BlockAction::__NULL
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 14*8, 0 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 15*8, 0 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 16*8, 0 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 17*8, 0 ),
            {}//{}//BlockType::BlockAction::__NULL
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 22*8, 0 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 23*8, 0 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 23*8, 0 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 22*8, 0 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 23*8, 8, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 24*8, 8, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 16, 0, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 16, 0, false, false, 0, true ),
            {}//{}//BlockType::BlockAction::__NULL
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 25*8, 8, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 26*8, 8, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 16, 0, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 16, 0, false, false, 0, true ),
            { new BlockComponentClimbable() }, { { new BlockConditionCollideAny(), new BlockConditionHero() } }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 22*8, 0 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 23*8, 0 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 24*8, 0 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 25*8, 0 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 29*8, 8 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 30*8, 8 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 29*8, 16 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 30*8, 16 ),
            { new BlockComponentHurt(), new BlockComponentSolid() }, { { new BlockConditionHero(), new BlockConditionCollideBottom() } }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 31*8, 0 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 31*8, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 31*8, 8 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 31*8, 8, true ),
            {}//{}//BlockType::BlockAction::__NULL
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 80, 16 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 88, 16 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 80, 24 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 88, 24 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 7*8, 24, 0, 0, 90 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 7*8, 16, 0, 0, 90 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 64, 24, 0, 0, 90 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 64, 16, 0, 0, 90 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 3*8, 8 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 3*8, 8 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 16, 16 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 16, 16 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 9*8, 0 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 9*8, 0 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 24, 0 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 24, 0 ),
            { new BlockComponentSolid() }, { { new BlockConditionCollideBottom() } }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 24, 0 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 24, 0 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 64, 0 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 64, 0 ),
            {}//{}//BlockType::BlockAction::__NULL
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 18*8, 8 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 18*8, 8 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 18*8, 8 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 18*8, 8 ),
            {}//{}//BlockType::BlockAction::__NULL
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 19*8, 8 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 19*8, 8 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 18*8, 8 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 18*8, 8 ),
            { new BlockComponentSolid() }, { { new BlockConditionCollideBottom() } }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 18*8, 8 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 18*8, 8 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 20*8, 8 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 20*8, 8 ),
            {}//{}//BlockType::BlockAction::__NULL
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 21*8, 8 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 22*8, 8 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 21*8, 16 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 22*8, 16 ),
            {}//{}//BlockType::BlockAction::__NULL
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 18*8, 0 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 19*8, 0 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 20*8, 0 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 21*8, 0 ),
            {}//{}//BlockType::BlockAction::__NULL
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 6*8, 0 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 7*8, 0 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 23*8, 0 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 22*8, 0 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 27*8, 0 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 28*8, 0 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 2*8, 0 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 2*8, 0 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 29*8, 0 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 30*8, 0 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 2*8, 0 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 2*8, 0 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 27*8, 8 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 28*8, 8 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 27*8, 16 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 28*8, 16 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 112, 80 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 120, 80 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 112, 88 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 120, 88 ),
            { new BlockComponentGoal(), new BlockComponentLose() }, { { new BlockConditionCollideAny(), new BlockConditionHero() }, { new BlockConditionCollideAny(), new BlockConditionRival() } }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 31*8, 16 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 31*8, 16, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 31*8, 16 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 31*8, 16, true ),
            {}//{}//BlockType::BlockAction::__NULL
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 40, 24 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 40, 24 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 40, 24 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 40, 24 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 64, 24, 0, 0, 90 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 64, 16, 0, 0, 90 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 64, 24, 0, 0, 90 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 64, 16, 0, 0, 90 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 16, 16 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 16, 16 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 16, 16 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 16, 16 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 16, 8, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 16, 8, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 16, 8, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 16, 8, false, false, 0, true ),
            { new BlockComponentWarp() }, { { new BlockConditionCollideAny(), new BlockConditionHero() } }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 0, 48 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 8, 48 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 16, 48 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 16, 48 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 8, 48, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 8, 48 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 16, 48 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 16, 48 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 8, 48, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 0, 48, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 16, 48 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 16, 48 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 16, 48 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 16, 48 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 16, 48 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 16, 48 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 24, 16 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 16, 16 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 24, 24 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 32, 24 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 16, 16 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 6*8, 16 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 16, 24 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 6*8, 24 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 32, 16 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 16, 16 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 32, 16 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 16, 16 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 16, 16 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 40, 16 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 16, 16 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 40, 16 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( nullptr );
        block_types_.push_back( nullptr );
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 7*8, 16 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 64, 16 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 7*8, 24 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 64, 24 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 64, 16 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 64, 16 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 64, 24 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 64, 24 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 64, 16 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 72, 16 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 64, 24 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 72, 24 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 64, 24, 0, 0, 90 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 64, 16, 0, 0, 90 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 72, 24, 0, 0, 90 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 72, 16, 0, 0, 90 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 16, 16 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 16, 16 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 16, 24 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 32, 24 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 64, 8 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 72, 8 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 64, 8 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 72, 8 ),
            { new BlockComponentWarp(), new BlockComponentClimbable() }, { { new BlockConditionCollideAny(), new BlockConditionHero() }, { new BlockConditionCollideAny() } }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 24, 48 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 32, 48 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 56, 48 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 64, 48 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 40, 48 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 48, 48 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 72, 48 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 80, 48 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 152, 48 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 160, 48 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 184, 48 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 192, 48 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 168, 48 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 176, 48 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 200, 48 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 208, 48 ),
            { new BlockComponentSolid() }
        ));

        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 0, 56 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 8, 56 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 32, 56 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 40, 56 ),
            { new BlockComponentGoal() }, { { new BlockConditionCollideAny(), new BlockConditionKey( true ) } }
        ));

        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 16, 56 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 24, 56 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 48, 56 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 56, 56 ),
            { new BlockComponentGoal() }, { { new BlockConditionCollideAny(), new BlockConditionKey( true ) } }
        ));

        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 128, 56 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 128, 56 ),
            nullptr,
            nullptr,
            { new BlockComponentSolid() }, { { new BlockConditionNotDucking(), new BlockConditionCollideBottom() } }
        ));

        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 136, 56 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 144, 56 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 152, 56 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 160, 56 ),
            {}
        ));

        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 168, 56 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 176, 56 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 200, 56 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 208, 56 ),
            { new BlockComponentDiamond() }
        ));

        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 184, 56 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 192, 56 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 216, 56 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 224, 56 ),
            { new BlockComponentDiamond() }
        ));

        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 32, 64 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 32, 48 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 40, 64 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 64, 48 ),
            { new BlockComponentSolid() }
        ));

        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 40, 48 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 64, 64 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 72, 48 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 72, 64 ),
            { new BlockComponentSolid() }
        ));

        block_types_.push_back( new BlockType
        (
            nullptr,
            nullptr,
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 96, 64 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 104, 64 ),
            { new BlockComponentLowSlopeLeft() }
        ));

        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 112, 64 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 120, 64 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 128, 64 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 80, 48 ),
            { new BlockComponentHighSlopeLeft() }
        ));

        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 144, 64 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 152, 64 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 160, 64 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 128, 48 ),
            { }
        ));

        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 240, 40 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 248, 40 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 248, 32 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 248, 48 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 88, 48 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 96, 48 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 120, 48 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 128, 48 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 104, 48 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 112, 48 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 136, 48 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 144, 48 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            nullptr,
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 216, 48 ),
            nullptr,
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 232, 48 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 224, 48 ),
            nullptr,
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 240, 48 ),
            nullptr,
            { new BlockComponentSolid() }
        ));


        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 64, 56 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 72, 56 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 96, 56 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 104, 56 ),
            { new BlockComponentGoal() }, { { new BlockConditionCollideAny(), new BlockConditionKey( true ) } }
        ));

        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 80, 56 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 88, 56 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 112, 56 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 120, 56 ),
            { new BlockComponentGoal() }, { { new BlockConditionCollideAny(), new BlockConditionKey( true ) } }
        ));

        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 144, 72, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 152, 72, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 168, 72, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 176, 72, false, false, 0, true ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 152, 72, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 160, 72, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 184, 72, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 192, 72, false, false, 0, true ),
            { new BlockComponentSolid() }
        ));

        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 232, 56 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 240, 56 ),
            nullptr,
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 8, 64 ),
            { new BlockComponentDiamond() }
        ));

        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 248, 56 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 0, 64 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 16, 64 ),
            nullptr,
            { new BlockComponentDiamond() }
        ));

        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 48, 64 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 96, 48 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 56, 64 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 128, 48 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 104, 48 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 80, 64 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 136, 48 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 88, 64 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 216, 64 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 224, 64 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 136, 48 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 144, 48 ),
            { }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 200, 64 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 208, 64 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 184, 64 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 192, 64 ),
            { new BlockComponentHighSlopeRight() }
        ));
        block_types_.push_back( new BlockType
        (
            nullptr,
            nullptr,
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 168, 64 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 176, 64 ),
            { new BlockComponentLowSlopeRight() }
        ));
        block_types_.push_back( new BlockType
        (
            new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair( 232, 64 ), std::make_pair(  0, 72 ), std::make_pair(  48, 72 ), std::make_pair(   96, 72 ) } ),
            new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair( 240, 64 ), std::make_pair(  8, 72 ), std::make_pair(  56, 72 ), std::make_pair(  104, 72 ) } ),
            new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair( 232, 72 ), std::make_pair( 24, 72 ), std::make_pair(  72, 72 ), std::make_pair(  120, 72 ) } ),
            new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair( 240, 72 ), std::make_pair( 32, 72 ), std::make_pair(  80, 72 ), std::make_pair(  128, 72 ) } ),
            { new BlockComponentSolid(), new BlockComponentConveyor( Direction::Horizontal::LEFT ) }, { {}, { new BlockConditionCollideBottom() } }
        ));
        block_types_.push_back( new BlockType
        (
            new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair( 240, 64 ), std::make_pair(  8, 72 ), std::make_pair(  56, 72 ), std::make_pair(  104, 72 ) } ),
            new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair( 240, 64 ), std::make_pair(  8, 72 ), std::make_pair(  56, 72 ), std::make_pair(  104, 72 ) } ),
            new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair( 240, 72 ), std::make_pair( 32, 72 ), std::make_pair(  80, 72 ), std::make_pair(  128, 72 ) } ),
            new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair( 240, 72 ), std::make_pair( 32, 72 ), std::make_pair(  80, 72 ), std::make_pair(  128, 72 ) } ),
            { new BlockComponentSolid(), new BlockComponentConveyor( Direction::Horizontal::LEFT ) }, { {}, { new BlockConditionCollideBottom() } }
        ));
        block_types_.push_back( new BlockType
        (
            new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair( 240, 64 ), std::make_pair(  8, 72 ), std::make_pair(  56, 72 ), std::make_pair(  104, 72 ) } ),
            new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair( 248, 64 ), std::make_pair( 16, 72 ), std::make_pair(  64, 72 ), std::make_pair(  112, 72 ) } ),
            new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair( 240, 72 ), std::make_pair( 32, 72 ), std::make_pair(  80, 72 ), std::make_pair(  128, 72 ) } ),
            new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair( 248, 72 ), std::make_pair( 40, 72 ), std::make_pair(  88, 72 ), std::make_pair(  136, 72 ) } ),
            { new BlockComponentSolid(), new BlockComponentConveyor( Direction::Horizontal::LEFT ) }, { {}, { new BlockConditionCollideBottom() } }
        ));

        block_types_.push_back( new BlockType
        (
            new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair(   96, 72 ), std::make_pair(  48, 72 ), std::make_pair(  0, 72 ), std::make_pair( 232, 64 ) } ),
            new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair(  104, 72 ), std::make_pair(  56, 72 ), std::make_pair(  8, 72 ), std::make_pair( 240, 64 ) } ),
            new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair(  120, 72 ), std::make_pair(  72, 72 ), std::make_pair( 24, 72 ), std::make_pair( 232, 72 ) } ),
            new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair(  128, 72 ), std::make_pair(  80, 72 ), std::make_pair( 32, 72 ), std::make_pair( 240, 72 ) } ),
            { new BlockComponentSolid(), new BlockComponentConveyor( Direction::Horizontal::RIGHT ) }, { {}, { new BlockConditionCollideBottom() } }
        ));
        block_types_.push_back( new BlockType
        (
            new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair(  104, 72 ), std::make_pair(  56, 72 ), std::make_pair(  8, 72 ), std::make_pair( 240, 64 ) } ),
            new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair(  104, 72 ), std::make_pair(  56, 72 ), std::make_pair(  8, 72 ), std::make_pair( 240, 64 ) } ),
            new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair(  128, 72 ), std::make_pair(  80, 72 ), std::make_pair( 32, 72 ), std::make_pair( 240, 72 ) } ),
            new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair(  128, 72 ), std::make_pair(  80, 72 ), std::make_pair( 32, 72 ), std::make_pair( 240, 72 ) } ),
            { new BlockComponentSolid(), new BlockComponentConveyor( Direction::Horizontal::RIGHT ) }, { {}, { new BlockConditionCollideBottom() } }
        ));
        block_types_.push_back( new BlockType
        (
            new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair(  104, 72 ), std::make_pair(  56, 72 ), std::make_pair(  8, 72 ), std::make_pair( 240, 64 ) } ),
            new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair(  112, 72 ), std::make_pair(  64, 72 ), std::make_pair( 16, 72 ), std::make_pair( 248, 64 ) } ),
            new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair(  128, 72 ), std::make_pair(  80, 72 ), std::make_pair( 32, 72 ), std::make_pair( 240, 72 ) } ),
            new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair(  136, 72 ), std::make_pair(  88, 72 ), std::make_pair( 40, 72 ), std::make_pair( 248, 72 ) } ),
            { new BlockComponentSolid(), new BlockComponentConveyor( Direction::Horizontal::RIGHT ) }, { {}, { new BlockConditionCollideBottom() } }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 24, 80 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 24, 80 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 32, 80 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 32, 80 ),
            {}
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 200, 72, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 208, 72, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY,   0, 80, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY,   8, 80, false, false, 0, true ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 216, 72, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 224, 72, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY,   8, 80, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY,  16, 80, false, false, 0, true ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 32, 80 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 32, 80 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 32, 80 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 32, 80 ),
            { new BlockComponentSwimmable() }, { { new BlockConditionCollideAny() } }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 48, 80 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 56, 80 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 64, 80 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 72, 80 ),
            { new BlockComponentChangeDirection( Direction::Simple::LEFT ) }, { { new BlockConditionCollideAny() } }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 80, 80 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 88, 80 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 96, 80 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 104, 80 ),
            { new BlockComponentChangeDirection( Direction::Simple::UP ) }, { { new BlockConditionCollideAny() } }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 40, 80 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 40, 80 ),
            nullptr,
            nullptr,
            { new BlockComponentSolid() }, { { new BlockConditionCollideBottom() } }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 232, 16, false, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 240, 16, false, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 240,  8, true, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 240,  8, false, true ),
            { new BlockComponentHurt(), new BlockComponentSolid() }, { { new BlockConditionCollideTop() } }
        ));
        block_types_.push_back( new BlockType
        (
            nullptr,
            nullptr,
            nullptr,
            nullptr,
            { new BlockComponentChangePalette( { Palette::PaletteType::ICY_BLUE, 2 } ) }, { { new BlockConditionHero() } }
        ));
        block_types_.push_back( new BlockType
        (
            nullptr,
            nullptr,
            nullptr,
            nullptr,
            { new BlockComponentChangePalette( { Palette::PaletteType::GRAYSCALE, 2 } ) }, { { new BlockConditionHero() } }
        ));
        block_types_.push_back( new BlockType
        (
            nullptr,
            nullptr,
            nullptr,
            nullptr,
            { new BlockComponentChangePalette( { Palette::PaletteType::SUNNY_YELLOW, 2 } ) }, { { new BlockConditionHero() } }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 48, 80 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 56, 80 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 64, 80 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 72, 80 ),
            { new BlockComponentChangeDirection( Direction::Simple::RIGHT ) }, { { new BlockConditionCollideAny() } }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 0, 16 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 8, 16 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 0, 24 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 8, 24 ),
            { new BlockComponentKey(), new BlockComponentSingleUse() }, { { new BlockConditionCollideAny(), new BlockConditionHero() }, { new BlockConditionCollideAny(), new BlockConditionHero() } }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 128, 80 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 136, 80 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 128, 88 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 136, 88 ),
            { new BlockComponentChangeBlock( 69 ), new BlockComponentSolid() }, { { new BlockConditionCollideAny(), new BlockConditionKey( true ) } }
        ));
        block_types_.push_back( new BlockType
        (
            nullptr,
            nullptr,
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 16, 88 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 24, 88 ),
            { }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 176, 80 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 184, 80 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 184, 80 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 176, 80 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 192, 80 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 200, 80 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 216, 80 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 224, 80 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 200, 80 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 200, 80 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 224, 80 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 224, 80 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 200, 80 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 208, 80 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 224, 80 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 232, 80 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::pair<int, int> ( 152, 16 ), std::pair<int, int> ( 160, 16 ), std::pair<int, int> ( 24, 80 ), std::pair<int, int> ( 32, 80 ) }, false, false, 0, true ),
            new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::pair<int, int> ( 152, 16 ), std::pair<int, int> ( 160, 16 ), std::pair<int, int> ( 24, 80 ), std::pair<int, int> ( 32, 80 ) }, false, false, 0, true ),
            new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::pair<int, int> ( 144, 112 ), std::pair<int, int> ( 152, 112 ), std::pair<int, int> ( 160, 112 ), std::pair<int, int> ( 168, 112 ) }, false, false, 0, true ),
            new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::pair<int, int> ( 144, 112 ), std::pair<int, int> ( 152, 112 ), std::pair<int, int> ( 160, 112 ), std::pair<int, int> ( 168, 112 ) }, false, false, 0, true ),
            {}
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 72, 40, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 72, 40, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 72, 40, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 72, 40, false, false, 0, true ),
            { new BlockComponentSwimmable(), new BlockComponentHurt() }, { { new BlockConditionCollideAny() }, { new BlockConditionCollideAny() } }
        ));

        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 192, 96 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 200, 96 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 200, 96 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 192, 96 ),
            { new BlockComponentSolid() }
        ));

        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 30*8, 8, false, false, -90 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 30*8, 16, false, false, -90 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 29*8, 8, false, false, -90 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 29*8, 16, false, false, -90 ),
            { new BlockComponentHurt(), new BlockComponentSolid() }, { { new BlockConditionHero(), new BlockConditionCollideRight() } }
        ));


        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 29*8, 16, false, false, 90 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 29*8, 8, false, false, 90 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 30*8, 16, false, false, 90 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 30*8, 8, false, false, 90 ),
            { new BlockComponentHurt(), new BlockComponentSolid() }, { { new BlockConditionHero(), new BlockConditionCollideLeft() } }
        ));

        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 96, 112 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 104, 112 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 104, 112, 0, 0, 180 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 96, 112, 0, 0, 180 ),
            { new BlockComponentSolid(), new BlockComponentBouncy() }, {{}}
        ));


        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 112, 112 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 120, 112 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 128, 112 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 136, 112 ),
            { new BlockComponentHurt(), new BlockComponentSolid() }, { { new BlockConditionHero(), new BlockConditionCollideAny() } }
        ));


        block_types_.push_back( new BlockType
        (
            nullptr,
            nullptr,
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 240, 80, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 248, 80, false, false, 0, true ),
            {}
        ));
        block_types_.push_back( new BlockType
        (
            nullptr,
            nullptr,
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 248, 80, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 248, 80, false, false, 0, true ),
            {}
        ));
        block_types_.push_back( new BlockType
        (
            nullptr,
            nullptr,
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 248, 80, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 200, 88, false, false, 0, true ),
            {}
        ));

        // SWITCH_FLIP
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 216, 104 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 224, 104 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 232, 104 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 240, 104 ),
            { new BlockComponentSwitchFlip(), new BlockComponentSolid() }, { { new BlockConditionCollideTop() }, {} }
        ));

        // TRASH_BOTTOM
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 32, 88 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 40, 88 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 48, 88 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 56, 88 ),
            { new BlockComponentSolid() }
        ));

        //SWITCH_BUTTON_BLACK
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 48, 112 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 56, 112 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 64, 112 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 72, 112 ),
            {
                new BlockComponentSwitch
                (
                    nullptr,
                    new BlockComponentSwitchFlip(),
                    {
                        nullptr,
                        nullptr,
                        new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 32, 112 ),
                        new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 40, 112 )
                    },
                    {
                        new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 0, 112 ),
                        new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 8, 112 ),
                        new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 16, 112 ),
                        new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 24, 112 )
                    }
                )
            },
            { { new BlockConditionCollideTop() } }
        ));

        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 216, 80 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 224, 80 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 216, 80 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 224, 80 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 224, 80 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 224, 80 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 224, 80 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 224, 80 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 224, 80 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 232, 80 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 224, 80 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 232, 80 ),
            { new BlockComponentSolid() }
        ));


        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 0, 96 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 8, 96 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 24, 96 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 32, 96 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 8, 96 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 8, 96 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 32, 96 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 32, 96 ),
            { new BlockComponentSolid() }, { { new BlockConditionCollideBottom() } }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 8, 96 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 16, 96 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 32, 96 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 40, 96 ),
            { new BlockComponentSolid() }
        ));



        block_types_.push_back( new BlockType
        (
            nullptr,
            nullptr,
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 96, 96 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 104, 96 ),
            { new BlockComponentLowSlopeLeft() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 96, 96 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 104, 96 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 112, 96 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 120, 96 ),
            { new BlockComponentHighSlopeLeft() }
        ));

        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 144, 96 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 152, 96 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 160, 96 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 168, 96 ),
            { new BlockComponentHighSlopeRight() }
        ));
        block_types_.push_back( new BlockType
        (
            nullptr,
            nullptr,
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 144, 96 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 152, 96 ),
            { new BlockComponentLowSlopeRight() }
        ));

        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 208, 88, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 216, 88, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 224, 88, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 232, 88, false, false, 0, true ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 248, 96, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 216, 88, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 240, 88, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 232, 88, false, false, 0, true ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 248, 96, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 248, 88, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 240, 88, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 112, 80, false, false, 0, true ),
            { new BlockComponentSolid() }
        ));


        //SWITCH_BLOCK_BLACK
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 144, 80 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 152, 80 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 144, 88 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 152, 88 ),
            {
                new BlockComponentSwitch
                (
                    new BlockComponentSolid(),
                    nullptr,
                    {
                        new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 144, 80 ),
                        new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 152, 80 ),
                        new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 144, 88 ),
                        new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 152, 88 ),
                    },
                    {
                        new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 208, 104 ),
                        new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 208, 104, true ),
                        new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 208, 104, false, true ),
                        new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 208, 104, true, true ),
                    }
                )
            }
        ));

        //SWITCH_BLOCK_WHITE
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 200, 104 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 200, 104, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 200, 104, false, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 200, 104, true, true ),
            {
                new BlockComponentSwitch
                (
                    nullptr,
                    new BlockComponentSolid(),
                    {
                        new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 200, 104 ),
                        new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 200, 104, true ),
                        new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 200, 104, false, true ),
                        new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 200, 104, true, true )
                    },
                    {
                        new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 160, 80 ),
                        new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 168, 80 ),
                        new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 160, 88 ),
                        new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 168, 88 )
                    }
                )
            }
        ));

        //SWITCH_BUTTON_WHITE
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 48, 112 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 56, 112 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 64, 112 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 72, 112 ),
            {
                new BlockComponentSwitch
                (
                    new BlockComponentSwitchFlip(),
                    nullptr,
                    {
                        new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 48, 112 ),
                        new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 56, 112 ),
                        new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 64, 112 ),
                        new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 72, 112 )
                    },
                    {
                        nullptr,
                        nullptr,
                        new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 80, 112 ),
                        new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 88, 112 )
                    }
                )
            },
            { { new BlockConditionCollideTop() } }
        ));

        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 216, 80 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 224, 80 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 176, 88 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 184, 88 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 224, 80 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 224, 80 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 184, 88 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 184, 88 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 224, 80 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 232, 80 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 184, 88 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 192, 88 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 48, 96 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 56, 96 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 48, 96 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 56, 96 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 56, 96 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 56, 96 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 56, 96 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 56, 96 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 56, 96 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 64, 96 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 56, 96 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 64, 96 ),
            { new BlockComponentSolid() }
        ));



        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 112, 96 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 120, 96 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 128, 96 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 136, 96 ),
            {}
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 128, 96 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 136, 96 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY,  56, 96 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY,  56, 96 ),
            {}
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 176, 96 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 184, 96 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY,  56, 96 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY,  56, 96 ),
            {}
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 160, 96 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 168, 96 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 176, 96 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 184, 96 ),
            {}
        ));

        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY,  96, 88, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 144, 16, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY,  96, 88, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 144, 16, false, false, 0, true ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 144, 16, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 144, 16, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 144, 16, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 144, 16, false, false, 0, true ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 144, 16, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 104, 88, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 144, 16, false, false, 0, true ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 104, 88, false, false, 0, true ),
            { new BlockComponentSolid() }
        ));

        block_types_.push_back( new BlockType
        (
            new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair( 232, 64 ), std::make_pair(  0, 72 ), std::make_pair(  48, 72 ), std::make_pair(   96, 72 ) } ),
            new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair( 240, 64 ), std::make_pair(  8, 72 ), std::make_pair(  56, 72 ), std::make_pair(  104, 72 ) } ),
            new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair( 232, 72 ), std::make_pair( 24, 72 ), std::make_pair(  72, 72 ), std::make_pair(  120, 72 ) } ),
            new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair( 240, 72 ), std::make_pair( 32, 72 ), std::make_pair(  80, 72 ), std::make_pair(  128, 72 ) } ),
            {
                new BlockComponentSwitch
                (
                    new BlockComponentConveyor( Direction::Horizontal::LEFT ),
                    new BlockComponentConveyor( Direction::Horizontal::RIGHT ),
                    {
                        new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair( 232, 64 ), std::make_pair(  0, 72 ), std::make_pair(  48, 72 ), std::make_pair(   96, 72 ) } ),
                        new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair( 240, 64 ), std::make_pair(  8, 72 ), std::make_pair(  56, 72 ), std::make_pair(  104, 72 ) } ),
                        new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair( 232, 72 ), std::make_pair( 24, 72 ), std::make_pair(  72, 72 ), std::make_pair(  120, 72 ) } ),
                        new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair( 240, 72 ), std::make_pair( 32, 72 ), std::make_pair(  80, 72 ), std::make_pair(  128, 72 ) } )
                    },
                    {
                        new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair(   96, 72 ), std::make_pair(  48, 72 ), std::make_pair(  0, 72 ), std::make_pair( 232, 64 ) } ),
                        new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair(  104, 72 ), std::make_pair(  56, 72 ), std::make_pair(  8, 72 ), std::make_pair( 240, 64 ) } ),
                        new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair(  120, 72 ), std::make_pair(  72, 72 ), std::make_pair( 24, 72 ), std::make_pair( 232, 72 ) } ),
                        new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair(  128, 72 ), std::make_pair(  80, 72 ), std::make_pair( 32, 72 ), std::make_pair( 240, 72 ) } )
                    }
                ),
                new BlockComponentSolid()
            },
            { { new BlockConditionCollideBottom() } }
        ));

        block_types_.push_back( new BlockType
        (
            new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair( 232, 64 ), std::make_pair(  0, 72 ), std::make_pair(  48, 72 ), std::make_pair(   96, 72 ) } ),
            new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair( 240, 64 ), std::make_pair(  8, 72 ), std::make_pair(  56, 72 ), std::make_pair(  104, 72 ) } ),
            new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair( 232, 72 ), std::make_pair( 24, 72 ), std::make_pair(  72, 72 ), std::make_pair(  120, 72 ) } ),
            new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair( 240, 72 ), std::make_pair( 32, 72 ), std::make_pair(  80, 72 ), std::make_pair(  128, 72 ) } ),
            {
                new BlockComponentSwitch
                (
                    new BlockComponentConveyor( Direction::Horizontal::LEFT ),
                    new BlockComponentConveyor( Direction::Horizontal::RIGHT ),
                    {
                        new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair( 240, 64 ), std::make_pair(  8, 72 ), std::make_pair(  56, 72 ), std::make_pair(  104, 72 ) } ),
                        new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair( 240, 64 ), std::make_pair(  8, 72 ), std::make_pair(  56, 72 ), std::make_pair(  104, 72 ) } ),
                        new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair( 240, 72 ), std::make_pair( 32, 72 ), std::make_pair(  80, 72 ), std::make_pair(  128, 72 ) } ),
                        new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair( 240, 72 ), std::make_pair( 32, 72 ), std::make_pair(  80, 72 ), std::make_pair(  128, 72 ) } )
                    },
                    {
                        new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair(  104, 72 ), std::make_pair(  56, 72 ), std::make_pair(  8, 72 ), std::make_pair( 240, 64 ) } ),
                        new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair(  104, 72 ), std::make_pair(  56, 72 ), std::make_pair(  8, 72 ), std::make_pair( 240, 64 ) } ),
                        new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair(  128, 72 ), std::make_pair(  80, 72 ), std::make_pair( 32, 72 ), std::make_pair( 240, 72 ) } ),
                        new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair(  128, 72 ), std::make_pair(  80, 72 ), std::make_pair( 32, 72 ), std::make_pair( 240, 72 ) } )
                    }
                ),
                new BlockComponentSolid()
            },
            { { new BlockConditionCollideBottom() } }
        ));

        block_types_.push_back( new BlockType
        (
            new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair( 240, 64 ), std::make_pair(  8, 72 ), std::make_pair(  56, 72 ), std::make_pair(  104, 72 ) } ),
            new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair( 248, 64 ), std::make_pair( 16, 72 ), std::make_pair(  64, 72 ), std::make_pair(  112, 72 ) } ),
            new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair( 240, 72 ), std::make_pair( 32, 72 ), std::make_pair(  80, 72 ), std::make_pair(  128, 72 ) } ),
            new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair( 248, 72 ), std::make_pair( 40, 72 ), std::make_pair(  88, 72 ), std::make_pair(  136, 72 ) } ),
            {
                new BlockComponentSwitch
                (
                    new BlockComponentConveyor( Direction::Horizontal::LEFT ),
                    new BlockComponentConveyor( Direction::Horizontal::RIGHT ),
                    {
                        new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair( 240, 64 ), std::make_pair(  8, 72 ), std::make_pair(  56, 72 ), std::make_pair(  104, 72 ) } ),
                        new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair( 248, 64 ), std::make_pair( 16, 72 ), std::make_pair(  64, 72 ), std::make_pair(  112, 72 ) } ),
                        new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair( 240, 72 ), std::make_pair( 32, 72 ), std::make_pair(  80, 72 ), std::make_pair(  128, 72 ) } ),
                        new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair( 248, 72 ), std::make_pair( 40, 72 ), std::make_pair(  88, 72 ), std::make_pair(  136, 72 ) } )
                    },
                    {
                        new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair(  104, 72 ), std::make_pair(  56, 72 ), std::make_pair(  8, 72 ), std::make_pair( 240, 64 ) } ),
                        new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair(  112, 72 ), std::make_pair(  64, 72 ), std::make_pair( 16, 72 ), std::make_pair( 248, 64 ) } ),
                        new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair(  128, 72 ), std::make_pair(  80, 72 ), std::make_pair( 32, 72 ), std::make_pair( 240, 72 ) } ),
                        new AnimatedGraphics( Graphics::SpriteSheet::LVTILESET_CITY, { std::make_pair(  136, 72 ), std::make_pair(  88, 72 ), std::make_pair( 40, 72 ), std::make_pair( 248, 72 ) } )
                    }
                ),
                new BlockComponentSolid()
            },
            { { new BlockConditionCollideBottom() } }
        ));

        block_types_.push_back( nullptr );
        block_types_.push_back( nullptr );
        block_types_.push_back( nullptr );
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 48, 96 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 56, 96 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 72, 96 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 80, 96 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 56, 96 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 56, 96 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 80, 96 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 80, 96 ),
            { new BlockComponentSolid() }
        ));
        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 56, 96 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 64, 96 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 80, 96 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 88, 96 ),
            { new BlockComponentSolid() }
        ));

        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 0, 120 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 8, 120 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 32, 120 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 40, 120 ),
            { new BlockComponentDiamond() }
        ));

        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 16, 120 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 24, 120 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 48, 120 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 56, 120 ),
            { new BlockComponentDiamond() }
        ));

        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 112, 120 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 120, 120 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 128, 120 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 136, 120 ),
            { new BlockComponentMessage(), new BlockComponentSolid() }, { { new BlockConditionCollideTop(), new BlockConditionHero() } }
        ));
        block_types_.push_back( nullptr );
        block_types_.push_back( nullptr );
        block_types_.push_back( nullptr );
        block_types_.push_back( nullptr );
        block_types_.push_back( nullptr );
        block_types_.push_back( nullptr );
        block_types_.push_back( nullptr );
        block_types_.push_back( nullptr );
        block_types_.push_back( nullptr );
        block_types_.push_back( nullptr );
        block_types_.push_back( nullptr );
        block_types_.push_back( nullptr );
        block_types_.push_back( nullptr );


        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 64, 120 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 72, 120 ),
            nullptr,
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 96, 120 ),
            { new BlockComponentDiamond() }
        ));

        block_types_.push_back( new BlockType
        (
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 80, 120 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 88, 120 ),
            new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 104, 120 ),
            nullptr,
            { new BlockComponentDiamond() }
        ));
    };
