#include "audio.hpp"
#include "input.hpp"
#include "inventory.hpp"
#include "level_state.hpp"
#include "level_tile_menu_state.hpp"
#include "localization.hpp"
#include "localization_language.hpp"
#include "main.hpp"
#include "wtext_obj.hpp"

static constexpr int WIDTH = 112;
static constexpr int HEIGHT = 72;
static constexpr int X = ( int )( ( double )( Unit::WINDOW_WIDTH_PIXELS - WIDTH ) / 2.0 );
static constexpr int Y = ( int )( ( double )( Unit::WINDOW_HEIGHT_PIXELS - HEIGHT ) / 2.0 );
static constexpr int ROW_HEIGHT = 16;
static constexpr int X_PADDING = 16;
static constexpr int TEXT_WIDTH = WIDTH - ( X_PADDING * 2 );

LevelTileMenuState::LevelTileMenuState( const Palette& palette, int level )
:
	GameState( StateID::LEVEL_TILE_MENU, palette ),
    level_ ( level ),
    selection_ ( 0 ),
    bg_ ( WIDTH, HEIGHT, X, Y ),
    highlighted_text_ ()
{};

LevelTileMenuState::~LevelTileMenuState()
{
	bg_.destroy();
	for ( int i = 0; i < NUMBER_OF_OPTIONS; ++i )
	{
		highlighted_text_[ i ].destroy();
	}
};

void LevelTileMenuState::stateUpdate()
{
	if ( Input::pressed( Input::Action::MOVE_DOWN ) )
	{
		++selection_;
		Audio::playSound( Audio::SoundType::SELECT );
	}
	else if ( Input::pressed( Input::Action::MOVE_UP ) )
	{
		--selection_;
		Audio::playSound( Audio::SoundType::SELECT );
	}

	if ( Input::pressed( Input::Action::CONFIRM ) )
	{
		switch ( ( Option )( selection_() ) )
		{
			case ( Option::PLAY ):
			{
		        Audio::playSound( Audio::SoundType::CONFIRM );
				Main::changeState( std::make_unique<LevelState> ( level_, Difficulty::NORMAL, Inventory::heartUpgrades(), Inventory::haveOxygenUpgrade() ) );
			}
			break;
			case ( Option::HARD_MODE ):
			{
		        Audio::playSound( Audio::SoundType::CONFIRM );
				Main::changeState( std::make_unique<LevelState> ( level_, Difficulty::HARD, Inventory::heartUpgrades(), Inventory::haveOxygenUpgrade() ) );
			}
			break;
			case ( Option::CANCEL ):
			{
		        Audio::playSound( Audio::SoundType::CANCEL );
				Main::popState();
			}
			break;
		}
	}
	else if ( Input::pressed( Input::Action::MENU ) )
	{
		Input::reset();
		Audio::resumeSong();
		Main::popState();
		Audio::playSound( Audio::SoundType::CANCEL );
	}
};

void LevelTileMenuState::stateRender()
{
	bg_.render();
	highlighted_text_[ selection_() ].render();
};

void LevelTileMenuState::init()
{
	bg_.init();
	for ( int i = 0; i < NUMBER_OF_OPTIONS; ++i )
	{
		const int y = Y + ( ( i + 1 ) * ROW_HEIGHT );
		highlighted_text_[ i ].changeSize( TEXT_WIDTH, HEIGHT );
		highlighted_text_[ i ].setX( X + X_PADDING );
		highlighted_text_[ i ].setY( y );
		highlighted_text_[ i ].init();
	}
	bg_.startDrawing();
	Render::renderObject( "bg/level-tile-menu-frame.png", { 0, 0, WIDTH, HEIGHT }, { 0, 0, WIDTH, HEIGHT } );
	for ( int i = 0; i < NUMBER_OF_OPTIONS; ++i )
	{
		const int y = ( i + 1 ) * ROW_HEIGHT;
		WTextObj text{ getOptionName( ( Option )( i ) ), 0, y, WTextCharacter::Color::BLACK, WIDTH + ( X_PADDING * 2 ), WTextObj::Align::LEFT, WTextCharacter::Color::__NULL, X_PADDING };
		text.render();
	}
	bg_.endDrawing();
	for ( int i = 0; i < NUMBER_OF_OPTIONS; ++i )
	{
		highlighted_text_[ i ].startDrawing();
		Render::clearScreenTransparency();
		WTextObj text{ getOptionName( ( Option )( i ) ), 0, 0, WTextCharacter::Color::LIGHT_MID_GRAY, WIDTH, WTextObj::Align::LEFT, WTextCharacter::Color::__NULL };
		text.render();
		highlighted_text_[ i ].endDrawing();
	}
};

std::u32string LevelTileMenuState::getOptionName( Option type )
{
	switch ( type )
	{
		case ( Option::PLAY ):
		{
			return Localization::getCurrentLanguage().getLevelTilePlay();
		}
		break;
		case ( Option::HARD_MODE ):
		{
			return Localization::getCurrentLanguage().getLevelTileHardMode();
		}
		break;
		case ( Option::CANCEL ):
		{
			return Localization::getCurrentLanguage().getLevelTileCancel();
		}
		break;
	}
};