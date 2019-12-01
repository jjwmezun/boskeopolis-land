#include "audio.hpp"
#include <fstream>
#include "input.hpp"
#include "inventory.hpp"
#include "level.hpp"
#include "main.hpp"
#include "message_state.hpp"
#include "mezun_exceptions.hpp"
#include "options_state.hpp"
#include "overworld_state.hpp"
#include "render.hpp"
#include "text_info.hpp"
#include "title_state.hpp"
#include "wtext_obj.hpp"

static constexpr int LOGO_WIDTH = 367;
static constexpr int LOGO_HEIGHT = 38;
static constexpr int LOGO_Y = 16;
static constexpr int CREATED_BY_Y = LOGO_HEIGHT + LOGO_Y + 8;
static constexpr int CREATED_BY_HEIGHT = 8;
static constexpr int OPTIONS_TOP_PADDING = 16;
static constexpr int OPTIONS_TOP_Y = CREATED_BY_Y + CREATED_BY_HEIGHT + OPTIONS_TOP_PADDING;
static constexpr int OPTION_WIDTH_MINIBLOCKS = 12;

TitleState::TitleState()
:
	GameState( StateID::TITLE_STATE, { "Pale Purple", 2 } ),
	light_gradient_bg_ ( "bg/light_gradient.png", 400, 80, 0, Unit::WINDOW_HEIGHT_PIXELS - 100, 1, 1, 1, MapLayerImage::REPEAT_INFINITE, 0 ),
	skyline_bg_ ( "bg/skyline.png", 264, 224, 0, 48, 1, 1, 1, MapLayerImage::REPEAT_INFINITE, 0, -500 ),
	skyscrapers_bg_ ( "bg/title_skyscrapers.png", 248, 175, 0, Unit::WINDOW_HEIGHT_PIXELS - 175, 1, 1, 1, MapLayerImage::REPEAT_INFINITE, 0, -1000 ),
	cloud_bg_ ( "bg/city_clouds.png", 400, 112, 0, 0, 1, 1, 1, MapLayerImage::REPEAT_INFINITE, 0, -250, 0, 1, false, 128 ),
	logo_gfx_ ( "bosko_logo.png" ),
	options_ ( { "New Game", "Load Game", "Options", "Quit" }, OPTION_WIDTH_MINIBLOCKS, OPTIONS_TOP_Y ),
	created_by_ ( "Created by J.J.W. Mezun, 2017-2019", 0, CREATED_BY_Y, Text::FontColor::WHITE, Text::FontAlign::CENTER, Text::FontColor::BLACK ),
	logo_rect_ ( ( Unit::WINDOW_WIDTH_PIXELS - LOGO_WIDTH ) / 2, 16, LOGO_WIDTH, LOGO_HEIGHT ),
	can_load_ ( false )
{};

TitleState::~TitleState() {};

void TitleState::stateUpdate()
{
	options_.update();
	if ( Input::pressed( Input::Action::CONFIRM ) )
	{
		switch( ( Option )( options_.selection() ) )
		{
			case ( Option::NEW ):
				Main::changeState( std::unique_ptr<GameState> ( new OverworldState() ) );
			break;

			case ( Option::LOAD ):
				Inventory::load();
				Main::changeState( std::unique_ptr<GameState> ( new OverworldState() ) );
			break;

			case ( Option::OPTIONS ):
				Main::pushState( std::make_unique<OptionsState> () );
			break;

			case ( Option::QUIT ):
				Main::quit();
			break;
		}
		Audio::playSound( Audio::SoundType::CONFIRM );
	}

	cloud_bg_.move( Unit::WINDOW_WIDTH_PIXELS, Render::window_box_ );
	skyline_bg_.move( Unit::WINDOW_WIDTH_PIXELS, Render::window_box_ );
	skyscrapers_bg_.move( Unit::WINDOW_WIDTH_PIXELS, Render::window_box_ );
};

void TitleState::stateRender()
{
	Render::colorCanvas( 2 );
	logo_gfx_.render( logo_rect_, nullptr );
	light_gradient_bg_.render( Render::window_box_ );
	skyline_bg_.render( Render::window_box_ );
	skyscrapers_bg_.render( Render::window_box_ );
	cloud_bg_.render( Render::window_box_ );
	options_.render();
	//created_by_.render();
	WTextObj created_by = { TextInfo::getTitleCreatedBy(), 0, CREATED_BY_Y, WTextObj::Color::WHITE, Unit::WINDOW_WIDTH_PIXELS, WTextObj::Align::CENTER, WTextObj::Color::BLACK, Unit::PIXELS_PER_MINIBLOCK };
	created_by.render();
};

void TitleState::init()
{
	try
	{
		Level::buildLevelList();
	}
	catch ( const mezun::CantLoadLevelNames e )
	{
		Main::changeState
		(
			std::unique_ptr<MessageState> ( MessageState::errorMessage( e.what() ) )
		);
	}

	std::ifstream ifs( Main::savePath() );
	if ( ifs.is_open() )
	{
		can_load_ = true;
	}
	else
	{
		can_load_ = false;
	}
	ifs.close();
	Inventory::reset();
	Audio::changeSong( "title" );
};

void TitleState::backFromPop()
{
	Audio::changeSong( "title" );
};
