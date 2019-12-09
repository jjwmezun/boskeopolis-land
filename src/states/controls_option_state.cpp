#include "audio.hpp"
#include "char_frame.hpp"
#include "controls_option_state.hpp"
#include "controls_option_prompt_state.hpp"
#include "localization.hpp"
#include "localization_language.hpp"
#include "main.hpp"
#include "options_state.hpp"
#include "render.hpp"
#include "screen_option_state.hpp"

static constexpr int OPTION_NAME_X = 16;
static constexpr int OPTION_NAME_START_Y = 48;

static int calculateMaxActionNameLength( const std::u32string* action_names )
{
	int max = 0;
	for ( int i = 0; i < Input::NUM_O_ACTIONS; ++i )
	{
		if ( action_names[ i ].size() > max )
		{
			max = action_names[ i ].size();
		}
	}
	return max;
};

static int calculateKeyNameX( const std::u32string* action_names )
{
	return OPTION_NAME_X + ( calculateMaxActionNameLength( action_names ) * CharFrame::SIZE_PIXELS ) + ( CharFrame::SIZE_PIXELS * 2 );
};

ControlsOptionState::ControlsOptionState()
:
	GameState( StateID::OPTIONS_STATE, { "Mountain Red", 2 }, false ),
	bg_ (),
	title_ (),
	text_ (),
	highlights_ (),
	selection_ ( Input::NUM_O_ACTIONS - 1 )
{};

ControlsOptionState::~ControlsOptionState()
{
	title_.destroy();
	text_.destroy();
	for ( int i = 0; i < Input::NUM_O_ACTIONS; i++ )
	{
		highlights_[ i ].destroy();
	}
};

void ControlsOptionState::stateUpdate()
{
	bg_.update();
	updateSelection();
	updateInput();
};

void ControlsOptionState::stateRender()
{
	bg_.render();
	renderOptions();
	title_.render();
};

void ControlsOptionState::init()
{
	WTextObj::generateTexture
	(
		title_,
		Localization::getCurrentLanguage().getControlsOptionsTitle(),
		0,
		16,
		WTextObj::Color::WHITE,
		Unit::WINDOW_WIDTH_PIXELS,
		WTextObj::Align::CENTER,
		WTextObj::Color::BLACK,
		16
	);

	//
	//  Create texture for all text in normal text.
	//
	//  & list o’ textures for each line with highlight.
	//
	//  When rendering, we will draw all the normal text
	//  & the selected line's highlighted texture o’er
	//  the text o’ that line.
	//
	///////////////////////////////////////////////////////

	const std::u32string* action_names = Localization::getCurrentLanguage().getControlsActionNames();
	const std::u32string* key_names = Input::generateKeyNames();
	const std::u32string* button_names = Input::generateButtonNames();
	const int KEY_NAME_X = calculateKeyNameX( action_names );

	text_.init();
	int y = OPTION_NAME_START_Y;
	for ( int i = 0; i < Input::NUM_O_ACTIONS; i++ )
	{
		// Draw line to normal text.
		text_.startDrawing();
		WTextObj action_name_obj
		{
			action_names[ i ],
			OPTION_NAME_X,
			y,
			WTextObj::Color::WHITE,
			Unit::WINDOW_WIDTH_PIXELS,
			WTextObj::Align::LEFT,
			WTextObj::Color::BLACK
		};
		action_name_obj.render();

		WTextObj key_name_obj
		{
			key_names[ i ],
			KEY_NAME_X,
			y,
			WTextObj::Color::WHITE,
			Unit::WINDOW_WIDTH_PIXELS,
			WTextObj::Align::LEFT,
			WTextObj::Color::BLACK
		};
		key_name_obj.render();

		WTextObj button_name_obj
		{
			button_names[ i ],
			Unit::WINDOW_WIDTH_PIXELS - 72,
			y,
			WTextObj::Color::WHITE,
			Unit::WINDOW_WIDTH_PIXELS,
			WTextObj::Align::LEFT,
			WTextObj::Color::BLACK
		};
		button_name_obj.render();

		// Generate highlight texture.
		highlights_[ i ].init();
		highlights_[ i ].startDrawing();
		action_name_obj.changeColor( WTextObj::Color::LIGHT_GRAY );
		action_name_obj.render();
		key_name_obj.changeColor( WTextObj::Color::LIGHT_GRAY );
		key_name_obj.render();
		button_name_obj.changeColor( WTextObj::Color::LIGHT_GRAY );
		button_name_obj.render();

		y += CharFrame::SIZE_PIXELS;
	}
	highlights_[ Input::NUM_O_ACTIONS - 1 ].endDrawing();
};

void ControlsOptionState::backFromPop()
{
	// todo: RESET NAMES
};

void ControlsOptionState::updateSelection()
{
	selection_.update();
}

void ControlsOptionState::updateInput()
{
	if ( Input::pressed( Input::Action::CANCEL ) )
	{
		Audio::playSound( Audio::SoundType::CANCEL );
		Main::changeState( std::make_unique<OptionsState> ( 1 ) );
	}
	else if ( Input::pressed( Input::Action::CONFIRM ) )
	{
		Audio::playSound( Audio::SoundType::CONFIRM );
		Main::pushState( std::make_unique<ControlsOptionPromptState> ( ( Input::Action )( selection_.selection() ) ) );
	}
};

void ControlsOptionState::renderOptions() const
{
	text_.render();
	highlights_[ selection_.selection() ].render();
};
