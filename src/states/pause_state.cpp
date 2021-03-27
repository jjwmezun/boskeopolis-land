#include "audio.hpp"
#include "main.hpp"
#include "input.hpp"
#include "inventory.hpp"
#include "frame.hpp"
#include "localization.hpp"
#include "localization_language.hpp"
#include "main.hpp"
#include "options_state.hpp"
#include "pause_state.hpp"
#include "render.hpp"
#include "wtext_obj.hpp"

static constexpr int WIDTH = 208;
static constexpr int HEIGHT = 72;
static constexpr int X = ( int )( ( double )( Unit::WINDOW_WIDTH_PIXELS - WIDTH ) / 2.0 );
static constexpr int Y = ( int )( ( double )( Unit::WINDOW_HEIGHT_PIXELS - HEIGHT ) / 2.0 );
static constexpr int ROW_HEIGHT = 16;
static constexpr int X_PADDING = 16;
static constexpr int TEXT_WIDTH = WIDTH - ( X_PADDING * 2 );

PauseState::PauseState( const Palette& palette, EventSystem& events, InventoryLevel& inventory_box, PaletteTransition palette_transition, Uint8 palette_transition_alpha )
:
	GameState( StateID::PAUSE_STATE, palette ),
	palette_transition_alpha_ ( palette_transition_alpha ),
	language_id_ ( Localization::getCurrentLanguageIndex() ),
	selection_ ( 0 ),
	events_ ( events ),
	inventory_box_ ( inventory_box ),
	bg_ ( WIDTH, HEIGHT, X, Y ),
	highlighted_text_ (),
	palette_transition_ ( palette_transition )
{
	Audio::pauseSong();
	Audio::playSound( Audio::SoundType::PAUSE );
};

PauseState::~PauseState()
{
	bg_.destroy();
	for ( int i = 0; i < NUMBER_OF_OPTIONS; ++i )
	{
		highlighted_text_[ i ].destroy();
	}
};

void PauseState::stateUpdate()
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
		switch ( ( PauseOption )( selection_() ) )
		{
			case ( PauseOption::CONTINUE ):
			{
				Audio::resumeSong();
				Main::popState();
			}
			break;
			case ( PauseOption::OPTIONS ):
			{
				Main::pushState( std::make_unique<OptionsState> (), true );
			}
			break;
			case ( PauseOption::QUIT ):
			{
				events_.quitLevel();
				Main::popState();
			}
			break;
		}
		Input::reset();
		Audio::playSound( Audio::SoundType::CONFIRM );
	}
	else if ( Input::pressed( Input::Action::MENU ) )
	{
		Input::reset();
		Audio::resumeSong();
		Main::popState();
		Audio::playSound( Audio::SoundType::CANCEL );
	}
};

void PauseState::stateRender()
{
	bg_.render();
	highlighted_text_[ selection_() ].render();
};

void PauseState::init()
{
	bg_.init();
	for ( int i = 0; i < NUMBER_OF_OPTIONS; ++i )
	{
		const int y = Y + ( ( i ) * ROW_HEIGHT + 10 ) - 4;
		highlighted_text_[ i ].changeSize( TEXT_WIDTH + ( X_PADDING * 2 ) - 10, HEIGHT );
		highlighted_text_[ i ].setX( X + 5 );
		highlighted_text_[ i ].setY( y );
		highlighted_text_[ i ].init();
	}
	generateTextures();
};

void PauseState::generateTextures()
{
	TextureBox shade_bg { WIDTH, HEIGHT, 0, 0 };
	if ( Render::hasTransPalette() )
	{
		Render::swapTransPalette();
		shade_bg.init();
		shade_bg.startDrawing();
		Render::clearScreenTransparency();
		renderBG();
		shade_bg.endDrawing();
		SDL_SetTextureAlphaMod( shade_bg.getTexture(), palette_transition_alpha_ );
		Render::swapTransPalette();
	}

	bg_.startDrawing();
	Render::clearScreenTransparency();
	renderBG();
	if ( Render::hasTransPalette() )
	{
		shade_bg.render();
	}
	bg_.endDrawing();

	if ( Render::hasTransPalette() )
	{
		shade_bg.destroy();
	}

	sdl2::SDLRect highlight_bg { 0, 0, WIDTH, ROW_HEIGHT };
	TextureBox shade_highlight { WIDTH, ROW_HEIGHT };
	if ( Render::hasTransPalette() )
	{
		shade_highlight.init();
	}
	for ( int i = 0; i < NUMBER_OF_OPTIONS; ++i )
	{
		if ( Render::hasTransPalette() )
		{
			Render::swapTransPalette();
			shade_highlight.startDrawing();
			Render::clearScreenTransparency();
			renderHighlight( highlight_bg, i );
			shade_highlight.endDrawing();
			SDL_SetTextureAlphaMod( shade_highlight.getTexture(), palette_transition_alpha_ );
			Render::swapTransPalette();
		}

		highlighted_text_[ i ].startDrawing();
		Render::clearScreenTransparency();
		renderHighlight( highlight_bg, i );
		if ( Render::hasTransPalette() )
		{
			shade_highlight.render();
		}
		highlighted_text_[ i ].endDrawing();
	}
	if ( Render::hasTransPalette() )
	{
		shade_highlight.destroy();
	}
};

void PauseState::renderBG()
{
	Frame frame = { 0, 0, WIDTH, ROW_HEIGHT * NUMBER_OF_OPTIONS + 12 };
	frame.render();
	for ( int i = 0; i < NUMBER_OF_OPTIONS; ++i )
	{
		const int y = ( i ) * ROW_HEIGHT + 10;
		WTextObj text{ getOptionName( ( PauseOption )( i ) ), 0, y, WTextCharacter::Color::BLACK, WIDTH, WTextObj::Align::LEFT, WTextCharacter::Color::__NULL, X_PADDING };
		text.render();
	}
};

void PauseState::renderHighlight( const sdl2::SDLRect& r, int i )
{
	Render::renderRect( r, 6 );
	WTextObj text{ getOptionName( ( PauseOption )( i ) ), 0, 0, WTextCharacter::Color::LIGHT_MID_GRAY, WIDTH, WTextObj::Align::LEFT, WTextCharacter::Color::__NULL, X_PADDING - 5, 4, WTextObj::VAlign::CENTER, ROW_HEIGHT };
	text.render();
};

std::u32string PauseState::getOptionName( PauseOption type )
{
	switch ( type )
	{
		case ( PauseOption::CONTINUE ):
		{
			return Localization::getCurrentLanguage().getPauseContinue();
		}
		break;
		case ( PauseOption::OPTIONS ):
		{
			return Localization::getCurrentLanguage().getPauseOptions();
		}
		break;
		case ( PauseOption::QUIT ):
		{
			return ( Inventory::victory() )
				? Localization::getCurrentLanguage().getPauseQuitBeaten()
				: Localization::getCurrentLanguage().getPauseQuitUnbeaten();
		}
		break;
	}
};

void PauseState::backFromPop()
{
	if ( palette_transition_.set )
	{
		Render::turnOnTransitionPalette( palette_transition_.from, palette_transition_.to );
	}
	if ( Localization::getCurrentLanguageIndex() != language_id_ )
	{
		generateTextures();
		inventory_box_.forceRerender();
	}
};