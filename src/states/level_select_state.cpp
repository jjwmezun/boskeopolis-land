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
#include "title_state.hpp"

static constexpr int DEFAULT_LEVEL = 0;
static constexpr int FUNDS_X = 32;
static constexpr int BG_COLOR = 1;
static constexpr int INVENTORY_Y = Unit::WINDOW_HEIGHT_PIXELS - 24;
static constexpr int START_Y = 40;

LevelSelectState::LevelSelectState( int level )
:
	GameState ( StateID::LEVEL_SELECT_STATE, { "Level Select", 1 } ),
	win_icon_gfx_ ( "tilesets/universal.png", 48, 8 ),
	diamond_gfx_ ( "tilesets/universal.png", 48, 0 ),
	gem_icon_gfx_ ( "charset.png", 32, 16 ),
	title_ ( "Select a Level, yo.", 0, 16, Text::FontColor::BLACK, Text::FontAlign::CENTER ),
	camera_ ( 0, 0, Unit::WINDOW_WIDTH_BLOCKS, Unit::WINDOW_HEIGHT_BLOCKS - 4, 0, START_Y ),
	selection_ ( level, Level::realLevelNum()-1, 0 ),
	INVENTORY_BG_DEST ( 0, INVENTORY_Y, Unit::WINDOW_WIDTH_PIXELS, 24 ),
	HEADER_BG_DEST ( 0, 0, Unit::WINDOW_WIDTH_PIXELS, START_Y ),
	GEM_ICON_DEST ( FUNDS_X - 16, INVENTORY_Y + 8, 8, 8 ),
	highlight_dest_ ( 16, START_Y, Unit::WINDOW_WIDTH_PIXELS - 32, 8 ),
	prev_level_ ( level ),
	delay_length_ ( 16 ),
	delay_ ( 0 ),
	show_challenges_ ( false )
{};

LevelSelectState::~LevelSelectState() {};

void LevelSelectState::stateUpdate()
{
	highlight_dest_.y = ( 8 * selection_() );

	if ( highlight_dest_.y + highlight_dest_.h > camera_.bottom() )
	{
		camera_.moveDown( 8 );
	}
	if ( highlight_dest_.y < camera_.y() )
	{
		camera_.moveUp( 8 );
	}

	highlight_dest_.y = camera_.relativeY( highlight_dest_ );

	if ( delay_ <= 0 )
	{
		if ( Input::held( Input::Action::MOVE_UP ) )
		{
			--selection_;
			delay_ = delay_length_;
		}
		if ( Input::held( Input::Action::MOVE_DOWN ) )
		{
			++selection_;
			delay_ = delay_length_;
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

		if ( Inventory::beenToLevel( level_ids_.at( selection_() ) ) && Input::held( Input::Action::CANCEL ) )
		{
			show_challenges_ = true;
		}
	}

	Inventory::update();

	if ( Inventory::beenToLevel( level_ids_.at( selection_() ) ) && Input::pressed( Input::Action::CONFIRM ) )
	{
		Inventory::setCurrentLevel( level_ids_.at( selection_() ) );
		Main::changeState( std::make_unique<OverworldState> () );
	}
	else if ( Input::pressed( Input::Action::MENU ) )
	{
		Main::popState();
	}
};

void LevelSelectState::stateRender()
{
	Render::colorCanvas();
	Render::renderRect( highlight_dest_, 6 );

	for ( int i = 0; i < level_ids_.size(); ++i )
	{
		Text::FontColor shade = Text::FontColor::BLACK;
		
		if ( levels_complete_.at( i ) )
		{
			shade = Text::FontColor::LIGHT_MID_GRAY;
		}
		else if ( i == selection_.value() )
		{
			if ( show_challenges_ )
			{
				shade = Text::FontColor::LIGHT_GRAY;
			}
			else
			{
				shade = Text::FontColor::WHITE;
			}
		}

		if ( i < level_names_.size() )
		{
			level_names_[ i ].render( &camera_, shade );
		}
		
		if ( i < gem_scores_.size() )
		{
			if ( i == selection_.value() && show_challenges_ )
			{
				gem_challenges_text_[ i ].render( &camera_, Text::FontColor::LIGHT_GRAY );
			}
			else
			{
				if ( gem_challenges_.at( i ) )
				{
					gem_scores_[ i ].render( &camera_, Text::FontColor::LIGHT_MID_GRAY );
				}
				else
				{
					gem_scores_[ i ].render( &camera_, shade );
				}
			}
		}

		if ( i < time_scores_.size() )
		{
			if ( i == selection_.value() && show_challenges_ )
			{
				time_challenges_text_[ i ].render( &camera_, Text::FontColor::LIGHT_GRAY );
			}
			else
			{
				if ( time_challenges_.at( i ) )
				{
					time_scores_[ i ].render( &camera_, Text::FontColor::LIGHT_MID_GRAY );
				}
				else
				{
					time_scores_[ i ].render( &camera_, shade );
				}
			}
		}
	}

	for ( int j = 0; j < win_icon_dests_.size(); ++j )
	{
		win_icon_gfx_.render( win_icon_dests_[ j ], &camera_ );
	}

	for ( int i = 0; i < diamond_icon_dests_.size(); ++i )
	{
		diamond_gfx_.render( diamond_icon_dests_[ i ], &camera_ );
	}
	
	for ( auto& k : gem_challenge_icon_dests_ )
	{
		win_icon_gfx_.render( k, &camera_ );
	}
	
	for ( auto& l : time_challenge_icon_dests_ )
	{
		win_icon_gfx_.render( l, &camera_ );
	}

	Render::renderRect( INVENTORY_BG_DEST, BG_COLOR );
	gem_icon_gfx_.render( GEM_ICON_DEST, nullptr );

	Text::FontColor shade = Text::FontColor::BLACK;

	if ( Inventory::totalFundsShown() < 0 )
	{
		shade = Text::FontColor::LIGHT_GRAY;
	}

	Text::renderNumber( Inventory::totalFundsShown(), FUNDS_X, INVENTORY_Y + 8, 9, shade );
	Text::renderText( Inventory::percentShown(), FUNDS_X + ( 30 * 8 ), INVENTORY_Y + 8 );

	Render::renderRect( HEADER_BG_DEST, BG_COLOR );
	title_.render();
};

void LevelSelectState::init()
{
	constexpr int X = 24;
	constexpr int END_X = Unit::WINDOW_WIDTH_PIXELS - 16;

	try
	{

		int reali = 0;

		for ( int i = 0; i < Level::NUM_O_LEVELS; ++i )
		{
			const std::string lvname = Inventory::levelName( i );

			if ( !mezun::isStringEmpty( lvname ) )
			{				
				level_names_.emplace_back( lvname, X + 8, ( 8 * reali ) );

				level_ids_.emplace_back( i );

				gem_scores_.emplace_back( Inventory::gemScore( i ), END_X - ( 11 * 8 ), ( 8 * reali ) );
				time_scores_.emplace_back( Inventory::timeScore( i ), END_X - ( 4 * 8 ), ( 8 * reali ) );
				gem_challenges_text_.emplace_back( Level::gemChallengeText( i ), END_X - ( 11 * 8 ), ( 8 * reali ) );
				time_challenges_text_.emplace_back( Level::timeChallengeText( i ), END_X - ( 4 * 8 ), ( 8 * reali ) );
				gem_challenges_.emplace_back( Inventory::gemChallengeBeaten( i ) );
				time_challenges_.emplace_back( Inventory::timeChallengeBeaten( i ) );

				if ( Inventory::victory( i ) )
				{
					win_icon_dests_.emplace_back( X - 8, ( 8 * reali ), 8, 8 );
				}

				if ( Inventory::haveDiamond( i ) )
				{
					diamond_icon_dests_.emplace_back( X, ( 8 * reali ), 8, 8 );
				}
				
				if ( Inventory::gemChallengeBeaten( i ) )
				{
					gem_challenge_icon_dests_.emplace_back( END_X - ( 12 * 8 ), ( 8 * reali ), 8, 8 );
				}
				
				if ( Inventory::timeChallengeBeaten( i ) )
				{
					time_challenge_icon_dests_.emplace_back( END_X - ( 5 * 8 ), ( 8 * reali ), 8, 8 );
				}
				
				if ( Inventory::levelComplete( i ) )
				{
					levels_complete_.emplace_back( true );
				}
				else
				{
					levels_complete_.emplace_back( false );
				}

				++reali;
			}
		}

		auto select = std::find( level_ids_.begin(), level_ids_.end(), prev_level_ );

		if ( select != level_ids_.end() )
		{
			selection_ = std::distance( level_ids_.begin(), select );
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