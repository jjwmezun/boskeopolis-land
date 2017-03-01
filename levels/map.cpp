
// Name
//===================================
//
// Map
//


// DEPENDENCIES
//===================================

    #include <algorithm>
    #include <iostream>
    #include <fstream>
    #include "game.h"
	#include "mezun_helpers.h"
    #include "map.h"
    #include "rapidjson/document.h"
    #include "rapidjson/istreamwrapper.h"
    #include "unit.h"
    #include "water_effect.h"


// STATIC PROPERTIES
//===================================

// METHODS
//===================================

    Map Map::mapFromPath
    (
        std::string path,
        Palette::PaletteSet palette,
        std::vector<MapLayer*> backgrounds,
        std::vector<Warp> warps,
        std::vector<MapLayer*> foregrounds
    )
    {
	
		// Setup
		//=============================================================
		
			std::vector<int> blocks = {};
			std::vector<int> sprites = {};

			const std::string MAPS_DIR = Game::resourcePath() + "maps" + Game::pathDivider();
			const std::string MAP_PATH = MAPS_DIR + "land-" + path +".json";

			std::ifstream map_stream( MAP_PATH );

			assert( map_stream.is_open() );

			rapidjson::IStreamWrapper map_stream_wrapper( map_stream );
			rapidjson::Document map_data;
			map_data.ParseStream( map_stream_wrapper );

			assert( map_data.IsObject() );



		// Get Blocks & Sprites
		//=============================================================
		
			assert( map_data.HasMember("layers") );
			assert( map_data[ "layers" ].IsArray() );
		
			constexpr int BLOCKS_INDEX = 0;
			constexpr int SPRITES_INDEX = 1;

			int i = 0;
			for ( auto& v : map_data[ "layers" ].GetArray() )
			{
				if ( v.HasMember("data") )
				{
					if ( v[ "data" ].IsArray() )
					{
						for ( auto& n : v[ "data" ].GetArray() )
						{
							if ( i == BLOCKS_INDEX )
							{
								blocks.push_back( n.GetInt() );
							}
							else if ( i == SPRITES_INDEX )
							{
								sprites.push_back( n.GetInt() );
							}
							else
							{
								break;
							}
						}
					}
				}
				++i;
			}



		// Get Map Sizes
		//=============================================	
		
			assert( map_data.HasMember( "width" ) );
			assert( map_data[ "width" ].IsInt() );
			const int width = map_data[ "width" ].GetInt();

			assert( map_data.HasMember( "height" ) );
			assert( map_data[ "height" ].IsInt() );
			const int height = map_data[ "height" ].GetInt();



		// Misc. Features
		//=============================================		
			// Defaults
			bool slippery = false;
			int camera_limit_top = -1;
			int camera_limit_bottom = -1;
			int camera_limit_left = -1;
			int camera_limit_right = -1;
			int water_effect_height_blocks = -1;
			int scroll_loop_width = 0;
			SpriteSystem::HeroType hero_type = SpriteSystem::HeroType::NORMAL;
			Camera::XPriority camera_x_priority = Camera::XPriority::__NULL;
			Camera::YPriority camera_y_priority = Camera::YPriority::__NULL;
			bool blocks_work_offscreen = false;

			// Test for features.
			if ( map_data.HasMember( "properties" ) )
			{
				for ( auto& prop : map_data[ "properties" ].GetObject() )
				{
					std::string name = prop.name.GetString();

					if ( mezun::areStringsEqual( name, "slippery" ) )
					{
						if ( prop.value.IsBool() )
						{
							slippery = prop.value.GetBool();
						}
					}

					if ( mezun::areStringsEqual( name, "camera_limit_top" ) )
					{
						if ( prop.value.IsInt() )
						{
							camera_limit_top = prop.value.GetInt();
						}
					}

					if ( mezun::areStringsEqual( name, "camera_limit_bottom" ) )
					{
						if ( prop.value.IsInt() )
						{
							camera_limit_bottom = prop.value.GetInt();
						}
					}

					if ( mezun::areStringsEqual( name, "camera_limit_left" ) )
					{
						if ( prop.value.IsInt() )
						{
							camera_limit_left = prop.value.GetInt();
						}
					}

					if ( mezun::areStringsEqual( name, "camera_limit_right" ) )
					{
						if ( prop.value.IsInt() )
						{
							camera_limit_right = prop.value.GetInt();
						}
					}

					if ( mezun::areStringsEqual( name, "water_effect_height" ) )
					{
						if ( prop.value.IsInt() )
						{
							water_effect_height_blocks = prop.value.GetInt();
						}
					}

					if ( mezun::areStringsEqual( name, "scroll_loop_width" ) )
					{
						if ( prop.value.IsInt() )
						{
							scroll_loop_width = prop.value.GetInt();
						}
					}

					if ( mezun::areStringsEqual( name, "hero_type" ) )
					{
						if ( prop.value.IsString() )
						{
							hero_type = SpriteSystem::heroType( prop.value.GetString() );
						}
					}

					if ( mezun::areStringsEqual( name, "camera_priority_x" ) )
					{
						if ( prop.value.IsString() )
						{
							const std::string camera_priority_x_string = prop.value.GetString();
							if ( camera_priority_x_string.compare( "CENTER" ) == 0 )
							{
								camera_x_priority = Camera::XPriority::CENTER;
							}
						}
					}

					if ( mezun::areStringsEqual( name, "camera_priority_y" ) )
					{
						if ( prop.value.IsString() )
						{
							const std::string camera_priority_y_string = prop.value.GetString();
							if ( camera_priority_y_string.compare( "CENTER" ) == 0 )
							{
								camera_y_priority = Camera::YPriority::CENTER;
							}
						}
					}

					if ( mezun::areStringsEqual( name, "blocks_work_offscreen" ) )
					{
						if ( prop.value.IsBool() )
						{
							blocks_work_offscreen = prop.value.GetBool();
						}
					}
				}
			}



		// Send all data
		//=============================================	
		
			return Map
			(
				blocks,
				sprites,
				width,
				height,
				palette,
				backgrounds,
				warps,
				foregrounds,
				slippery,
				camera_limit_top,
				camera_limit_bottom,
				camera_limit_left,
				camera_limit_right,
				hero_type,
				water_effect_height_blocks,
				scroll_loop_width,
				camera_x_priority,
				camera_y_priority,
				blocks_work_offscreen
			);
    };

    Map::Map
    (
        std::vector<int> blocks,
        std::vector<int> sprites,
        int width,
        int height,
        Palette::PaletteSet palette,
        std::vector<MapLayer*> backgrounds,
        std::vector<Warp> warps,
        std::vector<MapLayer*> foregrounds,
        bool slippery,
        int top_limit,
        int bottom_limit,
        int left_limit,
        int right_limit,
        SpriteSystem::HeroType hero_type,
        int water_effect_height_blocks,
        int scroll_loop_width,
        Camera::XPriority camera_x_priority,
        Camera::YPriority camera_y_priority,
        bool blocks_work_offscreen
    )
    :
        blocks_ ( blocks ),
        sprites_ ( sprites ),
        width_ ( width ),
        height_ ( height ),
        palette_ ( palette ),
        warps_ ( warps ),
        slippery_ ( slippery ),
        top_limit_ ( top_limit ),
        bottom_limit_ ( bottom_limit ),
        left_limit_ ( left_limit ),
        right_limit_ ( right_limit ),
        hero_type_ ( hero_type ),
        water_effect_ ( testWaterEffect( water_effect_height_blocks ) ),
        scroll_loop_width_ ( scroll_loop_width ),
        changed_ ( true ),
        camera_x_priority_ ( camera_x_priority ),
        camera_y_priority_ ( camera_y_priority ),
        blocks_work_offscreen_ ( blocks_work_offscreen )
    {
        for ( auto& b : backgrounds )
        {
            backgrounds_.emplace_back( std::shared_ptr<MapLayer>( b ) );
        }
        for ( auto& f : foregrounds )
        {
            foregrounds_.emplace_back( std::shared_ptr<MapLayer>( f ) );
        }
    };

    Map::~Map() noexcept {};

    Map::Map( Map&& m ) noexcept
    :
        blocks_ ( m.blocks_ ),
        sprites_ ( m.sprites_ ),
        width_ ( m.width_ ),
        height_ ( m.height_ ),
        palette_ ( m.palette_ ),
        backgrounds_ ( std::move( m.backgrounds_ ) ),
        warps_ ( m.warps_ ),
        foregrounds_ ( std::move( m.foregrounds_ ) ),
        slippery_ ( m.slippery_ ),
        top_limit_ ( m.top_limit_ ),
        bottom_limit_ ( m.bottom_limit_ ),
        left_limit_ ( m.left_limit_ ),
        right_limit_ ( m.right_limit_ ),
        hero_type_ ( m.hero_type_ ),
        water_effect_ ( std::move( m.water_effect_ ) ),
        scroll_loop_width_ ( m.scroll_loop_width_ ),
        camera_x_priority_ ( m.camera_x_priority_ ),
        camera_y_priority_ ( m.camera_y_priority_ ),
		blocks_work_offscreen_ ( m.blocks_work_offscreen_ )
    {};

/*
    Map& Map::operator= ( Map&& m )
    {
        if ( this != &m )
        {
            water_effect_.reset( m.water_effect_.release() );
            blocks_ = m.blocks_;
            sprites_ = m.sprites_;
            width_ = m.width_;
            height_ = m.height_;
            palette_ = m.palette_;
            backgrounds_ = m.backgrounds_;
            warps_ = m.warps_;
            foregrounds_ = m.foregrounds_;
            slippery_ = m.slippery_;
            top_limit_ = m.top_limit_;
            bottom_limit_ = m.bottom_limit_;
            left_limit_ = m.left_limit_;
            right_limit_ = m.right_limit_;
            hero_type_ = m.hero_type_;
        }
    };*/


    Map::Map( const Map& c )
    :
        blocks_ ( c.blocks_ ),
        sprites_ ( c.sprites_ ),
        width_ ( c.width_ ),
        height_ ( c.height_ ),
        palette_ ( c.palette_ ),
        backgrounds_ ( c.backgrounds_ ),
        foregrounds_ ( c.foregrounds_ ),
        warps_ ( c.warps_ ),
        slippery_ ( c.slippery_ ),
        top_limit_ ( c.top_limit_ ),
        bottom_limit_ ( c.bottom_limit_ ),
        left_limit_ ( c.left_limit_ ),
        right_limit_ ( c.right_limit_ ),
        hero_type_ ( c.hero_type_ ),
        scroll_loop_width_ ( c.scroll_loop_width_ ),
        camera_x_priority_ ( c.camera_x_priority_ ),
        camera_y_priority_ ( c.camera_y_priority_ ),
		blocks_work_offscreen_ ( c.blocks_work_offscreen_ )
    {
        if ( c.water_effect_ )
        {
            water_effect_.reset( new WaterEffect( c.water_effect_->yBlocks() ) );
        }
    };
/*
    Map& Map::operator= ( const Map& c )
    {
        if ( this != &c )
        {
            if ( c.water_effect_ )
            {
                water_effect_.reset( new WaterEffect( c.water_effect_->yBlocks() ) );
            }

            blocks_ = c.blocks_;
            sprites_ = c.sprites_;
            width_ = c.width_;
            height_ = c.height_;
            palette_ = c.palette_;
            backgrounds_ = c.backgrounds_;
            warps_ = c.warps_;
            foregrounds_ = c.foregrounds_;
            slippery_ = c.slippery_;
            top_limit_ = c.top_limit_;
            bottom_limit_ = c.bottom_limit_;
            left_limit_ = c.left_limit_;
            right_limit_ = c.right_limit_;
            hero_type_ = c.hero_type_;
        }
    };*/

    int Map::widthBlocks() const
    {
        return width_;
    };

    int Map::heightBlocks() const
    {
        return height_;
    };

    int Map::widthPixels() const
    {
        return Unit::BlocksToPixels( width_ );
    };

    int Map::heightPixels() const
    {
        return Unit::BlocksToPixels( height_ );
    };

    int Map::blocksSize() const
    {
        return std::min( widthBlocks() * heightBlocks(), (int)blocks_.size() );
    };

    int Map::spritesSize() const
    {
        return std::min( widthBlocks() * heightBlocks(), (int)sprites_.size() );
    };

    int Map::block( int n ) const
    {
        if ( scrollLoop() )
        {
        }

        if ( n < 0 || n >= blocks_.size() )
        {
            return 0;
        }
        else
        {
            return blocks_.at( n );
        }
    };

    int Map::sprite( int n ) const
    {
        return sprites_[ n ];
    };

    int Map::mapX( int n ) const
    {
        return n % widthBlocks();
    };

    int Map::mapY( int n ) const
    {
        return floor( n / widthBlocks() );
    };

    int Map::indexFromXAndY( int x, int y ) const
    {
        if ( scrollLoop() )
        {
            while ( x < 0 )
            {
                x = widthBlocks() + x;
            }

            x = x % widthBlocks();
        }
        else
        {
            if ( x < 0 )
            {
                x = 0;
            }
            if ( x >= widthBlocks() )
            {
                x = widthBlocks() - 1;
            }
        }

        if ( y < 0 )
        {
            y = 0;
        }
        if ( y >= heightBlocks() )
        {
            y = heightBlocks() - 1;
        }

        return ( y * widthBlocks() ) + x;
    };

    const bool Map::changed() const
    {
        return changed_;
    };

    void Map::changeBlock( int where, int value )
    {
        if ( inBounds( where ) )
        {
            blocks_[ where ] = value;
        }
        changed_ = true;
    };

    void Map::deleteBlock( int where )
    {
        changeBlock( where, 0 );
    };

    bool Map::inBounds( int n ) const
    {
        return n > 0 && n < blocks_.size();
    };

    void Map::update()
    {
        for ( int i = 0; i < backgrounds_.size(); ++i )
        {
            backgrounds_[ i ]->update();
        }
        for ( int i = 0; i < foregrounds_.size(); ++i )
        {
            foregrounds_[ i ]->update();
        }
        changed_ = false;
		
		if ( water_effect_ )
		{
			water_effect_->update();
		}
    };

    Palette::PaletteSet Map::palette() const
    {
        return palette_;
    };

    void Map::renderBG( Graphics& graphics, Camera& camera )
    {
        for ( int i = 0; i < backgrounds_.size(); ++i )
        {
            backgrounds_[ i ]->render( graphics, camera );
        }
    };

    void Map::renderFG( Graphics& graphics, Camera& camera )
    {
        if ( water_effect_ )
        {
            water_effect_->render( graphics, camera );
        }

        for ( int i = 0; i < foregrounds_.size(); ++i )
        {
            foregrounds_[ i ]->render( graphics, camera );
        }
    };

    const Warp* Map::getWarp( int x_sub_pixels, int y_sub_pixels ) const
    {
        for ( int i = 0; i < warps_.size(); ++i )
        {
            if ( warps_[ i ].inInterval( x_sub_pixels, y_sub_pixels ) )
                return &warps_[ i ];
        }

        return nullptr;
    };

    bool Map::slippery() const
    {
        return slippery_;
    };

    int Map::topLimit() const
    {
        return top_limit_;
    };

    int Map::bottomLimit() const
    {
        return bottom_limit_;
    };

    int Map::leftLimit() const
    {
        return left_limit_;
    };

    int Map::rightLimit() const
    {
        return right_limit_;
    };

    SpriteSystem::HeroType Map::heroType() const
    {
        return hero_type_;
    };

    const WaterEffect* Map::effect()
    {
        return water_effect_.get();
    };

    WaterEffect* Map::testWaterEffect( int n ) const
    {
        return ( n > -1 ) ? new WaterEffect( n ) : nullptr;
    };

    bool Map::scrollLoop() const
    {
        return scroll_loop_width_ != 0;
    };

    int Map::scrollLoopWidth() const
    {
        return scroll_loop_width_;
    };

    Camera::XPriority Map::cameraXPriority() const
    {
        return camera_x_priority_;
    };

    Camera::YPriority Map::cameraYPriority() const
    {
        return camera_y_priority_;
    };

    bool Map::blocksWorkOffscreen() const
    {
        return blocks_work_offscreen_;
    };

	void Map::setChanged()
	{
		changed_ = true;
	};