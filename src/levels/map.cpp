#include <algorithm>
#include <iostream>
#include <fstream>
#include "main.h"
#include "mezun_helpers.h"
#include "map.h"
#include "map_layer_tilemap.h"
#include "map_layer_water.h"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "unit.h"

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
		std::vector<int> layer2 = {};

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

		constexpr int BLOCKS_INDEX = 0;
		constexpr int SPRITES_INDEX = 1;
		constexpr int LAYER2_INDEX = 2;

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
						else if ( i == LAYER2_INDEX )
						{
							layer2.push_back( n.GetInt() );
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
		
		if ( !layer2.empty() )
		{
			backgrounds.emplace_back( new MapLayerTilemap( layer2, width, height ) );
		}



	// Misc. Features
	//=============================================		
		// Defaults

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

		// Test for features.
		if ( map_data.HasMember( "properties" ) )
		{
			for ( auto& prop : map_data[ "properties" ].GetObject() )
			{
				std::string name = prop.name.GetString();

				if ( mezun::areStringsEqual( name, "tileset" ) )
				{
					if ( prop.value.IsString() )
					{
						tileset = prop.value.GetString();
					}
				}

				if ( mezun::areStringsEqual( name, "palette" ) )
				{
					if ( prop.value.IsString() )
					{
						palette = prop.value.GetString();
					}
				}

				if ( mezun::areStringsEqual( name, "bg_color" ) )
				{
					if ( prop.value.IsInt() )
					{
						bg_color = prop.value.GetInt();
					}
				}

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

				if ( mezun::areStringsEqual( name, "water_effect_height" ) )
				{
					if ( prop.value.IsInt() )
					{
						foregrounds.emplace_back( std::make_unique<MapLayerWater> ( prop.value.GetInt() ) );
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
			blocks_work_offscreen
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
	bool blocks_work_offscreen
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
	blocks_work_offscreen_ ( blocks_work_offscreen )
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
	blocks_work_offscreen_ ( m.blocks_work_offscreen_ )
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
	blocks_work_offscreen_ ( c.blocks_work_offscreen_ )
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
	return std::min( widthBlocks() * heightBlocks(), (int)blocks_.size() );
};

int Map::spritesSize() const
{
	return std::min( widthBlocks() * heightBlocks(), (int)sprites_.size() );
};

int Map::block( int n ) const
{
	if ( !inBounds( n ) )
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
	if ( !inBounds( n ) )
	{
		return 0;
	}
	else
	{
		return sprites_.at( n );
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
	
	if ( x < 0 || x > widthBlocks() || y < 0 || y > heightBlocks() )
	{
		return -1;
	}
	else
	{
		return ( y * widthBlocks() ) + x;
	}
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
	return n >= 0 && n < blocks_.size();
};

void Map::update( EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, const Camera& camera )
{
	for ( int i = 0; i < backgrounds_.size(); ++i )
	{
		backgrounds_[ i ]->update( events, blocks, camera );
	}
	for ( int i = 0; i < foregrounds_.size(); ++i )
	{
		foregrounds_[ i ]->update( events, blocks, camera );
	}

	changed_ = false;
	
	if ( sprites.hero().rightPixels() > rightEdgeOfLoop() )
	{
		++current_loop_;
	}
	else if ( sprites.hero().xPixels() < leftEdgeOfLoop() )
	{
		--current_loop_;
	}
};

Palette Map::palette() const
{
	return palette_;
};

void Map::renderBG( Camera& camera )
{
	for ( int i = 0; i < backgrounds_.size(); ++i )
	{
		backgrounds_[ i ]->render( camera );
	}
};

void Map::renderFG( Camera& camera )
{
	for ( int i = 0; i < foregrounds_.size(); ++i )
	{
		foregrounds_[ i ]->render( camera );
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

void Map::interact( Sprite& sprite, Camera& camera )
{
	for ( auto b : backgrounds_ )
	{
		b->interact( sprite );
	}

	for ( auto f : foregrounds_ )
	{
		f->interact( sprite );
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