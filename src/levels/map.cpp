#include <algorithm>
#include <fstream>
#include "main.hpp"
#include "mezun_helpers.hpp"
#include "map.hpp"
#include "map_layer_tilemap.hpp"
#include "map_layer_water.hpp"
#include "map_layer_water_back.hpp"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "unit.hpp"

static constexpr int LOOP_CHANGE = 3;
static constexpr int FLASH_INTERVAL = 200;
static constexpr int FLASH_DURATION = 8;
static constexpr int INNER_FLASH_DURATION = 2;
static constexpr int BG_LAYER_LIMIT = 5;

Map::LayerInfo Map::getLayerInfo( const std::string& layer_name )
{
	if ( layer_name == "Blocks" )
	{
		return { LayerType::BLOCKS, 0 };
	}
	else if ( layer_name == "Sprites" )
	{
		return { LayerType::SPRITES, 0 };
	}
	else
	{
		for ( int i = 1; i <= BG_LAYER_LIMIT; ++i )
		{
			const std::string bg_line = std::string( std::string( "BG" ) + std::to_string( i ) );
			const std::string fg_line = std::string( std::string( "FG" ) + std::to_string( i ) );
			const std::string fade_fg_line = std::string( std::string( "Fade FG" ) + std::to_string( i ) );

			if ( layer_name == bg_line )
			{
				return { LayerType::BACKGROUND, i };
			}
			else if ( layer_name == fg_line )
			{
				return { LayerType::FOREGROUND, i };
			}
			else if ( layer_name == fade_fg_line )
			{
				return { LayerType::FADE_FOREGROUND, i };
			}
		}
	}
	return { LayerType::__NULL, 0 };
};

Map Map::mapFromPath
(
	std::string path,
	std::vector<std::unique_ptr<MapLayer>> backgrounds,
	std::vector<Warp> warps,
	std::vector<std::unique_ptr<MapLayer>> foregrounds
)
{

	// Setup
	//=============================================================

		std::vector<int> blocks = {};
		std::vector<int> sprites = {};
		std::vector<std::vector<int>> bg_block_layers = {};
		std::vector<std::vector<int>> fg_block_layers = {};
		std::vector<std::vector<int>> fade_fg_block_layers = {};

		std::string palette = "Grayscale";
		int bg_color = 1;
		std::string tileset = "urban";
		bool slippery = false;
		int camera_limit_top = -1;
		int camera_limit_bottom = -1;
		int camera_limit_left = -1;
		int camera_limit_right = -1;
		int scroll_loop_width = 0;
		SpriteSystem::HeroType hero_type = SpriteSystem::HeroType::NORMAL;
		Camera::XPriority camera_x_priority = Camera::XPriority::__NULL;
		Camera::YPriority camera_y_priority = Camera::YPriority::__NULL;
		bool blocks_work_offscreen = false;
		bool loop_sides = false;
		int water_effect_height = 0;
		std::string water_effect_type = "NORMAL";
		int wind_strength = 0;
		bool moon_gravity = false;
		bool show_on_off = false;
		int lightning_flash_color = 0;
		std::string music = "";
		bool warp_on_fall = false;

		const std::string MAPS_DIR = Main::resourcePath() + "maps" + Main::pathDivider();
		const std::string MAP_PATH = MAPS_DIR + "land-" + path +".json";

		std::ifstream map_stream( MAP_PATH );

		assert( map_stream.is_open() );

		rapidjson::IStreamWrapper map_stream_wrapper( map_stream );
		rapidjson::Document map_data;
		map_data.ParseStream( map_stream_wrapper );

		assert( map_data.IsObject() );






	// Get Map Sizes
	//=============================================	

		assert( map_data.HasMember( "width" ) );
		assert( map_data[ "width" ].IsInt() );
		const int width = map_data[ "width" ].GetInt();

		assert( map_data.HasMember( "height" ) );
		assert( map_data[ "height" ].IsInt() );
		const int height = map_data[ "height" ].GetInt();
		
		
		
		
		
		
	// Get Blocks & Sprites
	//=============================================================

		assert( map_data.HasMember("layers") );
		assert( map_data[ "layers" ].IsArray() );

		int i = 0;
		for ( auto& v : map_data[ "layers" ].GetArray() )
		{
			if ( v.HasMember( "data" ) && v[ "data" ].IsArray() )
			{
				const std::string layer_type = v[ "name" ].GetString();
				const LayerInfo layer_info = getLayerInfo( layer_type );

				for ( auto& n : v[ "data" ].GetArray() )
				{
					switch ( layer_info.type )
					{
						case ( LayerType::BLOCKS ):
							blocks.push_back( n.GetInt() );
						break;
						case ( LayerType::SPRITES ):
							sprites.push_back( n.GetInt() );
						break;
						case ( LayerType::BACKGROUND ):
							while ( bg_block_layers.size() < layer_info.n )
							{
								bg_block_layers.emplace_back( std::vector<int> () );
							}
							bg_block_layers[ layer_info.n - 1 ].emplace_back( n.GetInt() );
						break;
						case ( LayerType::FOREGROUND ):
							while ( fg_block_layers.size() < layer_info.n )
							{
								fg_block_layers.emplace_back( std::vector<int> () );
							}
							fg_block_layers[ layer_info.n - 1 ].emplace_back( n.GetInt() );
						break;
						case ( LayerType::FADE_FOREGROUND ):
							while ( fade_fg_block_layers.size() < layer_info.n )
							{
								fade_fg_block_layers.emplace_back( std::vector<int> () );
							}
							fade_fg_block_layers[ layer_info.n - 1 ].emplace_back( n.GetInt() );
						break;
					}
				}
			}
			++i;
		}
		
		for ( auto& bg_block_layer : bg_block_layers )
		{
			backgrounds.emplace_back( new MapLayerTilemap( bg_block_layer, width, height ) );
		}
		for ( auto& fg_block_layer : fg_block_layers )
		{
			foregrounds.emplace_back( new MapLayerTilemap( fg_block_layer, width, height, false ) );
		}
		for ( auto& fade_fg_block_layer : fade_fg_block_layers )
		{
			foregrounds.emplace_back( new MapLayerTilemap( fade_fg_block_layer, width, height, true ) );
		}



	// Misc. Features
	//=============================================		
		// Defaults

		// Test for features.
		if ( map_data.HasMember( "properties" ) )
		{
			std::unordered_map< std::string, rapidjson::GenericValue< rapidjson::UTF8<> > > properties;
			auto& prop_group = map_data[ "properties" ];

			// For newer versions o' Tiled, where properties is an array o' objects:
			// {
			//  	"name": %name%,
			//  	"type": %type%,
			//  	"value": %value%
			// }
			if ( prop_group.IsArray() )
			{
				for ( auto& prop_obj : prop_group.GetArray() )
				{
					auto prop = prop_obj.GetObject();
					std::string name = "";
					rapidjson::GenericValue< rapidjson::UTF8<> > value;

					for ( auto& prop_item : prop )
					{
						auto prop_key = prop_item.name.GetString();
						if ( mezun::areStringsEqual( prop_key, "name" ) )
						{
							name = prop_item.value.GetString();
						}
						else if ( mezun::areStringsEqual( prop_key, "value" ) )
						{
							value = prop_item.value;
						}
					}

					properties[ name ] = value;
				}
			}
			// For older versions o' Tiled, where properties is an object of %name%: %value% pairs.
			else if ( prop_group.IsObject() )
			{
				for ( auto& prop : prop_group.GetObject() )
				{
					properties[ prop.name.GetString() ] = prop.value;
				}
			}

			for ( auto& pair : properties )
			{
				const std::string& name = pair.first;
				auto& value = pair.second;

				if ( mezun::areStringsEqual( name, "tileset" ) )
				{
					if ( value.IsString() )
					{
						tileset = value.GetString();
					}
				}

				else if ( mezun::areStringsEqual( name, "palette" ) )
				{
					if ( value.IsString() )
					{
						palette = value.GetString();
					}
				}

				else if ( mezun::areStringsEqual( name, "bg_color" ) )
				{
					if ( value.IsInt() )
					{
						bg_color = value.GetInt();
					}
					else if ( value.IsString() )
					{
						bg_color = std::stoi( value.GetString() );
					}
				}

				else if ( mezun::areStringsEqual( name, "music" ) )
				{
					if ( value.IsString() )
					{
						music = value.GetString();
					}
				}

				else if ( mezun::areStringsEqual( name, "slippery" ) )
				{
					if ( value.IsBool() )
					{
						slippery = value.GetBool();
					}
					else if ( value.IsString() )
					{
						std::cout<<value.GetString()<<std::endl;
						slippery = ( ( strcmp( value.GetString(), "true" ) == 0 ) ? true : false );
					}
				}

				else if ( mezun::areStringsEqual( name, "camera_limit_top" ) )
				{
					if ( value.IsInt() )
					{
						camera_limit_top = value.GetInt();
					}
				}

				else if ( mezun::areStringsEqual( name, "camera_limit_bottom" ) )
				{
					if ( value.IsInt() )
					{
						camera_limit_bottom = value.GetInt();
					}
				}

				else if ( mezun::areStringsEqual( name, "camera_limit_left" ) )
				{
					if ( value.IsInt() )
					{
						camera_limit_left = value.GetInt();
					}
				}

				else if ( mezun::areStringsEqual( name, "camera_limit_right" ) )
				{
					if ( value.IsInt() )
					{
						camera_limit_right = value.GetInt();
					}
				}

				else if ( mezun::areStringsEqual( name, "scroll_loop_width" ) )
				{
					if ( value.IsInt() )
					{
						scroll_loop_width = value.GetInt();
					}
				}

				else if ( mezun::areStringsEqual( name, "hero_type" ) )
				{
					if ( value.IsString() )
					{
						hero_type = SpriteSystem::heroType( value.GetString() );
					}
				}

				else if ( mezun::areStringsEqual( name, "camera_priority_x" ) )
				{
					if ( value.IsString() )
					{
						const std::string camera_priority_x_string = value.GetString();
						if ( camera_priority_x_string.compare( "CENTER" ) == 0 )
						{
							camera_x_priority = Camera::XPriority::CENTER;
						}
					}
				}

				else if ( mezun::areStringsEqual( name, "camera_priority_y" ) )
				{
					if ( value.IsString() )
					{
						const std::string camera_priority_y_string = value.GetString();
						if ( camera_priority_y_string.compare( "CENTER" ) == 0 )
						{
							camera_y_priority = Camera::YPriority::CENTER;
						}
					}
				}

				else if ( mezun::areStringsEqual( name, "blocks_work_offscreen" ) )
				{
					if ( value.IsBool() )
					{
						blocks_work_offscreen = value.GetBool();
					}
				}

				else if ( mezun::areStringsEqual( name, "loop_sides" ) )
				{
					if ( value.IsBool() )
					{
						loop_sides = value.GetBool();
					}
				}

				else if ( mezun::areStringsEqual( name, "water_effect_height" ) )
				{
					if ( value.IsInt() )
					{
						water_effect_height = value.GetInt();
					}
				}

				else if ( mezun::areStringsEqual( name, "water_effect_type" ) )
				{
					if ( value.IsString() )
					{
						water_effect_type = value.GetString();
					}
				}

				else if ( mezun::areStringsEqual( name, "wind_strength" ) )
				{
					if ( value.IsInt() )
					{
						wind_strength = value.GetInt();
					}
				}

				else if ( mezun::areStringsEqual( name, "moon_gravity" ) )
				{
					if ( value.IsBool() )
					{
						moon_gravity = value.GetBool();
					}
				}

				else if ( mezun::areStringsEqual( name, "show_on_off" ) )
				{
					if ( value.IsBool() )
					{
						show_on_off = value.GetBool();
					}
				}

				else if ( mezun::areStringsEqual( name, "warp_on_fall" ) )
				{
					if ( value.IsBool() )
					{
						warp_on_fall = value.GetBool();
					}
				}

				else if ( mezun::areStringsEqual( name, "lightning_flash_color" ) )
				{
					if ( value.IsInt() )
					{
						lightning_flash_color = value.GetInt();
						assert( lightning_flash_color >= 0 || lightning_flash_color < Palette::COLOR_LIMIT );
					}
				}
			}
		}

		MapLayerWater* water_ptr = nullptr;
		if ( water_effect_type == "RISING" )
		{
			water_ptr = MapLayerWater::makeRisingWater( water_effect_height );
		}
		else if ( water_effect_type == "SLUDGE" )
		{
			water_ptr = MapLayerWater::makeSludgeWater( water_effect_height );
		}
		else if ( water_effect_height != 0 )
		{
			water_ptr = MapLayerWater::makeNormalWater( water_effect_height );
		}

		if ( water_ptr != nullptr )
		{
			backgrounds.emplace_back( std::make_unique<MapLayerWaterBack> ( water_ptr ) );
			foregrounds.emplace_back( water_ptr );
		}



	// Send all data
	//=============================================	

		return Map
		(
			blocks,
			sprites,
			width,
			height,
			tileset,
			{ palette, bg_color },
			std::move( backgrounds ),
			warps,
			std::move( foregrounds ),
			slippery,
			camera_limit_top,
			camera_limit_bottom,
			camera_limit_left,
			camera_limit_right,
			hero_type,
			scroll_loop_width,
			camera_x_priority,
			camera_y_priority,
			blocks_work_offscreen,
			loop_sides,
			wind_strength,
			moon_gravity,
			show_on_off,
			lightning_flash_color,
			music,
			warp_on_fall
		);
};

Map::Map
(
	std::vector<int> blocks,
	std::vector<int> sprites,
	int width,
	int height,
	std::string tileset,
	Palette palette,
	std::vector<std::unique_ptr<MapLayer>> backgrounds,
	std::vector<Warp> warps,
	std::vector<std::unique_ptr<MapLayer>> foregrounds,
	bool slippery,
	int top_limit,
	int bottom_limit,
	int left_limit,
	int right_limit,
	SpriteSystem::HeroType hero_type,
	int scroll_loop_width,
	Camera::XPriority camera_x_priority,
	Camera::YPriority camera_y_priority,
	bool blocks_work_offscreen,
	bool loop_sides,
	int wind_strength,
	bool moon_gravity,
	bool show_on_off,
	int lightning_flash_color,
	std::string music,
	bool warp_on_fall
)
:
	blocks_ ( blocks ),
	sprites_ ( sprites ),
	width_ ( width ),
	height_ ( height ),
	tileset_ ( tileset ),
	palette_ ( palette ),
	warps_ ( warps ),
	slippery_ ( slippery ),
	top_limit_ ( top_limit ),
	bottom_limit_ ( bottom_limit ),
	left_limit_ ( left_limit ),
	right_limit_ ( right_limit ),
	hero_type_ ( hero_type ),
	scroll_loop_width_ ( scroll_loop_width ),
	current_loop_ ( 0 ),
	changed_ ( true ),
	camera_x_priority_ ( camera_x_priority ),
	camera_y_priority_ ( camera_y_priority ),
	blocks_work_offscreen_ ( blocks_work_offscreen ),
	loop_sides_ ( loop_sides ),
	wind_strength_ ( wind_strength ),
	moon_gravity_ ( moon_gravity ),
	show_on_off_ ( show_on_off ),
	lightning_flash_color_ ( lightning_flash_color ),
	current_bg_ ( palette.bgN() ),
	music_ ( music ),
	warp_on_fall_ ( warp_on_fall )
{
	for ( auto& b : backgrounds )
	{
		backgrounds_.emplace_back( std::shared_ptr<MapLayer>( b.release() ) );
	}
	for ( auto& f : foregrounds )
	{
		foregrounds_.emplace_back( std::shared_ptr<MapLayer>( f.release() ) );
	}
};

Map::~Map() noexcept {};

Map::Map( Map&& m ) noexcept
:
	blocks_ ( m.blocks_ ),
	sprites_ ( m.sprites_ ),
	width_ ( m.width_ ),
	height_ ( m.height_ ),
	tileset_ ( m.tileset_ ),
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
	scroll_loop_width_ ( m.scroll_loop_width_ ),
	current_loop_ ( m.current_loop_ ),
	camera_x_priority_ ( m.camera_x_priority_ ),
	camera_y_priority_ ( m.camera_y_priority_ ),
	blocks_work_offscreen_ ( m.blocks_work_offscreen_ ),
	loop_sides_ ( m.loop_sides_ ),
	wind_strength_ ( m.wind_strength_ ),
	moon_gravity_ ( m.moon_gravity_ ),
	changed_ ( m.changed_ ),
	show_on_off_ ( m.show_on_off_ ),
	lightning_flash_color_ ( m.lightning_flash_color_ ),
	current_bg_ ( m.current_bg_ ),
	music_ ( m.music_ ),
	warp_on_fall_ ( m.warp_on_fall_ )
{};

Map::Map( const Map& c )
:
	blocks_ ( c.blocks_ ),
	sprites_ ( c.sprites_ ),
	width_ ( c.width_ ),
	height_ ( c.height_ ),
	tileset_ ( c.tileset_ ),
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
	current_loop_ ( c.current_loop_ ),
	camera_x_priority_ ( c.camera_x_priority_ ),
	camera_y_priority_ ( c.camera_y_priority_ ),
	blocks_work_offscreen_ ( c.blocks_work_offscreen_ ),
	loop_sides_ ( c.loop_sides_ ),
	wind_strength_ ( c.wind_strength_ ),
	moon_gravity_ ( c.moon_gravity_ ),
	changed_ ( c.changed_ ),
	show_on_off_ ( c.show_on_off_ ),
	lightning_flash_color_ ( c.lightning_flash_color_ ),
	current_bg_ ( c.current_bg_ ),
	music_ ( c.music_ ),
	warp_on_fall_ ( c.warp_on_fall_ )
{};

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
	return std::min( widthBlocks() * heightBlocks(), ( int )( blocks_.size() ) );
};

int Map::spritesSize() const
{
	return std::min( widthBlocks() * heightBlocks(), ( int )( sprites_.size() ) );
};

int Map::block( int n ) const
{
	if ( !inBounds( n ) )
	{
		return 0;
	}
	else
	{
		return blocks_[ n ];
	}
};

int Map::sprite( int n ) const
{
	if ( !inBounds( n ) )
	{
		return 0;
	}
	else
	{
		return sprites_[ n ];
	}
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
		x = getXIndexForLoop( x );
	}
	
	if ( x < 0 || x >= ( int )( widthBlocks() ) || y < 0 || y > ( int )( heightBlocks() ) )
	{
		return -1;
	}
	else
	{
		return ( y * ( int )( widthBlocks() ) ) + x;
	}
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

void Map::deleteSprite( int where )
{
	if ( inBounds( where ) )
	{
		sprites_[ where ] = 0;
	}
};

bool Map::inBounds( int n ) const
{
	return n < blocks_.size();
};

void Map::update( EventSystem& events, const SpriteSystem& sprites, BlockSystem& blocks, const Camera& camera )
{
	updateLayers( events, blocks, camera );
	updateLoop( sprites );
	updateBGColor();
	changed_ = false;
};

void Map::updateLayers( EventSystem& events, BlockSystem& blocks, const Camera& camera )
{
	for ( auto b : backgrounds_ )
	{
		b->update( events, blocks, camera, *this );
	}
	for ( auto f : foregrounds_ )
	{
		f->update( events, blocks, camera, *this );
	}
};

void Map::updateLoop( const SpriteSystem& sprites )
{	
	if ( sprites.hero().rightPixels() > rightEdgeOfLoop() )
	{
		++current_loop_;
	}
	else if ( sprites.hero().xPixels() < leftEdgeOfLoop() )
	{
		--current_loop_;
	}
};

void Map::updateBGColor()
{
	// Default BG Color
	current_bg_ = palette_.bgN();
	
	// Only bother with calculating flash color if we have a flash color set.
	// Also, since we check flashing using remainder, we need to make sure we're 'bove or equal to the interval,
	// so it doesn't flash right @ the start.
	if ( lightning_flash_color_ != 0 && Main::stateFrame() >= FLASH_INTERVAL )
	{
		const int colors_in_between = palette_.bgN() - lightning_flash_color_ - 1;
		assert( colors_in_between >= 0 );

		 // * 2 refers to how it goes through inner colors twice: toward & 'way from flash.
		const int max_duration = FLASH_DURATION + ( ( colors_in_between * INNER_FLASH_DURATION ) * 2 );

		const int frame = Main::frame() % FLASH_INTERVAL;
		const int frames_going_toward_flash = ( colors_in_between * INNER_FLASH_DURATION ) + FLASH_DURATION;

		// Checks for frame going toward flash.
		if ( frame < frames_going_toward_flash )
		{
			for ( int i = colors_in_between; i >= 0; --i )
			{
				const int color = lightning_flash_color_ + i;
				const int duration = ( color == lightning_flash_color_ ) ? FLASH_DURATION : INNER_FLASH_DURATION;
				const int first_frame_check = ( colors_in_between - i ) * INNER_FLASH_DURATION;

				// Checks interval o' first_frame & duration.
				// duration - 1 'cause the first frame counts as part o' duration.
				if ( frame >= first_frame_check && frame <= first_frame_check + ( duration - 1 ) )
				{
					current_bg_ = color;
				}
			}
		}
		// Checks for frame going 'way from flash.
		else if ( frame < max_duration )
		{
			for ( int i = 1; i <= colors_in_between; ++i )
			{
				const int color = lightning_flash_color_ + i;
				const int first_frame_check = frames_going_toward_flash + ( ( i - 1 ) * INNER_FLASH_DURATION );

				if ( frame >= first_frame_check && frame <= first_frame_check + ( INNER_FLASH_DURATION - 1 ) )
				{
					current_bg_ = color;
				}
			}
		}
	}
};

void Map::renderBG( const Camera& camera )
{
	renderBGColor();
	for ( auto b : backgrounds_ )
	{
		b->render( camera );
	}
};

void Map::renderFG( const Camera& camera )
{
	for ( auto f : foregrounds_ )
	{
		f->render( camera );
	}
};

void Map::renderBGColor() const
{
	Render::colorCanvas( current_bg_ );
};

const Warp* Map::getWarp( int x_sub_pixels, int y_sub_pixels ) const
{
	for ( int i = 0; i < warps_.size(); ++i )
	{
		if ( warps_[ i ].inInterval( x_sub_pixels, y_sub_pixels ) )
		{
			return &warps_[ i ];
		}
	}
	return nullptr;
};

void Map::interact( Sprite& sprite, Camera& camera, Health& health )
{
	for ( auto b : backgrounds_ )
	{
		b->interact( sprite, health );
	}

	for ( auto f : foregrounds_ )
	{
		f->interact( sprite, health );
	}
	
	if ( scrollLoop() )
	{
		if
		(
			!sprite.hasType( Sprite::SpriteType::HERO ) &&
			!sprite.hasType( Sprite::SpriteType::CLOUD_PLATFORM )
		)
		{
			sprite.changeX( spriteLoopPosition( sprite.xSubPixels() ) );
		}
	}
};

const std::string& Map::tileset() const
{
	return tileset_;
};


/* LOOP
//===================================================*/

	bool Map::scrollLoop() const
	{
		return scroll_loop_width_ != 0;
	};

	int Map::scrollLoopWidthBlocks( int loop ) const
	{
		if ( loop == LOOP_CHANGE )
		{
			return widthBlocks();
		}
		else
		{
			return scroll_loop_width_;
		}
	};

	int Map::scrollLoopWidthPixels( int loop ) const
	{
		return Unit::BlocksToPixels( scrollLoopWidthBlocks( loop ) );
	};

	int Map::scrollLoopWidthBlocks() const
	{
		return scrollLoopWidthBlocks( current_loop_ );
	};

	int Map::scrollLoopWidthPixels() const
	{
		return scrollLoopWidthPixels( current_loop_ );
	};

	int Map::leftEdgeOfLoop( int loop ) const
	{
		if ( loop == 0 )
		{
			return 0;
		}
		else if ( loop < 0 )
		{
			return -scrollLoopWidthPixels( loop - 1 ) + leftEdgeOfLoop( loop + 1 );
		}
		else // loop > 0
		{
			return scrollLoopWidthPixels( loop - 1 ) + leftEdgeOfLoop( loop - 1 );
		}
	};

	int Map::leftEdgeOfLoop() const
	{
		return leftEdgeOfLoop( current_loop_ );
	};

	int Map::rightEdgeOfLoop( int loop ) const
	{
		return leftEdgeOfLoop( loop + 1 );
	};

	int Map::rightEdgeOfLoop() const
	{
		return rightEdgeOfLoop( current_loop_ );
	};

	int Map::getLoopPixels( int x ) const
	{
		int loop = 0;
		while ( x > rightEdgeOfLoop( loop ) )
		{
			++loop;
		}
		
		return loop;
	};

	int Map::getLoopBlocks( int x ) const
	{
		return getLoopPixels( Unit::BlocksToPixels( x ) );
	};

	int Map::getXIndexForLoop( int x ) const
	{
		if ( x < 0 )
		{
			while ( x < 0 )
			{
				x += scroll_loop_width_;
			}
			
			return x % scroll_loop_width_;
		}
		else if ( getLoopBlocks( x ) < LOOP_CHANGE )
		{
			return x % scroll_loop_width_;
		}
		else if ( getLoopBlocks( x ) == LOOP_CHANGE )
		{
			return ( x - Unit::PixelsToBlocks( leftEdgeOfLoop( LOOP_CHANGE ) ) ) % widthBlocks();
		}
		else
		{
			return ( x - Unit::PixelsToBlocks( rightEdgeOfLoop( LOOP_CHANGE ) ) ) % scroll_loop_width_;
		}
	};

	int Map::spriteLoopPosition( int x ) const
	{
		while ( x < Unit::PixelsToSubPixels( leftEdgeOfLoop() ) || x > Unit::PixelsToSubPixels( rightEdgeOfLoop() ) )
		{
			if ( x < Unit::PixelsToSubPixels( leftEdgeOfLoop() ) )
			{
				if ( leftEdgeOfLoop() <= 0 )
				{
					x += Unit::BlocksToSubPixels( scroll_loop_width_ );
				}
				else
				{
					x += Unit::PixelsToSubPixels( leftEdgeOfLoop() );
				}
			}

			if ( x > Unit::PixelsToSubPixels( rightEdgeOfLoop() ) )
			{
				if ( rightEdgeOfLoop() <= 0 )
				{
					x -= Unit::BlocksToSubPixels( scroll_loop_width_ );
				}
				else
				{
					x -= Unit::PixelsToSubPixels( rightEdgeOfLoop() );
				}
			}
		}
		
		return x;
	};