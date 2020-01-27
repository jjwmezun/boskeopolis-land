#include "audio.hpp"
#include "input.hpp"
#include "inventory.hpp"
#include "level.hpp"
#include "localization.hpp"
#include "localization_language.hpp"
#include <fstream>
#include "main.hpp"
#include "title_state.hpp"
#include "message_state.hpp"
#include "mezun_exceptions.hpp"
#include "options_state.hpp"
#include "overworld_state.hpp"
#include "overworld_state.hpp"
#include "pause_state.hpp"
#include "level_select_state.hpp"
#include "render.hpp"
#include "title_state.hpp"
#include "wtext_obj.hpp"

static constexpr int CAMERA_HEIGHT = Unit::WINDOW_HEIGHT_PIXELS - 64;
static constexpr int LOGO_WIDTH = 352;
static constexpr int LOGO_HEIGHT = 36;
static constexpr int LOGO_Y = 8;
static constexpr int CREATED_BY_Y = LOGO_HEIGHT + LOGO_Y + 8;
static constexpr int CREATED_BY_HEIGHT = 8;
static constexpr int OPTIONS_TOP_PADDING = 16;
static constexpr int OPTIONS_TOP_Y = CREATED_BY_Y + CREATED_BY_HEIGHT + OPTIONS_TOP_PADDING + 20;
static constexpr int OPTION_WIDTH_MINIBLOCKS = 12;
static constexpr int OPTION_WIDTH_MINIBLOCKS_PIXELS = Unit::MiniBlocksToPixels( OPTION_WIDTH_MINIBLOCKS );
enum class Option
{
	NEW,
	LOAD,
	OPTIONS,
	QUIT
};
static constexpr int OPTIONS_SIZE = 4;

static constexpr int NUMBER_OF_TRAINER_LEVELS = 4;
static const std::string TRAINER_LEVELS[ NUMBER_OF_TRAINER_LEVELS ] = { "city-1", "woods-1", "factory-2", "city-4" };

static const std::string& getRandomTrainerLevel()
{
    return TRAINER_LEVELS[ mezun::randInt( NUMBER_OF_TRAINER_LEVELS - 1, 0 ) ];
};

TitleState::TitleState()
:
	GameState ( StateID::TITLE_STATE ),
	can_load_ ( false ),
    paused_ ( false ),
    screen_texture_ ( Render::createRenderBox( Unit::WINDOW_WIDTH_PIXELS, CAMERA_HEIGHT ) ),
	created_by_ (),
	health_ (),
	options_ ( OptionSystem::generateVerticalOptionSystem( Localization::getCurrentLanguage().getTitleOptions(), OPTIONS_TOP_Y ) ),
    screen_src_ ( 0, 0, Unit::WINDOW_WIDTH_PIXELS, CAMERA_HEIGHT ),
    screen_dest_ ( 0, 64, Unit::WINDOW_WIDTH_PIXELS, CAMERA_HEIGHT ),
    curtain_ ( "bg/stageplay.png", { 0, 0, 400, 224 }, { 0, 68, 400, 224 } ),
	logo_ ( "bosko_logo.png", { 0, 0, LOGO_WIDTH, LOGO_HEIGHT }, { ( Unit::WINDOW_WIDTH_PIXELS - LOGO_WIDTH ) / 2, LOGO_Y, LOGO_WIDTH, LOGO_HEIGHT } ),
	skyscrapers_bg_ ( "bg/title_skyscrapers.png", 496, 72, 0, 0, 1, 1, 1, MapLayerImage::REPEAT_INFINITE, 0, -1000 ),
	light_gradient_bg_ ( "bg/light_gradient.png", 400, 80, 0, Unit::WINDOW_HEIGHT_PIXELS - 100, 1, 1, 1, MapLayerImage::REPEAT_INFINITE, 0 ),
	skyline_bg_ ( "bg/title-skyline.png", 224, 72, 0, 0, 1, 1, 1, MapLayerImage::REPEAT_INFINITE, 0, -500 ),
	cloud_bg_ ( "bg/city_clouds.png", 400, 72, 0, 0, 1, 1, 1, MapLayerImage::REPEAT_INFINITE, 0, -250, 0, 1, false, 128 ),
	level_ ( Level::getLevel( Level::getIDFromCodeName( getRandomTrainerLevel() ) ) ),
	camera_ ( level_.cameraX(), level_.cameraY(), Unit::WINDOW_WIDTH_PIXELS, CAMERA_HEIGHT, 0, 0, Unit::WINDOW_WIDTH_PIXELS, CAMERA_HEIGHT ),
	blocks_ ( level_.currentMap() ),
	sprites_ ( level_.entranceX(), level_.entranceY() ),
	events_ ( level_.startOn() ),
    inventory_screen_ ()
{};

TitleState::~TitleState()
{
    SDL_DestroyTexture( screen_texture_ );
    created_by_.destroy();
};

void TitleState::stateUpdate()
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
                    Main::changeState( std::unique_ptr<GameState> ( new OverworldState( Inventory::currentLevel(), false, true ) ) );
                }
                break;

                case ( Option::LOAD ):
                {
                    Inventory::load();
                    Main::changeState( std::unique_ptr<GameState> ( new OverworldState( Inventory::currentLevel(), false, true ) ) );
                }
                break;

                case ( Option::OPTIONS ):
                {
                    Main::pushState( std::make_unique<OptionsState> ( true ), true );
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
        }

        if ( Input::held( Input::Action::CONFIRM ) )
        {
            paused_ = true;
            Audio::setTrainerModeOff();
            Audio::playSound( Audio::SoundType::CONFIRM );
        }
    }
	cloud_bg_.move( Unit::WINDOW_WIDTH_PIXELS, Render::window_box_ );
	skyline_bg_.move( Unit::WINDOW_WIDTH_PIXELS, Render::window_box_ );
	skyscrapers_bg_.move( Unit::WINDOW_WIDTH_PIXELS, Render::window_box_ );
};

void TitleState::stateRender()
{
    if ( !paused_ )
    {
        generateLevelTexture();
    }
    renderLevel();
    curtain_.render();
    renderHeader();

    if ( paused_ )
    {
	    options_.render();
    }
};

void TitleState::renderLevel()
{
    Render::colorCanvas( palette().bgN() );
    Render::renderObject( screen_texture_, screen_src_, screen_dest_ );
};

void TitleState::generateLevelTexture()
{
    Render::setRenderTarget( screen_texture_ );
    Render::clearScreenTransparency();
    Render::colorCanvas( palette().bgN() );
    level_.currentMap().renderBG( camera_ );
    blocks_.render( level_.currentMap(), camera_, false );
    sprites_.render( camera_, false );
    blocks_.render( level_.currentMap(), camera_, true );
    sprites_.render( camera_, true );
    level_.currentMap().renderFG( camera_ );
    Render::releaseRenderTarget();
};

void TitleState::renderHeader()
{
    Render::renderRect( { 0, 72, Unit::WINDOW_WIDTH_PIXELS, 2 }, 6, 128 );
    Render::renderRect( { 0, 0, Unit::WINDOW_WIDTH_PIXELS, 72 }, 2 );
	cloud_bg_.render( sdl2::SDLRect{ 0, 0, Unit::WINDOW_WIDTH_PIXELS, 72 } );
	logo_.render();
	skyline_bg_.render( sdl2::SDLRect{ 0, 0, Unit::WINDOW_WIDTH_PIXELS, 72 } );
	//skyscrapers_bg_.render( sdl2::SDLRect{ 0, 0, Unit::WINDOW_WIDTH_PIXELS, 72 } );
	created_by_.render();
};

void TitleState::init()
{
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
	WTextObj::generateTexture( created_by_, Localization::getCurrentLanguage().getTitleCreatedBy(), 0, CREATED_BY_Y, WTextCharacter::Color::WHITE, Unit::WINDOW_WIDTH_PIXELS, WTextObj::Align::CENTER, WTextCharacter::Color::BLACK, Unit::PIXELS_PER_MINIBLOCK );
	options_.init();
	sprites_.resetTrainer( level_, events_ );
	camera_.setPosition( level_.cameraX(), level_.cameraY() );
	Audio::changeSong( "title" );
    Audio::setTrainerModeOn();
};