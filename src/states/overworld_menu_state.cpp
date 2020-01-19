#include "audio.hpp"
#include "input.hpp"
#include "level_select_state.hpp"
#include "main.hpp"
#include "options_state.hpp"
#include "overworld_menu_state.hpp"
#include "title_state.hpp"

static constexpr int BG_WIDTH_MINIBLOCKS = 24;
static constexpr int BG_HEIGHT_MINIBLOCKS = OverworldMenuState::NUM_O_OPTIONS * 2 + 3;
static constexpr int BG_WIDTH = Unit::MiniBlocksToPixels( BG_WIDTH_MINIBLOCKS );
static constexpr int BG_HEIGHT = Unit::MiniBlocksToPixels( BG_HEIGHT_MINIBLOCKS );
static constexpr int START_X = Unit::MiniBlocksToPixels( floor( Unit::WINDOW_WIDTH_MINIBLOCKS / 2 ) - floor( BG_WIDTH_MINIBLOCKS / 2 ) );
static constexpr int START_Y = Unit::MiniBlocksToPixels( floor( Unit::WINDOW_HEIGHT_MINIBLOCKS / 2 ) - floor( BG_HEIGHT_MINIBLOCKS / 2 ) );
static constexpr int OPTION_X = START_X + 8;

static std::string getOptionName( int i );
static int getOptionY( int i );
static TextObj getOption( int i );

OverworldMenuState::OverworldMenuState( const Palette& pal )
:
	GameState( StateID::OVERWORLD_MENU_STATE, pal ),
	option_text_
	({{
		getOption( 0 ),
		getOption( 1 ),
		getOption( 2 ),
		getOption( 3 ),
		getOption( 4 )
	}}),
	bg_
	(
		START_X,
		START_Y,
		BG_WIDTH,
		BG_HEIGHT
	),
	option_selection_ ( ( int )( Option::CONTINUE ) )
{
	Audio::playSound( Audio::SoundType::PAUSE );
};

OverworldMenuState::~OverworldMenuState() {};

void OverworldMenuState::stateUpdate()
{
	if ( Input::pressed( Input::Action::MENU ) )
	{
		Main::popState();
		Audio::playSound( Audio::SoundType::CANCEL );
	}

	if ( Input::pressed( Input::Action::MOVE_DOWN ) )
	{
		++option_selection_;
		Audio::playSound( Audio::SoundType::SELECT );
	}
	else if ( Input::pressed( Input::Action::MOVE_UP ) )
	{
		--option_selection_;
		Audio::playSound( Audio::SoundType::SELECT );
	}

	if ( option_selection_ >= NUM_O_OPTIONS )
	{
		option_selection_ = 0;
	}
	else if ( option_selection_ < 0 )
	{
		option_selection_ = NUM_O_OPTIONS - 1;
	}

	if ( Input::pressed( Input::Action::CONFIRM ) )
	{
		switch ( option_selection_ )
		{
			case ( ( int )( Option::CONTINUE ) ):
				Main::popState();
			break;

			case ( ( int )( Option::LIST ) ):
				Main::changeState( std::make_unique<LevelSelectState> ( 0 ) );
			break;

			case ( ( int )( Option::CAMERA ) ):
				Main::popState();
			break;

			case ( ( int )( Option::OPTIONS ) ):
				Main::pushState( std::make_unique<OptionsState> () );
			break;

			case ( ( int )( Option::QUIT ) ):
				Main::changeState( std::make_unique<TitleState> () );
			break;
		}
		Audio::playSound( Audio::SoundType::CONFIRM );
	}
};

void OverworldMenuState::stateRender()
{
	Render::renderRect( bg_, 6 );

	for ( int i = 0; i < NUM_O_OPTIONS; ++i )
	{
		option_text_[ i ].color_ = Text::FontColor::LIGHT_MID_GRAY;

		if ( option_selection_ == i )
		{
			option_text_[ i ].color_ = Text::FontColor::WHITE;
		}

		option_text_[ i ].render( nullptr );
	}
};

void OverworldMenuState::backFromPop()
{
	Audio::changeSong( "overworld" );
};

static std::string getOptionName( int i )
{
	switch ( i )
	{
		case ( 0 ): { return "Continue";    } break;
		case ( 1 ): { return "Level List";  } break;
		case ( 2 ): { return "Camera View"; } break;
		case ( 3 ): { return "Options";     } break;
		case ( 4 ): { return "Quit";        } break;
	}
};

static int getOptionY( int i )
{
	return START_Y + 16 + ( 16 * i );
};

static TextObj getOption( int i )
{
	return TextObj( getOptionName( i ), OPTION_X, getOptionY( i ), Text::FontColor::LIGHT_MID_GRAY );
};
