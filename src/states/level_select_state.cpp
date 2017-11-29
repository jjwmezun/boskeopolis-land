#include <algorithm>
#include "audio.hpp"
#include "main.hpp"
#include "input.hpp"
#include "inventory.hpp"
#include "level.hpp"
#include "level_state.hpp"
#include "level_select_state.hpp"
#include "message_state.hpp"
#include "mezun_exceptions.hpp"
#include "mezun_helpers.hpp"
#include "overworld_state.hpp"
#include "text_component_flashing.hpp"
#include "title_state.hpp"

static constexpr int CHAR_SIZE = Unit::PIXELS_PER_MINIBLOCK;
static constexpr int DEFAULT_LEVEL = 0;
static constexpr int BG_COLOR = 1;
static constexpr int X_PADDING = 8;
static constexpr int X = X_PADDING;
static constexpr int END_X = Unit::WINDOW_WIDTH_PIXELS - X_PADDING;
static constexpr int VICTORY_X = X;
static constexpr int DIAMOND_X = VICTORY_X + CHAR_SIZE;
static constexpr int LEVEL_NAME_X = DIAMOND_X + ( CHAR_SIZE * 2 );
static constexpr int TIME_LETTER_SIZE = 4;
static constexpr int GEM_DIGIT_LIMIT = 5;
static constexpr int TIME_CHALLENGE_TEXT_X = END_X - ( TIME_LETTER_SIZE * CHAR_SIZE );
static constexpr int TIME_ICON_X = TIME_CHALLENGE_TEXT_X - CHAR_SIZE;
static constexpr int GEM_CHALLENGE_TEXT_X = TIME_ICON_X - CHAR_SIZE - ( GEM_DIGIT_LIMIT * CHAR_SIZE );
static constexpr int GEM_ICON_X = GEM_CHALLENGE_TEXT_X - CHAR_SIZE;
static constexpr int FUNDS_ICON_X = X;
static constexpr int FUNDS_X = FUNDS_ICON_X + ( CHAR_SIZE * 2 );
static constexpr int INVENTORY_Y = Unit::WINDOW_HEIGHT_PIXELS - ( CHAR_SIZE * 2 );
static constexpr int START_Y = 40;

LevelSelectState::LevelSelectState( int level )
:
	GameState ( StateID::LEVEL_SELECT_STATE, { "Pale Green", 1 } ),
	win_icon_gfx_ ( "tilesets/universal.png", 48, 8 ),
	diamond_gfx_ ( "tilesets/universal.png", 48, 0 ),
	no_win_icon_gfx_ ( "tilesets/universal.png", 13 * 8, 7 * 8 ),
	no_diamond_gfx_ ( "tilesets/universal.png", 13 * 8, 6 * 8 ),
	no_win_icon_gfx_lighter_ ( "tilesets/universal.png", 7 * 8, 8 ),
	no_diamond_gfx_lighter_ ( "tilesets/universal.png", 7 * 8, 0 ),
	pts_ ( "\t", FUNDS_ICON_X, INVENTORY_Y, Text::FontColor::WHITE, Text::FontAlign::LEFT, Text::FontColor::BLACK ),
	percent_ ( Inventory::percentShown(), END_X - Unit::MiniBlocksToPixels( Inventory::percentShown().length() ), INVENTORY_Y, Text::FontColor::WHITE, Text::FontAlign::LEFT, Text::FontColor::BLACK ),
	gem_challenge_icon_gfx_ ( "tilesets/universal.png", 12 * 8, 7 * 8 ),
	time_challenge_icon_gfx_ ( "tilesets/universal.png", 40, 0 ),
	gem_challenge_icon_gfx_lighter_ ( "tilesets/universal.png", 14 * 8, 6 * 8 ),
	time_challenge_icon_gfx_lighter_ ( "tilesets/universal.png", 12 * 8, 6 * 8 ),
	bg_ ( "bg/checkerboard.png", 420, 271, 0, 0, 1, 1, 1, MapLayerImage::REPEAT_INFINITE, MapLayerImage::REPEAT_INFINITE, 250, -250 ),
	title_ ( "Select a Level, yo.", 0, 16, Text::FontColor::WHITE, Text::FontAlign::CENTER, Text::FontColor::BLACK ),
	camera_ ( 0, 0, Unit::WINDOW_WIDTH_BLOCKS, Unit::WINDOW_HEIGHT_BLOCKS - 4, 0, START_Y ),
	selection_ ( level ),
	INVENTORY_BG_DEST ( 0, INVENTORY_Y, Unit::WINDOW_WIDTH_PIXELS, 24 ),
	HEADER_BG_DEST ( 0, 0, Unit::WINDOW_WIDTH_PIXELS, START_Y ),
	highlight_dest_ ( 0, START_Y, Unit::WINDOW_WIDTH_PIXELS, 8 ),
	prev_level_ ( level ),
	delay_length_ ( 16 ),
	delay_ ( 0 ),
	first_entry_on_camera_ ( 0 ),
	end_entry_on_camera_ ( 0 ),
	show_challenges_ ( false ),
	show_challenges_prev_ ( false ),
	changed_selection_ ( true )
{
	Audio::changeSong( "level-select" );
};

LevelSelectState::~LevelSelectState() {};

void LevelSelectState::stateUpdate()
{
	updateInput();
	Inventory::update();

	if ( changed_selection_ )
	{
		updateHighlighter();
		updateCameraEntryLimits();
		updateEntries();
	}
	changed_selection_ = false;

	bg_.move( Unit::WINDOW_WIDTH_PIXELS, Render::window_box_ );
	for ( int i = first_entry_on_camera_; i < end_entry_on_camera_; ++i )
	{
		level_names_[ i ].update();
		gem_scores_[ i ].update();
		time_scores_[ i ].update();
	}
};

void LevelSelectState::updateEntries()
{
	for ( int i = first_entry_on_camera_; i < end_entry_on_camera_; ++i )
	{
		if ( i == selection_ && show_challenges_ )
		{
			gem_show_[ i ] = &gem_challenges_text_[ i ];
		}
		else
		{
			gem_show_[ i ] = &gem_scores_[ i ];
		}

		if ( i == selection_ && show_challenges_ )
		{
			time_show_[ i ] = &time_challenges_text_[ i ];
		}
		else
		{
			time_show_[ i ] = &time_scores_[ i ];
		}
		
		if ( !Inventory::gemChallengeBeaten( level_ids_[ i ] ) )
		{
			if ( i == selection_ )
			{
				gem_icon_show_[ i ] = &gem_challenge_icon_gfx_lighter_;
			}
			else
			{
				gem_icon_show_[ i ] = &gem_challenge_icon_gfx_;
			}
		}
		
		if ( !Inventory::timeChallengeBeaten( level_ids_[ i ] ) )
		{
			if ( i == selection_ )
			{
				time_icon_show_[ i ] = &time_challenge_icon_gfx_lighter_;
			}
			else
			{
				time_icon_show_[ i ] = &time_challenge_icon_gfx_;
			}
		}
		
		if ( !Inventory::victory( level_ids_[ i ] ) )
		{
			if ( i == selection_ )
			{
				win_icon_show_[ i ] = &no_win_icon_gfx_lighter_;
			}
			else
			{
				win_icon_show_[ i ] = &no_win_icon_gfx_;
			}
		}
		
		if ( !Inventory::haveDiamond( level_ids_[ i ] ) )
		{
			if ( i == selection_ )
			{
				diamond_icon_show_[ i ] = &no_diamond_gfx_lighter_;
			}
			else
			{
				diamond_icon_show_[ i ] = &no_diamond_gfx_;
			}
		}
	}
};

void LevelSelectState::updateInput()
{
	if ( delay_ <= 0 )
	{
		if ( Input::held( Input::Action::MOVE_UP ) )
		{
			--selection_;
			if ( selection_ < 0 ) { selection_ = 0; }
			else { Audio::playSound( Audio::SoundType::SELECT ); }
			delay_ = delay_length_;
			changed_selection_ = true;
		}
		if ( Input::held( Input::Action::MOVE_DOWN ) )
		{
			++selection_;
			if ( selection_ >= level_names_.size() ) { selection_ = level_names_.size() - 1; }
			else { Audio::playSound( Audio::SoundType::SELECT ); }
			delay_ = delay_length_;
			changed_selection_ = true;
		}
	}
	else
	{
		--delay_;
	}

	show_challenges_ = false;
	if ( Input::held( Input::Action::MOVE_UP ) || Input::held( Input::Action::MOVE_DOWN ) )
	{
		delay_length_ = 4;
	}
	else
	{
		delay_length_ = 8;

		if ( Inventory::beenToLevel( level_ids_.at( selection_ ) ) && Input::held( Input::Action::CANCEL ) )
		{
			show_challenges_ = true;
		}
	}
	if ( show_challenges_prev_ != show_challenges_ )
	{
		changed_selection_ = true;
	}
	show_challenges_prev_ = show_challenges_;
	
	if ( Input::pressed( Input::Action::CONFIRM ) )
	{
		if ( Inventory::beenToLevel( level_ids_.at( selection_ ) ) )
		{
			Inventory::setCurrentLevel( level_ids_.at( selection_ ) );
			Main::changeState( std::make_unique<OverworldState> () );
			Audio::playSound( Audio::SoundType::CONFIRM );
		}
		else
		{
			Audio::playSound( Audio::SoundType::CANCEL );
		}
	}
	else if ( Input::pressed( Input::Action::MENU ) )
	{
		Main::popState();
		Audio::playSound( Audio::SoundType::CANCEL );
	}
};

void LevelSelectState::updateHighlighter()
{
	highlight_dest_.y = ( 8 * selection_ );

	if ( highlight_dest_.y + highlight_dest_.h > camera_.bottom() )
	{
		camera_.moveDown( 8 );
	}
	if ( highlight_dest_.y < camera_.y() )
	{
		camera_.moveUp( 8 );
	}

	highlight_dest_.y = camera_.relativeY( highlight_dest_ );
};

void LevelSelectState::updateCameraEntryLimits()
{
	// Note: It's important that these happen in this order.
	first_entry_on_camera_ = calculateFirstEntryOnCamera();
	end_entry_on_camera_ = calculateEndEntryOnCamera();
};

int LevelSelectState::calculateFirstEntryOnCamera() const
{
	return Unit::PixelsToMiniBlocks( camera_.y() );
};

int LevelSelectState::calculateEndEntryOnCamera() const
{
	return std::min( ( int )( level_ids_.size() ), Unit::PixelsToMiniBlocks( camera_.heightPixels() ) + first_entry_on_camera_ );
};

void LevelSelectState::stateRender()
{
	bg_.render( Render::window_box_ );
	Render::renderRect( highlight_dest_, 6 );

	for ( int i = first_entry_on_camera_; i < end_entry_on_camera_; ++i )
	{
		win_icon_show_[ i ]->render( win_icon_dests_[ i ], &camera_ );
		diamond_icon_show_[ i ]->render( diamond_icon_dests_[ i ], &camera_ );
		level_names_[ i ].render( &camera_ );
		gem_icon_show_[ i ]->render( gem_challenge_icon_dests_[ i ], &camera_ );
		gem_show_[ i ]->render( &camera_ );
		time_icon_show_[ i ]->render( time_challenge_icon_dests_[ i ], &camera_ );
		time_show_[ i ]->render( &camera_ );
	}

	renderInventory();
	renderHeader();
};

void LevelSelectState::renderInventory() const
{
	pts_.render();
	Text::FontColor shade = Text::FontColor::WHITE;
	if ( Inventory::totalFundsShown() < 0 )
	{
		shade = Text::FontColor::DARK_MID_GRAY;
	}
	Text::renderNumber( Inventory::totalFundsShown(), FUNDS_X, INVENTORY_Y, 9, shade, nullptr, Text::FontColor::BLACK );
	percent_.render();
};

void LevelSelectState::renderHeader() const
{
	//Render::renderRect( HEADER_BG_DEST, BG_COLOR );
	title_.render();
};

void LevelSelectState::init()
{
	try
	{
		int reali = 0;

		for ( int i = 0; i < Level::MAX; ++i )
		{
			assert( reali <= ( int )( Level::realLevelNum() ) );
			const std::string lvname = Inventory::levelName( i );

			if ( !mezun::isStringEmpty( lvname ) )
			{
				if ( !Inventory::beenToLevel( i ) )
				{		
					if ( i != 1 && !Inventory::victory( i -1 ) )
					{
						continue;
					}
				}

				level_names_.emplace_back( lvname, LEVEL_NAME_X, Unit::MiniBlocksToPixels( reali ), Text::FontColor::WHITE, Text::FontAlign::LEFT, Text::FontColor::BLACK );

				level_ids_.emplace_back( i );

				gem_scores_.emplace_back( Inventory::gemScore( i ), GEM_CHALLENGE_TEXT_X, Unit::MiniBlocksToPixels( reali ), Text::FontColor::WHITE, Text::FontAlign::LEFT, Text::FontColor::BLACK );
				gem_show_.emplace_back( &gem_scores_[ reali ] );
				time_scores_.emplace_back( Inventory::timeScore( i ), TIME_CHALLENGE_TEXT_X, Unit::MiniBlocksToPixels( reali ), Text::FontColor::WHITE, Text::FontAlign::LEFT, Text::FontColor::BLACK );
				time_show_.emplace_back( &time_scores_[ reali ] );
				gem_challenges_text_.emplace_back( Level::gemChallengeText( i ), GEM_CHALLENGE_TEXT_X, Unit::MiniBlocksToPixels( reali ), Text::FontColor::LIGHT_GRAY );
				time_challenges_text_.emplace_back( Level::timeChallengeText( i ), TIME_CHALLENGE_TEXT_X, Unit::MiniBlocksToPixels( reali ), Text::FontColor::LIGHT_GRAY );

				const sdl2::SDLRect victory_rect = { VICTORY_X, Unit::MiniBlocksToPixels( reali ), CHAR_SIZE, CHAR_SIZE };
				const sdl2::SDLRect diamond_rect = { DIAMOND_X, Unit::MiniBlocksToPixels( reali ), CHAR_SIZE, CHAR_SIZE };
				const sdl2::SDLRect gem_challenge_rect = { GEM_ICON_X, Unit::MiniBlocksToPixels( reali ), CHAR_SIZE, CHAR_SIZE };
				const sdl2::SDLRect time_challenge_rect = { TIME_ICON_X, Unit::MiniBlocksToPixels( reali ), CHAR_SIZE, CHAR_SIZE };
				win_icon_dests_.emplace_back( victory_rect );
				diamond_icon_dests_.emplace_back( diamond_rect );
				gem_challenge_icon_dests_.emplace_back( gem_challenge_rect );
				time_challenge_icon_dests_.emplace_back( time_challenge_rect );

				if ( Inventory::victory( i ) )
				{
					win_icon_show_.emplace_back( &win_icon_gfx_ );
				}
				else
				{
					win_icon_show_.emplace_back( &no_win_icon_gfx_ );
				}

				if ( Inventory::haveDiamond( i ) )
				{
					diamond_icon_show_.emplace_back( &diamond_gfx_ );
				}
				else
				{
					diamond_icon_show_.emplace_back( &no_diamond_gfx_ );
				}
				
				if ( Inventory::gemChallengeBeaten( i ) )
				{
					gem_icon_show_.emplace_back( &win_icon_gfx_ );
					gem_scores_[ reali ].component_.reset( new TextComponentFlashing() );
				}
				else
				{
					gem_icon_show_.emplace_back( &gem_challenge_icon_gfx_ );
				}

				if ( Inventory::timeChallengeBeaten( i ) )
				{
					time_icon_show_.emplace_back( &win_icon_gfx_ );
					time_scores_[ reali ].component_.reset( new TextComponentFlashing() );
				}
				else
				{
					time_icon_show_.emplace_back( &time_challenge_icon_gfx_ );
				}
				
				if ( Inventory::levelComplete( i ) )
				{
					level_names_[ reali ].component_.reset( new TextComponentFlashing() );
				}

				++reali;
			}
		}
		auto select = std::find( level_ids_.begin(), level_ids_.end(), prev_level_ );

		if ( select != level_ids_.end() )
		{
			selection_ = std::distance( level_ids_.begin(), select );
		}
		else
		{
			selection_ = 0;
		}
	}
	catch ( const mezun::CantLoadLevels& e )
	{
		Main::pushState
		(
			std::move( MessageState::error
			(
				e.what(),
				false,
				std::make_unique<TitleState> (),
				false
			) )
		);
	}
};