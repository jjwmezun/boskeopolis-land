#include "audio.hpp"
#include "input.hpp"
#include "inventory.hpp"
#include <fstream>
#include "main.hpp"
#include "level_title_state.hpp"
#include "message_state.hpp"
#include "mezun_exceptions.hpp"
#include "options_state.hpp"
#include "overworld_state.hpp"
#include "overworld_state.hpp"
#include "pause_state.hpp"
#include "level_select_state.hpp"
#include "render.hpp"
#include "time_start_state.hpp"

static constexpr int LOGO_WIDTH = 367;
static constexpr int LOGO_HEIGHT = 38;
static constexpr int LOGO_Y = 16;
static constexpr int CREATED_BY_Y = LOGO_HEIGHT + LOGO_Y + 8;
static constexpr int CREATED_BY_HEIGHT = 8;
static constexpr int OPTIONS_TOP_PADDING = 16;
static constexpr int OPTIONS_TOP_Y = CREATED_BY_Y + CREATED_BY_HEIGHT + OPTIONS_TOP_PADDING;
static constexpr int OPTION_WIDTH_MINIBLOCKS = 12;
enum class Option
{
	NEW,
	LOAD,
	OPTIONS,
	QUIT
};
static constexpr int OPTIONS_SIZE = 4;

LevelTitleState::LevelTitleState()
:
	GameState ( StateID::LEVEL_STATE ),
	inventory_screen_ (),
	level_ ( Level::getLevel( 1 ) ),
	events_ ( level_.startOn() ),
	camera_ ( { level_.cameraX(), level_.cameraY() } ),
	sprites_ ( level_.entranceX(), level_.entranceY() ),
	blocks_ ( level_.currentMap() ),
	health_ (),
	options_ ( { "New Game", "Load Game", "Options", "Quit" }, OPTION_WIDTH_MINIBLOCKS, OPTIONS_TOP_Y ),
    screen_texture_ ( Render::createRenderBox( Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS - 32 ) ),
    screen_src_ ( 0, 0, Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS - 32 ),
    screen_dest_ ( 0, 32, Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS - 32 ),
    curtain_rect_ ( 0, 0, 400, 192 ),
	logo_gfx_ ( "bosko_logo.png" ),
	logo_rect_ ( ( Unit::WINDOW_WIDTH_PIXELS - LOGO_WIDTH ) / 2, 16, LOGO_WIDTH, LOGO_HEIGHT ),
    paused_ ( false ),
	can_load_ ( false )
{
	Inventory::levelStart( 1 );
}

LevelTitleState::~LevelTitleState()
{
    SDL_DestroyTexture( screen_texture_ );
};

void LevelTitleState::stateUpdate()
{
    if ( paused_ )
    {
        options_.update();
        if ( Input::pressed( Input::Action::CONFIRM ) )
        {
            switch( ( Option )( options_.selection() ) )
            {
                case ( Option::NEW ):
                {
                    Main::changeState( std::unique_ptr<GameState> ( new OverworldState() ) );
                }
                break;

                case ( Option::LOAD ):
                {
                    Inventory::load();
                    Main::changeState( std::unique_ptr<GameState> ( new OverworldState() ) );
                }
                break;

                case ( Option::OPTIONS ):
                {
                    Main::pushState( std::make_unique<OptionsState> () );
                }
                break;

                case ( Option::QUIT ):
                {
                    Main::quit();
                }
                break;
            }
            Audio::playSound( Audio::SoundType::CONFIRM );
        }
    }
    else
    {
        blocks_.blocksFromMap( level_.currentMap(), camera_ );
        blocks_.update( events_ );

        if ( camera_.testPause() )
        {
            camera_.scroll( level_.currentMap() );
        }
        else
        {
            level_.currentMap().update( events_, sprites_, blocks_, camera_ );
            camera_.update();
            sprites_.update( camera_, level_.currentMap(), events_, blocks_, health_ );
            sprites_.interact( blocks_, level_, events_, camera_, health_ );
            sprites_.interactWithMap( level_.currentMap(), camera_, health_ );
            sprites_.spriteInteraction( camera_, blocks_, level_.currentMap(), health_, events_ );
            health_.update();
            events_.updateTrainer( level_, sprites_, camera_, blocks_ );

            if ( events_.paletteChanged() )
            {
                newPalette( events_.getPalette() );
            }

            if ( events_.timerStart() )
            {
                events_.unsetFlag();
                Main::pushState
                (
                    std::unique_ptr<GameState>
                    (
                        new TimeStartState( palette() )
                    )
                );
            }
        }

        if ( Input::held( Input::Action::CONFIRM ) )
        {
            paused_ = true;
        }
    }
};

void LevelTitleState::stateRender()
{
    if ( !paused_ )
    {
        Render::setRenderTarget( screen_texture_ );
        level_.currentMap().renderBG( camera_ );
        blocks_.render( level_.currentMap(), camera_, false );
        sprites_.render( camera_, false );
        blocks_.render( level_.currentMap(), camera_, true );
        sprites_.render( camera_, true );
        level_.currentMap().renderFG( camera_ );
        Render::releaseRenderTarget();
    }
    Render::renderObject( screen_texture_, screen_src_, screen_dest_ );
    Render::renderObject( "bg/stageplay.png", curtain_rect_, curtain_rect_ );
	logo_gfx_.render( logo_rect_, nullptr );

    if ( paused_ )
    {
	    options_.render();
    }
};

void LevelTitleState::init()
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
	newPalette( level_.currentMap().palette_ );
	sprites_.resetTrainer( level_, events_ );
	camera_.setPosition( level_.cameraX(), level_.cameraY() );
	Audio::changeSong( "title" );
    Audio::setTrainerModeOn();
};

void LevelTitleState::testPause()
{
	if ( Input::pressed( Input::Action::MENU ) && !events_.disable_pause_ )
	{
		Main::pushState
		(
			std::unique_ptr<GameState>
			(
				new PauseState( palette(), events_ )
			)
		);
	}
};

void LevelTitleState::backFromPop()
{
	Audio::changeSong( level_.currentMap().music_ );
};
