#include "audio.hpp"
#include "input.hpp"
#include "level_select_state.hpp"
#include "localization.hpp"
#include "localization_language.hpp"
#include "main.hpp"
#include "options_state.hpp"
#include "overworld_menu_state.hpp"
#include "title_state.hpp"

static constexpr int LEVEL_NAME_X = 24;
static constexpr int LEVEL_NAME_Y = 192;
static constexpr int LEVEL_NAME_W = 312;
static constexpr int LEVEL_NAME_H = 16;
static constexpr int BG_WIDTH = 208;
static constexpr int BG_HEIGHT = 104;
static constexpr int BG_X = ( int )( ( double )( Unit::WINDOW_WIDTH_PIXELS - BG_WIDTH ) / 2.0 );
static constexpr int BG_Y = ( int )( ( double )( Unit::WINDOW_HEIGHT_PIXELS - BG_HEIGHT ) / 2.0 );
static constexpr WTextCharacter::Color HIGHLIGHT_COLOR = WTextCharacter::Color::DARK_MID_GRAY;
static constexpr WTextCharacter::Color REGULAR_COLOR = WTextCharacter::Color::BLACK;

static constexpr int getOptionY( int i )
{
	return BG_Y + 16 + ( 16 * i );
};

OverworldMenuState::OverworldMenuState( const Palette& pal, OWState* camera_state, int level, int level_color )
:
	GameState( StateID::OVERWORLD_MENU_STATE, pal ),
	language_id_ ( Localization::getCurrentLanguageIndex() ),
	level_ ( level ),
	level_color_ ( level_color ),
	option_selection_ ( ( int )( Option::CONTINUE ) ),
	camera_state_ ( camera_state ),
	level_name_replacement_ (),
	options_text_ (),
	bg_
	(
		BG_X,
		BG_Y,
		BG_WIDTH,
		BG_HEIGHT
	),
	frame_ ( "bg/overworld_menu_frame.png", { 0, 0, BG_WIDTH, BG_HEIGHT }, { BG_X, BG_Y, BG_WIDTH, BG_HEIGHT } )
{
	generateOptionsText();
	Audio::playSound( Audio::SoundType::PAUSE );
};

OverworldMenuState::~OverworldMenuState()
{
	level_name_replacement_.destroy();
};

void OverworldMenuState::stateUpdate()
{
	if ( Input::pressed( Input::Action::MENU ) || Input::pressed( Input::Action::CANCEL ) )
	{
		Main::popState();
		Audio::playSound( Audio::SoundType::CANCEL );
	}

	if ( Input::pressed( Input::Action::MOVE_DOWN ) )
	{
		options_text_[ option_selection_.value() ].changeColor( REGULAR_COLOR );
		++option_selection_;
		Audio::playSound( Audio::SoundType::SELECT );
		options_text_[ option_selection_.value() ].changeColor( HIGHLIGHT_COLOR );
	}
	else if ( Input::pressed( Input::Action::MOVE_UP ) )
	{
		options_text_[ option_selection_.value() ].changeColor( REGULAR_COLOR );
		--option_selection_;
		Audio::playSound( Audio::SoundType::SELECT );
		options_text_[ option_selection_.value() ].changeColor( HIGHLIGHT_COLOR );
	}

	if ( Input::pressed( Input::Action::CONFIRM ) )
	{
		switch ( ( Option )( option_selection_.value() ) )
		{
			case ( Option::CONTINUE ):
			{
				Main::popState();
			}
			break;

			case ( Option::LIST ):
			{
				Main::pushState( std::make_unique<LevelSelectState> ( 0 ), true );
			}
			break;

			case ( Option::CAMERA ):
			{
				*camera_state_ = OWState::MOVE_CAMERA;
				Main::popState();
			}
			break;

			case ( Option::OPTIONS ):
			{
				Main::pushState( std::make_unique<OptionsState> (), true );
			}
			break;

			case ( Option::QUIT ):
			{
				Main::changeState( std::make_unique<TitleState> () );
			}
			break;
		}
		Audio::playSound( Audio::SoundType::CONFIRM );
	}
};

void OverworldMenuState::stateRender()
{
	frame_.render();
	level_name_replacement_.render();
	renderOptionsText();
};

void OverworldMenuState::renderOptionsText()
{
	for ( int i = 0; i < LocalizationLanguage::NUMBER_OF_OVERWORLD_MENU_OPTIONS; ++i )
	{
		options_text_[ i ].render();
	}
};

void OverworldMenuState::backFromPop()
{
	regenerateOptionsTextOnLanguageChange();
	Audio::changeSong( "overworld" );
};

void OverworldMenuState::init()
{
	level_name_replacement_.init();
};

void OverworldMenuState::regenerateOptionsTextOnLanguageChange()
{
	if ( language_id_ != Localization::getCurrentLanguageIndex() )
	{
		generateOptionsText();
		language_id_ = Localization::getCurrentLanguageIndex();

		if ( testOverworldShowsLevelName() )
		{
			generateReplacementLevelNameTexture();
		}
	}
};

void OverworldMenuState::generateOptionsText()
{
	const std::u32string* names = Localization::getCurrentLanguage().getOverworldMenuNames();
	for ( int i = 0; i < LocalizationLanguage::NUMBER_OF_OVERWORLD_MENU_OPTIONS; ++i )
	{
		const WTextCharacter::Color color = ( i == option_selection_.value() ) ? HIGHLIGHT_COLOR : REGULAR_COLOR;
		options_text_[ i ] = WTextObj( names[ i ], BG_X, getOptionY( i ), color, BG_WIDTH, WTextObj::Align::LEFT, WTextCharacter::Color::__NULL, 16 );
	}
};

bool OverworldMenuState::testOverworldShowsLevelName() const
{
	return level_ != -1;
};

void OverworldMenuState::generateReplacementLevelNameTexture()
{
	// Since we can’t directly access OverworldState data,
	// we just create a texture with replacement text
	// that we then draw o’er the OverworldState’s
	// level name graphics.
	level_name_replacement_.startDrawing();
	Render::renderRect( { LEVEL_NAME_X, LEVEL_NAME_Y, LEVEL_NAME_W, LEVEL_NAME_H }, 1 );
	WTextObj text{ Level::getLevelNames()[ level_ ], LEVEL_NAME_X, LEVEL_NAME_Y, ( WTextCharacter::Color )( level_color_ ), LEVEL_NAME_W };
	text.render();
	level_name_replacement_.endDrawing();
};