#include "audio.hpp"
#include "main.hpp"
#include <fstream>
#include "input.hpp"
#include "inventory.hpp"
#include "level.hpp"
#include "level_list.hpp"
#include "level_tile_menu_state.hpp"
#include "localization.hpp"
#include "main.hpp"
#include "mezun_json.hpp"
#include "overworld_state.hpp"
#include "overworld_menu_state.hpp"
#include "shop_state.hpp"
#include "unit.hpp"

static constexpr int frameToScreenSize( int value )
{
	return value - 12;
};

static constexpr int MOVE_PLAYER_FRAME_HEIGHT = Unit::WINDOW_HEIGHT_PIXELS - 40;
static constexpr int NUMBER_OF_PALETTES = 9;
static constexpr char PALETTES[ NUMBER_OF_PALETTES ][ 17 ] =
{
	"Overworld Red",
	"Overworld Green",
	"Overworld Yellow",
	"Overworld Brown",
	"Overworld Purple",
	"Overworld Cyan",
	"Overworld Orange",
	"Overworld Olive",
	"Overworld Black"
};

static constexpr int LEVEL_PALETTES[ Level::NUMBER_OF_LEVELS + 4 ] = 
{
	0, 1, 1, 3, 2, 2, 2, 4, 5, 6, 7, 7, 3, 3, 8, 0, // CYCLE 1
	8, 1, 0, 0, 0, 0, 0, 0, 8, 8, 7, 7, 3, 0, 0, 0, // CYCLE 2
	0, 0, 0, 0, 0, 0, 0, 0, 5, 6, 7, 7, 3, 0, 0, 0, // CYCLE 3
	0, 0, 0, 0, 0, 0, 0, 0, 5, 6, 7, 7, 3, 0, 0, 0, // CYCLE 4
	8, 8, // CHAMSBY & TREASURE TOWERS
	1, 1, 1, 1 // SHOP PALETTES
};

static const std::unordered_map<int, std::array<int, 2>> animated_frames_
{
	{ 134, { 134, 137 }},
	{ 135, { 135, 138 }},
	{ 136, { 136, 139 }},
	{ 150, { 150, 153 }},
	{ 152, { 152, 155 }},
	{ 166, { 166, 169 }},
	{ 167, { 167, 170 }},
	{ 168, { 168, 171 }}
};

static constexpr OWState determineBeginningState( ShowEventType event_type )
{
	switch ( event_type )
	{
		case ( ShowEventType::NONE   ): return OWState::MOVE_PLAYER; break;
		case ( ShowEventType::NORMAL ): return OWState::CAMERA_MOVES_TO_EVENT; break;
		case ( ShowEventType::SECRET ): return OWState::CAMERA_MOVES_TO_SECRET_EVENT; break;
	}
};

static const char* getSpacePaletteName( OWTile space );
static int getSpacePalette( OWTile space );

OverworldState::OverworldState( OWTile previous_space, ShowEventType show_event )
:
	GameState( StateID::OVERWORLD_STATE, { getSpacePaletteName( previous_space ), 2 } ),
	state_ ( determineBeginningState( show_event ) ),
	width_blocks_ ( 0 ),
	height_blocks_ ( 0 ),
	background_animation_timer_ ( 0 ),
	background_animation_frame_ ( 0 ),
	language_id_ ( Localization::getCurrentLanguageIndex() ),
	current_palette_ ( getSpacePalette( previous_space ) ),
	current_animation_frame_ ( 0 ),
	animation_timer_ (),
	object_on_ ( nullptr ),
	current_space_ ( previous_space ),
	previous_space_ ( previous_space ),
	objects_ (),
	tilemap_ (),
	screen_ ( Unit::WINDOW_WIDTH_PIXELS - 12, frameToScreenSize( Unit::WINDOW_HEIGHT_PIXELS ), 6, 6 ),
	bg_textures_ (),
	fg_textures_ (),
	main_frame_ ( 0, 0, Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS, -1 ),
	water_background_ ( "bg/overworld-water.png", { 0, 0, 400, 224 } ),
	background_ ( "bg/ow-bg.png", 16, 16 ),
	camera_ ( 800, 448 ),
	event_ (),
	camera_arrows_ (),
	hero_ ( 300, 300 ),
	level_tile_graphics_ (),
	inventory_ ()
{};

OverworldState::~OverworldState()
{
	for ( int i = 0; i < NUMBER_OF_LAYERS; ++i )
	{
		for ( int j = 0; j < MAX_ANIMATION_FRAMES; ++j )
		{
			bg_textures_[ i ][ j ].destroy();
			fg_textures_[ i ][ j ].destroy();
		}
	}
	screen_.destroy();
};

void OverworldState::stateUpdate()
{
	switch ( state_ )
	{
		case ( OWState::MOVE_PLAYER ):
		{
			testForMenuAction();
			camera_.adjust( hero_.getGraphicsBox() );
			hero_.update( tilemap_, camera_.getBox() );

			const OWTile level_momento = current_space_;
			current_space_ = OWTile::createNull();
			const DPoint& hero_position = hero_.getPosition();
			const int tile_x = ( int )( std::floor( ( hero_position.x ) / 16.0 ) );
			const int tile_y = ( int )( std::floor( ( hero_position.y ) / 16.0 ) );
			const int tile = tile_y * width_blocks_ + tile_x;
			const auto& seek = objects_.find( tile );
			object_on_ = nullptr;
			if ( seek != objects_.end() )
			{
				object_on_ = &seek->second;
				switch ( seek->second.getType() )
				{
					case ( OWObject::Type::LEVEL ):
					{
						current_space_ = OWTile::createLevel( seek->second.getLevelValue() );
					}
					break;
					case ( OWObject::Type::PALETTE ):
					{
						if ( current_palette_ != seek->second.getPaletteID() )
						{
							current_palette_ = seek->second.getPaletteID();
							newPalette( PALETTES[ current_palette_ ] );
							generateMapTextures();
						}
					}
					break;
					case ( OWObject::Type::SHOP ):
					{
						current_space_ = OWTile::createShop( 1 );
					}
					break;
				}
			}
			inventory_.update( current_space_ );

			if ( Input::pressed( Input::Action::CONFIRM ) )
			{
				if ( object_on_ != nullptr )
				{
					switch ( object_on_->getType() )
					{
						case ( OWObject::Type::LEVEL ):
						{
							Audio::playSound( Audio::SoundType::CONFIRM );
							Main::pushState( std::make_unique<LevelTileMenuState> ( palette_, object_on_->getLevelValue() ) );
						}
						break;
						case ( OWObject::Type::SHOP ):
						{
							Audio::playSound( Audio::SoundType::CONFIRM );
							Main::changeState( std::make_unique<ShopState> ( object_on_->getShopNumber() ) );
						}
						break;
					}
				}
			}
			else
			{
				if ( !current_space_.isNull() && current_space_ != level_momento )
				{
					Audio::playSound( Audio::SoundType::SELECT );
				}
			}
		}
		break;

		case ( OWState::MOVE_CAMERA ):
		{
			if ( Input::pressed( Input::Action::MENU ) )
			{
				state_ = OWState::CAMERA_MOVES_AUTOMATICALLY_TO_PLAYER;
			}
			else
			{
				camera_.move();
				hero_.updateGraphics( camera_.getBox() );
				camera_arrows_.update( camera_.getBox() );
			}
		}
		break;

		case ( OWState::CAMERA_MOVES_AUTOMATICALLY_TO_PLAYER ):
		{
			hero_.updateGraphics( camera_.getBox() );
			if ( camera_.moveAutomaticallyToTarget( hero_.getPosition() ) )
			{
				state_ = OWState::MOVE_PLAYER;
			}
		}
		break;

		case ( OWState::EVENT ):
		{
			const OWEvent::MessageBack message = event_.update( bg_tiles_, fg_tiles_ );
			switch ( message )
			{
				case ( OWEvent::MessageBack::SHOW_NEXT_LEVEL ):
				{
					level_tile_graphics_.showTile( camera_.getBox(), event_.getNextLevel() );
					Audio::playSound( Audio::SoundType::SELECT );
				}
				break;

				case ( OWEvent::MessageBack::EVENT_OVER ):
				{
					tilemap_.updateSolids( bg_tiles_, fg_tiles_ );
					state_ = OWState::CAMERA_MOVES_AUTOMATICALLY_TO_PLAYER;
				}
				break;

				case ( OWEvent::MessageBack::__NULL ):
				{
					// Do nothing.
				}
				break;

				default:
				{
					/*
					if ( OWEvent::testBG1Change( message ) )
					{
						generateBGMapTexture( 0 );
					}
					if ( OWEvent::testBG2Change( message ) )
					{
						generateBGMapTexture( 1 );
					}
					if ( OWEvent::testFG1Change( message ) )
					{
						generateFGMapTexture( 0 );
					}
					if ( OWEvent::testFG2Change( message ) )
					{
						generateFGMapTexture( 1 );
					}*/
					generateMapTextures();
				}
				break;
			}
		}
		break;

		case ( OWState::CAMERA_MOVES_TO_EVENT ):
		case ( OWState::CAMERA_MOVES_TO_SECRET_EVENT ):
		{
			hero_.updateGraphics( camera_.getBox() );
			if ( camera_.moveAutomaticallyToTarget( event_.getTargetPosition(), 4 ) )
			{
				state_ = OWState::EVENT;
			}
		}
		break;
	}

	for ( int i = 0; i < NUMBER_OF_LAYERS; ++i )
	{
		for ( int j = 0; j < MAX_ANIMATION_FRAMES; ++j )
		{
			bg_textures_[ i ][ j ].setX( -camera_.x() );
			bg_textures_[ i ][ j ].setY( -camera_.y() );
			fg_textures_[ i ][ j ].setX( -camera_.x() );
			fg_textures_[ i ][ j ].setY( -camera_.y() );
		}
	}
	updateBackgroundAnimation();
	updateBackgroundPosition();
	level_tile_graphics_.update( camera_.getBox() );
	hero_.updateAnimation();

	if ( animation_timer_.update() )
	{
		++current_animation_frame_;
	}
	background_.update();

	if ( state_ == OWState::MOVE_PLAYER )
	{
		if ( main_frame_.getHeight() > MOVE_PLAYER_FRAME_HEIGHT )
		{
			main_frame_.changeHeight( -2 );
			screen_.setHeight( frameToScreenSize( main_frame_.getHeight() ) );
		}
	}
	else
	{
		if ( main_frame_.getHeight() < Unit::WINDOW_HEIGHT_PIXELS )
		{
			main_frame_.changeHeight( 2 );
			screen_.setHeight( frameToScreenSize( main_frame_.getHeight() ) );
		}
	}
};

void OverworldState::stateRender()
{
	background_.render();
	screen_.startDrawing();
	Render::clearScreenTransparency();
		water_background_.render();
		for ( int i = 0; i < NUMBER_OF_LAYERS; ++i )
		{
			bg_textures_[ i ][ current_animation_frame_.value() ].render();
		}
		level_tile_graphics_.render();
		hero_.render( camera_ );
		for ( int i = 0; i < NUMBER_OF_LAYERS; ++i )
		{
			fg_textures_[ i ][ current_animation_frame_.value() ].render();
		}
	screen_.endDrawing();
	screen_.render();

	switch ( state_ )
	{
		case ( OWState::MOVE_PLAYER ):
		{
			if ( main_frame_.getHeight() == MOVE_PLAYER_FRAME_HEIGHT )
			{
				inventory_.render();
			}
		}
		break;

		case ( OWState::MOVE_CAMERA ):
		{
			camera_arrows_.render();
		}
	}
	main_frame_.render();
};

void OverworldState::init()
{
	screen_.init();
	newPalette( PALETTES[ current_palette_ ] );
	generateMap();
	tilemap_.init( width_blocks_, height_blocks_ );
	const int width = Unit::BlocksToPixels( width_blocks_ );
	const int height = Unit::BlocksToPixels( height_blocks_ );

	for ( int i = 0; i < NUMBER_OF_LAYERS; ++i )
	{
		for ( int j = 0; j < MAX_ANIMATION_FRAMES; ++j )
		{
			bg_textures_[ i ][ j ].changeSize( width, height );
			fg_textures_[ i ][ j ].changeSize( width, height );
			bg_textures_[ i ][ j ].init();
			fg_textures_[ i ][ j ].init();
		}
		bg_tiles_[ i ].reserve( width * height );
		fg_tiles_[ i ].reserve( width * height );
	}
	camera_.setBounds( width, height );

	generateSprites();
	if ( Inventory::specialLevelUnlocked( 1 ) && ( state_ != OWState::CAMERA_MOVES_TO_EVENT || !current_space_.isShop() ) )
	{
		OWEvent event;
		event.init( -2, width_blocks_, false );
		event.changeAllTiles( bg_tiles_, fg_tiles_ );
		level_tile_graphics_.showTile( camera_.getBox(), event.getNextLevel() );
	}
	for ( int i = 0; i < Level::NUMBER_OF_LEVELS; ++i )
	{
		const bool reveal_event =
		(
			state_ != OWState::CAMERA_MOVES_TO_EVENT ||
			!current_space_.isLevel() ||
			!current_space_.isLevelNumber( i )
		);
		if ( Inventory::victory( i ) && reveal_event )
		{
			OWEvent event;
			event.init( i, width_blocks_, false );
			event.changeAllTiles( bg_tiles_, fg_tiles_ );
			level_tile_graphics_.showTile( camera_.getBox(), event.getNextLevel() );
		}
		if ( Inventory::getSecretGoal( i ) && reveal_event )
		{
			OWEvent event;
			event.init( i, width_blocks_, true );
			event.changeAllTiles( bg_tiles_, fg_tiles_ );
			level_tile_graphics_.showTile( camera_.getBox(), event.getNextLevel() );
		}
	}
	generateMapTextures();

	camera_.center( hero_.getPosition() );
	hero_.updateGraphics( camera_.getBox() );
	inventory_.init();

	if ( state_ == OWState::CAMERA_MOVES_TO_EVENT )
	{
		event_.init( Inventory::currentLevel(), width_blocks_, false );
	}
	else if ( state_ == OWState::CAMERA_MOVES_TO_SECRET_EVENT )
	{
		event_.init( Inventory::currentLevel(), width_blocks_, true );
	}
	else if ( state_ == OWState::MOVE_PLAYER )
	{
		tilemap_.updateSolids( bg_tiles_, fg_tiles_ );
	}
	main_frame_.setHeight( ( state_ == OWState::MOVE_PLAYER ) ? MOVE_PLAYER_FRAME_HEIGHT : Unit::WINDOW_HEIGHT_PIXELS );
	screen_.setHeight( frameToScreenSize( main_frame_.getHeight() ) );

	Audio::changeSong( "overworld" );
};

void loadMap( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& data )
{

};

void OverworldState::testForMenuAction()
{
	if ( Input::pressed( Input::Action::MENU ) )
	{
		Main::pushState
		(
			std::make_unique<OverworldMenuState> ( palette(), &state_, current_space_, inventory_.getFlashColor() )
		);
	}
};

void OverworldState::backFromPop()
{
	Audio::changeSong( "overworld" );
	if ( state_ == OWState::MOVE_CAMERA )
	{
		Audio::playSound( Audio::SoundType::PAUSE );
	}
	if ( testLanguageHasChanged() )
	{
		inventory_.forceLevelNameRedraw();
		language_id_ = Localization::getCurrentLanguageIndex();
	}
};

void OverworldState::updateBackgroundAnimation()
{
	if ( background_animation_timer_ == 7 )
	{
		background_animation_timer_ = 0;
		if ( background_animation_frame_ == 15 )
		{
			background_animation_frame_ = 0;
		}
		else
		{
			++background_animation_frame_;
		}
	}
	else
	{
		++background_animation_timer_;
	}
};

void OverworldState::updateBackgroundPosition()
{
	water_background_.src_.x = ( camera_.x() + background_animation_frame_ ) % 16;
	water_background_.src_.y = camera_.y() % 16;
};

void OverworldState::generateSprites()
{
	sdl2::SDLRect src = { 0, 0, 16, 16 };
	sdl2::SDLRect dest = { 0, 0, 16, 16 };
	for ( int i = 0; i < sprites_tiles_.size(); ++i )
	{
		const int sprite_tile = sprites_tiles_[ i ] - 1;
		dest.x = Unit::BlocksToPixels( i % width_blocks_ );
		dest.y = Unit::BlocksToPixels( ( int )( std::floor( ( double )( i ) / ( double )( width_blocks_ ) ) ) );
		if ( sprite_tile > -1 )
		{
			if ( sprite_tile < 2048 )
			{
				throw std::runtime_error( "Invalid sprite in o’erworld map." );
			}
			if ( sprite_tile == 2048 )
			{
				if ( previous_space_.isNull() )
				{
					hero_.setPosition( dest.x + 8, dest.y + 8, camera_.getBox() );
				}
			}
			else if ( sprite_tile == 2049 )
			{
				setLevelSprite( Level::NUMBER_OF_LEVELS - 2, i, dest );
			}
			else if ( sprite_tile == 2050 )
			{
				setLevelSprite( Level::NUMBER_OF_LEVELS - 1, i, dest );
			}
			else if ( sprite_tile > 2063 && sprite_tile < 2063 + ( 16 * 4 ) )
			{
				const int theme = ( sprite_tile - 2064 ) % 16;
				const int cycle = ( int )( std::floor( ( double )( sprite_tile - 2064 ) / 16.0 ) );
				setLevelSprite( LevelList::getIDbyCycleAndTheme( cycle, theme ), i, dest );
			}
			else if ( sprite_tile >= 2160 && sprite_tile < 2160 + 6 )
			{
				const int shop_number = sprite_tile - 2160 + 1;
				if ( previous_space_.isShop() )
				{
					hero_.setPosition( dest.x + 8, dest.y + 8, camera_.getBox() );
				}
				objects_.insert( std::pair<int, OWObject>( i, OWObject::createShop( shop_number ) ) );
				level_tile_graphics_.add( dest, -2, true );
			}
			else if ( sprite_tile >= 2176 && sprite_tile < 2176 + NUMBER_OF_PALETTES )
			{
				const int palette_id = sprite_tile - 2176;
				objects_.insert( std::pair<int, OWObject> ( i, OWObject::createPaletteChanger( palette_id ) ) );
			}
		}
	}
};

void OverworldState::generateMapTextures()
{
	for ( int i = 0; i < NUMBER_OF_LAYERS; ++i )
	{
		generateBGMapTexture( i );
		generateFGMapTexture( i );
	}
};

void OverworldState::generateBGMapTexture( int n )
{
	for ( int j = 0; j < MAX_ANIMATION_FRAMES; ++j )
	{
		sdl2::SDLRect src = { 0, 0, 16, 16 };
		sdl2::SDLRect dest = { 0, 0, 16, 16 };
		bg_textures_[ n ][ j ].startDrawing();
		Render::clearScreenTransparency();
		for ( int i = 0; i < bg_tiles_[ n ].size(); ++i )
		{
			int bg_tile = bg_tiles_[ n ][ i ] - 1;
			const int x = i % width_blocks_;
			const int y = ( int )( std::floor( ( double )( i ) / ( double )( width_blocks_ ) ) );
			dest.x = Unit::BlocksToPixels( x );
			dest.y = Unit::BlocksToPixels( y );
			if ( bg_tile > -1 )
			{
				const auto& animated_frame = animated_frames_.find( bg_tile );
				if ( animated_frame != animated_frames_.end() )
				{
					bg_tile = animated_frame->second[ j ];
				}
				src.x = Unit::BlocksToPixels( bg_tile % 16 );
				src.y = Unit::BlocksToPixels( ( int )( std::floor( ( double )( bg_tile ) / 16.0 ) ) );
				Render::renderObject( "tilesets/ow.png", src, dest );
			}
		}
		bg_textures_[ n ][ j ].endDrawing();
	}
};

void OverworldState::generateFGMapTexture( int n )
{
	for ( int j = 0; j < MAX_ANIMATION_FRAMES; ++j )
	{
		sdl2::SDLRect src = { 0, 0, 16, 16 };
		sdl2::SDLRect dest = { 0, 0, 16, 16 };
		fg_textures_[ n ][ j ].startDrawing();
		Render::clearScreenTransparency();
		for ( int i = 0; i < fg_tiles_[ n ].size(); ++i )
		{
			const int fg_tile = fg_tiles_[ n ][ i ] - 1;
			dest.x = Unit::BlocksToPixels( i % width_blocks_ );
			dest.y = Unit::BlocksToPixels( ( int )( std::floor( ( double )( i ) / ( double )( width_blocks_ ) ) ) );
			if ( fg_tile > -1 )
			{
				src.x = Unit::BlocksToPixels( fg_tile % 16 );
				src.y = Unit::BlocksToPixels( ( int )( std::floor( ( double )( fg_tile ) / 16.0 ) ) );
				Render::renderObject( "tilesets/ow.png", src, dest );
			}
		}
		fg_textures_[ n ][ j ].endDrawing();
	}
};

void OverworldState::generateMap()
{
	const std::string path = Main::resourcePath() + "maps/land-ow.json";
	std::ifstream ifs( path );
	if( !ifs.is_open() )
	{
		throw std::runtime_error( "The o’erworld map data @ “" + path + "” is missing. Please redownload this game & try ’gain." );
	}
	rapidjson::IStreamWrapper ifs_wrap( ifs );
    rapidjson::Document document;
    document.ParseStream( ifs_wrap );
	ifs.close();

    if ( !document.IsObject() )
    {
		throw std::runtime_error( "The o’erworld map data is broken. Please redownload this game & try ’gain." );
    }

    const auto data = document.GetObject();
	if
	(
		!data.HasMember( "width" )  ||
		!data[ "width" ].IsInt()    ||
		!data.HasMember( "height" ) ||
		!data[ "height" ].IsInt()   ||
		!data.HasMember( "layers" ) ||
		!data[ "layers" ].IsArray()
	)
	{
		throw std::runtime_error( "The o’erworld map data is broken. Please redownload this game & try ’gain." );
	}
	width_blocks_ = data[ "width" ].GetInt();
	height_blocks_ = data[ "height" ].GetInt();

	for ( const auto& i : data[ "layers" ].GetArray() )
	{
		if ( i.IsObject() )
		{
			const auto& item = i.GetObject();
			const bool has_name = item.HasMember( "name" );
			const bool name_is_string = item[ "name" ].IsString();
			const bool has_data = item.HasMember( "data" );
			const bool data_is_array = item[ "data" ].IsArray();

			if ( has_name && name_is_string && has_data && data_is_array )
			{
				if ( strcmp( item[ "name" ].GetString(), "BG1" ) == 0 )
				{
					for ( const auto& bg_tile : item[ "data" ].GetArray() )
					{
						if ( bg_tile.IsInt() )
						{
							bg_tiles_[ 0 ].emplace_back( bg_tile.GetInt() );
						}
					}
				}
				else if ( strcmp( item[ "name" ].GetString(), "BG2" ) == 0 )
				{
					for ( const auto& bg_tile : item[ "data" ].GetArray() )
					{
						if ( bg_tile.IsInt() )
						{
							bg_tiles_[ 1 ].emplace_back( bg_tile.GetInt() );
						}
					}
				}
				else if ( strcmp( item[ "name" ].GetString(), "FG1" ) == 0 )
				{
					for ( const auto& fg_tile : item[ "data" ].GetArray() )
					{
						if ( fg_tile.IsInt() )
						{
							fg_tiles_[ 0 ].emplace_back( fg_tile.GetInt() );
						}
					}
				}
				else if ( strcmp( item[ "name" ].GetString(), "FG2" ) == 0 )
				{
					for ( const auto& fg_tile : item[ "data" ].GetArray() )
					{
						if ( fg_tile.IsInt() )
						{
							fg_tiles_[ 1 ].emplace_back( fg_tile.GetInt() );
						}
					}
				}
				else if ( strcmp( item[ "name" ].GetString(), "Sprites" ) == 0 )
				{
					for ( const auto& sprites_tile : item[ "data" ].GetArray() )
					{
						if ( sprites_tile.IsInt() )
						{
							sprites_tiles_.emplace_back( sprites_tile.GetInt() );
						}
					}
				}
			}
		}
	}

	if ( bg_tiles_[ 0 ].size() == 0 || bg_tiles_[ 1 ].size() == 0 )
	{
		throw std::runtime_error( "Missing background layer for o’erworld map." );
	}
	else if ( fg_tiles_[ 0 ].size() == 0 || fg_tiles_[ 1 ].size() == 0 )
	{
		throw std::runtime_error( "Missing blocks layer for o’erworld map." );
	}
	else if ( sprites_tiles_.size() == 0 )
	{
		throw std::runtime_error( "Missing sprite layer for o’erworld map." );
	}
};

bool OverworldState::testLanguageHasChanged() const
{
	return Localization::getCurrentLanguageIndex() != language_id_;
};

void OverworldState::setLevelSprite( int level_id, int i, const sdl2::SDLRect& dest )
{
	if ( previous_space_.isLevel() && previous_space_.isLevelNumber( level_id ) )
	{
		hero_.setPosition( dest.x + 8, dest.y + 8, camera_.getBox() );
	}
	objects_.insert( std::pair<int, OWObject>( i, OWObject::createLevel( level_id ) ) );
	const bool reveal = level_id == 0;
	level_tile_graphics_.add( dest, level_id, reveal );
};

static const char* getSpacePaletteName( OWTile space )
{
	return PALETTES[ getSpacePalette( space ) ];
};

static int getSpacePalette( OWTile space )
{
	return ( space.isShop() ) ? LEVEL_PALETTES[ Level::NUMBER_OF_LEVELS ]
		: ( space.isLevel() ) ? LEVEL_PALETTES[ space.getLevelNumber() ]
		: 0;
};