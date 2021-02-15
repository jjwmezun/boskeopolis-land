#include <algorithm>
#include <cassert>
#include <fstream>
#include "level_state.hpp"
#include "main.hpp"
#include "mezun_helpers.hpp"
#include "map.hpp"
#include "map_layer_lava.hpp"
#include "map_layer_lava_switch.hpp"
#include "map_layer_tilemap.hpp"
#include "map_layer_tilemap_image.hpp"
#include "map_layer_water.hpp"
#include "map_layer_water_back.hpp"
#include "map_layer_renderable.hpp"
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
	LayerType type = LayerType::__NULL;
	int number_start = 0;
	Unit::Layer layer_position = Unit::Layer::BEFORE_BG_1;
	if ( mezun::stringStartsWith( layer_name, "BlocksTexture" ) )
	{
		type = LayerType::BLOCKS_TEXTURE;
		number_start = 13;
		layer_position = Unit::Layer::BLOCKS_1;
	}
	else if ( mezun::stringStartsWith( layer_name, "Blocks" ) )
	{
		type = LayerType::BLOCKS;
		number_start = 6;
		layer_position = Unit::Layer::BLOCKS_1;
	}
	else if ( mezun::stringStartsWith( layer_name, "Sprites" ) )
	{
		type = LayerType::SPRITES;
		number_start = 7;
		layer_position = Unit::Layer::SPRITES_1;
	}
	else if ( mezun::stringStartsWith( layer_name, "Texture" ) )
	{
		type = LayerType::TEXTURE;
		number_start = 7;
		layer_position = Unit::Layer::BG_1;
	}
	else if ( mezun::stringStartsWith( layer_name, "FadeTexture" ) )
	{
		type = LayerType::FADE_TEXTURE;
		number_start = 11;
		layer_position = Unit::Layer::FG_2;
	}
	else if ( mezun::stringStartsWith( layer_name, "Fade" ) )
	{
		type = LayerType::FADE;
		number_start = 4;
		layer_position = Unit::Layer::FG_2;
	}
	else if ( mezun::stringStartsWith( layer_name, "Tiles" ) )
	{
		type = LayerType::TILES;
		number_start = 5;
		layer_position = Unit::Layer::FG_2;
	}
	else
	{
		return { LayerType::__NULL, layer_position };
	}

	const std::string number_string = layer_name.substr( number_start, layer_name.size() - 6 );
	try
	{
		if ( !number_string.empty() )
		{
			layer_position = Unit::IntToLayer( std::stoi( number_string ) );
		}
	}
	catch ( std::invalid_argument& e )
	{
		switch ( type )
		{
			case ( LayerType::BLOCKS_TEXTURE ):
			{
			}
			// Fallthru
			case ( LayerType::BLOCKS ):
			{
				const auto substr = layer_name.substr( number_start, 2 );
				if ( substr == "BG1" )
				{
					layer_position = Unit::Layer::BLOCKS_1;
				}
				else if ( substr == "FG1" )
				{
					layer_position = Unit::Layer::BLOCKS_2;
				}
				else if ( substr == "FG2" )
				{
					layer_position = Unit::Layer::FG_2;
				}
				else if ( substr == "BG2" )
				{
					layer_position = Unit::Layer::BG_2;
				}
				else
				{
					const auto substr = layer_name.substr( number_start, 2 );
					if ( substr == "BG" )
					{
						layer_position = Unit::Layer::BLOCKS_1;
					}
					else if ( substr == "FG" )
					{
						layer_position = Unit::Layer::BLOCKS_2;
					}
				}
			}
			break;
			default:
			{
				const auto substr = layer_name.substr( number_start, 3 );
				if ( substr == "BG1" )
				{
					layer_position = Unit::Layer::BG_1;
				}
				else if ( substr == "FG1" )
				{
					layer_position = Unit::Layer::FG_1;
				}
				else if ( substr == "FG2" )
				{
					layer_position = Unit::Layer::FG_2;
				}
				else if ( substr == "BG2" )
				{
					layer_position = Unit::Layer::BG_2;
				}
				else
				{
					const auto substr = layer_name.substr( number_start, 2 );
					if ( substr == "BG" )
					{
						layer_position = Unit::Layer::BG_1;
					}
					else if ( substr == "FG" )
					{
						layer_position = Unit::Layer::FG_1;
					}
				}
			}
			break;
		}
	}
	return { type, layer_position };
};

Map Map::mapFromPath
(
	std::string path,
	std::vector<std::shared_ptr<MapLayer>> layers,
	std::vector<Warp> warps
)
{

	// Setup
	//=============================================================

		std::vector<BlockLayer> blocks_layers {};
		std::vector<BlockLayer> new_layers {};
		std::vector<int> sprites {};

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
		bool blocks_work_offscreen = false;
		bool loop_sides = false;
		int water_effect_height = 0;
		std::string water_effect_type = "NORMAL";
		int wind_strength = 0;
		int gravity = Sprite::GRAVITY_TOP_SPEED_NORMAL;
		bool show_on_off = false;
		bool show_key = false;
		int lightning_flash_color = 0;
		std::string music = "";
		bool warp_on_fall = false;
		int ui_bg_color = 1;
		bool watery = false;
		bool oxygen = false;
		bool hide = false;
		bool auto_message = false;
		int lava_y = -1;
		int lava_y_alt = -1;
		int x_block_when_lava_rises_forever = -1;
		Camera::Type camera_type = Camera::Type::NORMAL;

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
				switch ( layer_info.type_ )
				{
					case ( LayerType::SPRITES ):
					{
						for ( auto& n : v[ "data" ].GetArray() )
						{
							sprites.push_back( n.GetInt() );
						}
					}
					break;
					default:
					{
						std::vector<int> tiles;
						for ( auto& n : v[ "data" ].GetArray() )
						{
							tiles.emplace_back( n.GetInt() );
						}
						new_layers.push_back({ layer_info.type_, layer_info.layer_position_, tiles });
					}
					break;
				}
			}
			++i;
		}

		for ( auto& new_layer : new_layers )
		{
			bool is_fade = false;
			switch ( new_layer.type_ )
			{
				case ( LayerType::BLOCKS_TEXTURE ):
				//Fallthru
				case ( LayerType::BLOCKS ):
				{
					blocks_layers.push_back( new_layer );
				}
				break;
				case ( LayerType::FADE ):
				{
					is_fade = true;
				}
				// Fallthru
				case ( LayerType::TILES ):
				{
					layers.emplace_back( new MapLayerTilemap( new_layer.blocks_, width, height, is_fade, new_layer.layer_position_ ) );
				}
				break;
				default:
				{
					layers.emplace_back( new MapLayerTilemapImage( new_layer.blocks_, width, height, new_layer.layer_position_ ) );
				}
				break;
			}
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
						slippery = ( ( strcmp( value.GetString(), "true" ) == 0 ) ? true : false );
					}
				}

				else if ( mezun::areStringsEqual( name, "auto_message" ) )
				{
					if ( value.IsBool() )
					{
						auto_message = value.GetBool();
					}
					else if ( value.IsString() )
					{
						auto_message = ( ( strcmp( value.GetString(), "true" ) == 0 ) ? true : false );
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
							if ( camera_type == Camera::Type::CENTER_Y || camera_type == Camera::Type::CENTER_BOTH )
							{
								camera_type = Camera::Type::CENTER_BOTH;
							}
							else
							{
								camera_type = Camera::Type::CENTER_X;
							}
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
							if ( camera_type == Camera::Type::CENTER_X || camera_type == Camera::Type::CENTER_BOTH )
							{
								camera_type = Camera::Type::CENTER_BOTH;
							}
							else
							{
								camera_type = Camera::Type::CENTER_Y;
							}
						}
						else if ( camera_priority_y_string.compare( "TOP" ) == 0 )
						{
							camera_type = Camera::Type::PRIORITIZE_TOP;
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

				else if ( mezun::areStringsEqual( name, "gravity" ) )
				{
					if ( value.IsInt() )
					{
						gravity = value.GetInt();
					}
				}

				else if ( mezun::areStringsEqual( name, "show_on_off" ) )
				{
					if ( value.IsBool() )
					{
						show_on_off = value.GetBool();
					}
				}

				else if ( mezun::areStringsEqual( name, "show_key" ) )
				{
					if ( value.IsBool() )
					{
						show_key = value.GetBool();
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

				else if ( mezun::areStringsEqual( name, "ui_bg_color" ) )
				{
					if ( value.IsInt() )
					{
						ui_bg_color = value.GetInt();
						assert( ui_bg_color >= 0 || ui_bg_color < Palette::COLOR_LIMIT );
					}
				}

				else if ( mezun::areStringsEqual( name, "scroll_lock" ) )
				{
					if ( value.IsBool() && value.GetBool() )
					{
						camera_type = Camera::Type::SCROLL_LOCK;
					}
				}

				else if ( mezun::areStringsEqual( name, "watery" ) )
				{
					watery = ( value.IsBool() && value.GetBool() );
				}

				else if ( mezun::areStringsEqual( name, "oxygen" ) )
				{
					oxygen = ( value.IsBool() && value.GetBool() );
				}

				else if ( mezun::areStringsEqual( name, "hide" ) )
				{
					hide = value.IsBool() && value.GetBool();
				}

				if ( mezun::areStringsEqual( name, "lava_y" ) && value.IsInt() )
				{
					lava_y = value.GetInt();
				}

				if ( mezun::areStringsEqual( name, "lava_y_switch_off" ) && value.IsInt() )
				{
					lava_y_alt = value.GetInt();
				}

				if ( mezun::areStringsEqual( name, "x_block_when_lava_rises_forever" ) && value.IsInt() )
				{
					x_block_when_lava_rises_forever = value.GetInt();
				}
			}
		}

		if ( lava_y > -1 )
		{
			if ( lava_y_alt > -1 )
			{
				layers.emplace_back( new MapLayerLavaSwitch( lava_y, lava_y_alt, x_block_when_lava_rises_forever, Unit::Layer::AFTER_FG_2 ) );
			}
			else
			{
				layers.emplace_back( new MapLayerLava( lava_y, Unit::Layer::AFTER_FG_2 ) );
			}
		}

		MapLayerWater* water_ptr = nullptr;
		if ( water_effect_type == "RISING" )
		{
			water_ptr = MapLayerWater::makeRisingWater( water_effect_height, Unit::Layer::AFTER_FG_2 );
		}
		else if ( water_effect_type == "SLUDGE" )
		{
			water_ptr = MapLayerWater::makeSludgeWater( water_effect_height, Unit::Layer::AFTER_FG_2 );
		}
		else if ( water_effect_height != 0 )
		{
			water_ptr = MapLayerWater::makeNormalWater( water_effect_height, Unit::Layer::AFTER_FG_2 );
		}

		if ( water_ptr != nullptr )
		{
			layers.emplace_back( new MapLayerWaterBack( water_ptr ) );
			layers.emplace_back( water_ptr );
		}


	// Send all data
	//=============================================

		return Map
		(
			blocks_layers,
			sprites,
			width,
			height,
			tileset,
			{ palette, bg_color },
			layers,
			warps,
			slippery,
			camera_limit_top,
			camera_limit_bottom,
			camera_limit_left,
			camera_limit_right,
			hero_type,
			scroll_loop_width,
			camera_type,
			blocks_work_offscreen,
			loop_sides,
			wind_strength,
			gravity,
			show_on_off,
			show_key,
			lightning_flash_color,
			music,
			warp_on_fall,
			ui_bg_color,
			watery,
			oxygen,
			hide,
			auto_message
		);
};

Map::Map
(
	std::vector<BlockLayer> blocks_layers,
	std::vector<int> sprites,
	int width,
	int height,
	std::string tileset,
	Palette palette,
	std::vector<std::shared_ptr<MapLayer>> other_layers,
	std::vector<Warp> warps,
	bool slippery,
	int top_limit,
	int bottom_limit,
	int left_limit,
	int right_limit,
	SpriteSystem::HeroType hero_type,
	int scroll_loop_width,
	Camera::Type camera_type,
	bool blocks_work_offscreen,
	bool loop_sides,
	int wind_strength,
	int gravity,
	bool show_on_off,
	bool show_key,
	int lightning_flash_color,
	std::string music,
	bool warp_on_fall,
	int ui_bg_color,
	bool watery,
	bool oxygen,
	bool hide,
	bool auto_message
)
:
	blocks_layers_ ( blocks_layers ),
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
	camera_type_ ( camera_type ),
	blocks_work_offscreen_ ( blocks_work_offscreen ),
	loop_sides_ ( loop_sides ),
	wind_strength_ ( wind_strength ),
	gravity_ ( gravity ),
	show_on_off_ ( show_on_off ),
	show_key_ ( show_key ),
	lightning_flash_color_ ( lightning_flash_color ),
	current_bg_ ( palette.bgN() ),
	music_ ( music ),
	warp_on_fall_ ( warp_on_fall ),
	ui_bg_color_ ( ui_bg_color ),
	watery_ ( watery ),
	oxygen_ ( oxygen ),
	hide_ ( hide ),
	other_layers_ ( other_layers ),
	auto_message_ ( auto_message )
{};

Map::~Map() noexcept {};

Map::Map( Map&& m ) noexcept
:
	blocks_layers_ ( m.blocks_layers_ ),
	sprites_ ( m.sprites_ ),
	width_ ( m.width_ ),
	height_ ( m.height_ ),
	tileset_ ( m.tileset_ ),
	palette_ ( m.palette_ ),
	other_layers_ ( m.other_layers_ ),
	warps_ ( m.warps_ ),
	slippery_ ( m.slippery_ ),
	top_limit_ ( m.top_limit_ ),
	bottom_limit_ ( m.bottom_limit_ ),
	left_limit_ ( m.left_limit_ ),
	right_limit_ ( m.right_limit_ ),
	hero_type_ ( m.hero_type_ ),
	scroll_loop_width_ ( m.scroll_loop_width_ ),
	current_loop_ ( m.current_loop_ ),
	camera_type_ ( m.camera_type_ ),
	blocks_work_offscreen_ ( m.blocks_work_offscreen_ ),
	loop_sides_ ( m.loop_sides_ ),
	wind_strength_ ( m.wind_strength_ ),
	gravity_ ( m.gravity_ ),
	changed_ ( m.changed_ ),
	show_on_off_ ( m.show_on_off_ ),
	show_key_ ( m.show_key_ ),
	lightning_flash_color_ ( m.lightning_flash_color_ ),
	current_bg_ ( m.current_bg_ ),
	music_ ( m.music_ ),
	warp_on_fall_ ( m.warp_on_fall_ ),
	ui_bg_color_ ( m.ui_bg_color_ ),
	watery_ ( m.watery_ ),
	oxygen_ ( m.oxygen_ ),
	hide_ ( m.hide_ ),
	auto_message_ ( m.auto_message_ )
{};

Map::Map( const Map& c )
:
	blocks_layers_ ( c.blocks_layers_ ),
	sprites_ ( c.sprites_ ),
	width_ ( c.width_ ),
	height_ ( c.height_ ),
	tileset_ ( c.tileset_ ),
	palette_ ( c.palette_ ),
	other_layers_ ( c.other_layers_ ),
	warps_ ( c.warps_ ),
	slippery_ ( c.slippery_ ),
	top_limit_ ( c.top_limit_ ),
	bottom_limit_ ( c.bottom_limit_ ),
	left_limit_ ( c.left_limit_ ),
	right_limit_ ( c.right_limit_ ),
	hero_type_ ( c.hero_type_ ),
	scroll_loop_width_ ( c.scroll_loop_width_ ),
	current_loop_ ( c.current_loop_ ),
	camera_type_ ( c.camera_type_ ),
	blocks_work_offscreen_ ( c.blocks_work_offscreen_ ),
	loop_sides_ ( c.loop_sides_ ),
	wind_strength_ ( c.wind_strength_ ),
	gravity_ ( c.gravity_ ),
	changed_ ( c.changed_ ),
	show_on_off_ ( c.show_on_off_ ),
	show_key_ ( c.show_key_ ),
	lightning_flash_color_ ( c.lightning_flash_color_ ),
	current_bg_ ( c.current_bg_ ),
	music_ ( c.music_ ),
	warp_on_fall_ ( c.warp_on_fall_ ),
	ui_bg_color_ ( c.ui_bg_color_ ),
	watery_ ( c.watery_ ),
	oxygen_ ( c.oxygen_ ),
	hide_ ( c.hide_ ),
	auto_message_ ( c.auto_message_ )
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
	return widthBlocks() * heightBlocks();
};

int Map::spritesSize() const
{
	return std::min( widthBlocks() * heightBlocks(), ( int )( sprites_.size() ) );
};

int Map::block( int layer, int n ) const
{
	if ( layer < 0 || layer > blocks_layers_.size() || n < 0 || n > blocks_layers_[ layer ].blocks_.size() )
	{
		return 0;
	}
	else
	{
		return blocks_layers_[ layer ].blocks_[ n ];
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

const std::vector<Map::BlockLayer>& Map::blocksLayers() const
{
	return blocks_layers_;
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

void Map::changeBlock( int layer, int where, int value )
{
	assert( layer >= 0 && layer < blocks_layers_.size() );
	if ( where >= 0 && where < blocks_layers_[ layer ].blocks_.size() )
	{
		blocks_layers_[ layer ].blocks_[ where ] = value;
	}
	changed_ = true;
};

void Map::deleteBlock( int layer, int where )
{
	changeBlock( layer, where, 0 );
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
	return n < blocks_layers_[ 0 ].blocks_.size();
};

void Map::update( LevelState& level_state )
{
	updateLayers( level_state );
	updateLoop( level_state.sprites() );
	updateBGColor();
	changed_ = false;
};

void Map::updateLayers( LevelState& level_state )
{
	for ( auto& layer : other_layers_ )
	{
		layer->update( level_state );
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

void Map::renderLayer( int n, const LevelState& level_state ) const
{
	if ( n >= 0 && n < other_layers_.size() )
	{
		other_layers_[ n ]->render( level_state.camera() );
	}
};

void Map::renderBGColor() const
{
	Render::colorCanvas( current_bg_ );
};

void Map::initOtherLayers( LevelState& level_state )
{
	for ( int i = 0; i < other_layers_.size(); ++i )
	{
		auto& layer = other_layers_[ i ];
		layer->renderable_id_ = level_state.addRenderable( std::unique_ptr<MapLayerRenderable> ( new MapLayerRenderable( i ) ), layer->layer_position_ );
	}
};

void Map::removeRenderableLayers( LevelState& level_state )
{
	for ( int i = 0; i < other_layers_.size(); ++i )
	{
		auto& layer = other_layers_[ i ];
		level_state.removeRenderable( layer->renderable_id_ );
	}
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

void Map::interact( Sprite& sprite, LevelState& level_state )
{
	if ( !sprite.is_dead_ )
	{
		for ( auto& layer : other_layers_ )
		{
			layer->interact( sprite, level_state );
		}
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