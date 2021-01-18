#include "audio.hpp"
#include "input.hpp"
#include "inventory.hpp"
#include "frame.hpp"
#include "level_state.hpp"
#include "level_tile_menu_state.hpp"
#include "localization.hpp"
#include "localization_language.hpp"
#include "main.hpp"
#include "wtext_obj.hpp"

static constexpr int ROW_HEIGHT = 8 + WTextCharacter::SIZE_PIXELS;
static constexpr int X_PADDING = 16;

static constexpr int calculateHeight( int size )
{
	return 24 + ROW_HEIGHT * size;
};

static constexpr int calculateWidth( int size )
{
	return 32 + WTextCharacter::SIZE_PIXELS * size;
};

static constexpr int calculateYFromHeight( int height )
{
	return ( int )( ( double )( Unit::WINDOW_HEIGHT_PIXELS - 40 - height ) / 2.0 );
};

static constexpr int calculateXFromWidth( int width )
{
	return ( int )( ( double )( Unit::WINDOW_WIDTH_PIXELS - width ) / 2.0 );
};

LevelTileMenuState::LevelTileMenuState( const Palette& palette, int level )
:
	GameState( StateID::LEVEL_TILE_MENU, palette ),
    level_ ( level ),
    selection_ ( 0 ),
	options_ (),
    bg_ (),
    highlighted_text_ ()
{
	options_.emplace_back( OptionType::PLAY );
	if ( LevelList::hasHardMode( ( unsigned int )( level ) ) )
	{
		options_.emplace_back( OptionType::HARD_MODE );
	}
	options_.emplace_back( OptionType::CANCEL );
};

LevelTileMenuState::~LevelTileMenuState()
{
	bg_.destroy();
	for ( int i = 0; i < options_.size(); ++i )
	{
		highlighted_text_[ i ].destroy();
	}
};

void LevelTileMenuState::stateUpdate()
{
	if ( Input::pressed( Input::Action::MOVE_DOWN ) )
	{
		++selection_;
		if ( selection_ >= options_.size() )
		{
			selection_ = 0;
		}
		Audio::playSound( Audio::SoundType::SELECT );
	}
	else if ( Input::pressed( Input::Action::MOVE_UP ) )
	{
		--selection_;
		if ( selection_ < 0 )
		{
			selection_ = options_.size() - 1;
		}
		Audio::playSound( Audio::SoundType::SELECT );
	}

	if ( Input::pressed( Input::Action::CONFIRM ) )
	{
		switch ( options_[ selection_ ] )
		{
			case ( OptionType::PLAY ):
			{
		        Audio::playSound( Audio::SoundType::CONFIRM );
				Main::changeState( std::make_unique<LevelState> ( level_, Difficulty::NORMAL, Inventory::heartUpgrades(), Inventory::haveOxygenUpgrade() ) );
			}
			break;
			case ( OptionType::HARD_MODE ):
			{
		        Audio::playSound( Audio::SoundType::CONFIRM );
				Main::changeState( std::make_unique<LevelState> ( level_, Difficulty::HARD, Inventory::heartUpgrades(), Inventory::haveOxygenUpgrade() ) );
			}
			break;
			case ( OptionType::CANCEL ):
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
	highlighted_text_[ selection_ ].render();
};

void LevelTileMenuState::init()
{
	// Generate BG texture.
	std::vector<WTextObj> option_lines {};
	int width = 32;
	for ( int i = 0; i < options_.size(); ++i )
	{
		const int ty = ( i + 1 ) * ROW_HEIGHT;
		option_lines.emplace_back( getOptionName( options_[ i ] ), 0, ty, WTextCharacter::Color::BLACK, Unit::WINDOW_WIDTH_PIXELS, WTextObj::Align::LEFT, WTextCharacter::Color::__NULL, X_PADDING );
		width = std::max( width, calculateWidth( option_lines[ option_lines.size() - 1 ].getNumberOfCharacters() ) );
	}
	const int height = calculateHeight( ( int )( options_.size() ) );
	const int y = calculateYFromHeight( height );
	const int x = calculateXFromWidth( width );
	bg_.setHeight( height );
	bg_.setY( y );
	bg_.setX( x );
	bg_.setWidth( width );
	bg_.init();
	bg_.startDrawing();
	Frame frame{ 0, 0, width, height };
	frame.render();
	for ( const auto& line : option_lines )
	{
		line.render();
	}
	bg_.endDrawing();

	// Generate highlight textures.
	for ( int i = 0; i < options_.size(); ++i )
	{
		const int hy = y + ( ( i + 1 ) * ROW_HEIGHT );
		highlighted_text_[ i ].changeSize( width - X_PADDING * 2, height );
		highlighted_text_[ i ].setX( x + X_PADDING );
		highlighted_text_[ i ].setY( hy );
		highlighted_text_[ i ].init();
		highlighted_text_[ i ].startDrawing();
		Render::clearScreenTransparency();
		WTextObj text{ getOptionName( options_[ i ] ), 0, 0, WTextCharacter::Color::LIGHT_MID_GRAY, width, WTextObj::Align::LEFT, WTextCharacter::Color::__NULL };
		text.render();
		highlighted_text_[ i ].endDrawing();
	}
};

std::u32string LevelTileMenuState::getOptionName( OptionType type )
{
	switch ( type )
	{
		case ( OptionType::PLAY ):
		{
			return Localization::getCurrentLanguage().getLevelTilePlay();
		}
		break;
		case ( OptionType::HARD_MODE ):
		{
			return Localization::getCurrentLanguage().getLevelTileHardMode();
		}
		break;
		case ( OptionType::CANCEL ):
		{
			return Localization::getCurrentLanguage().getLevelTileCancel();
		}
		break;
	}
};