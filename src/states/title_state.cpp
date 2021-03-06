#include "audio.hpp"
#include "choose_save_state.hpp"
#include "input.hpp"
#include "inventory.hpp"
#include "level_list.hpp"
#include "localization.hpp"
#include "localization_language.hpp"
#include <fstream>
#include "main.hpp"
#include "title_state.hpp"
#include "message_state.hpp"
#include "mezun_exceptions.hpp"
#include "options_state.hpp"
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
static constexpr int OPTIONS_TOP_Y = CREATED_BY_Y + CREATED_BY_HEIGHT + OPTIONS_TOP_PADDING + 32;
static constexpr int OPTION_WIDTH_MINIBLOCKS = 12;
static constexpr int OPTION_WIDTH_MINIBLOCKS_PIXELS = Unit::MiniBlocksToPixels( OPTION_WIDTH_MINIBLOCKS );
enum class Option
{
	PLAY,
	OPTIONS,
	QUIT
};
static constexpr int OPTIONS_SIZE = 4;

static constexpr int NUMBER_OF_TRAINER_LEVELS = 4;
static const std::string TRAINER_LEVELS[ NUMBER_OF_TRAINER_LEVELS ] = { "city-4", "woods-1", "factory-2", "city-1" };

//static int trainer_ = -1;
static const std::string& getRandomTrainerLevel()
{
    //++trainer_;
    //return ( trainer_ < 4 ) ? TRAINER_LEVELS[ trainer_ ] : TRAINER_LEVELS[ mezun::randInt( NUMBER_OF_TRAINER_LEVELS - 1, 0 ) ];
    return TRAINER_LEVELS[ mezun::randInt( NUMBER_OF_TRAINER_LEVELS - 1, 0 ) ];
};

TitleState::TitleState()
:
	GameState ( StateID::TITLE_STATE ),
    paused_ ( false ),
    screen_texture_ ( Render::createRenderBox( Unit::WINDOW_WIDTH_PIXELS, CAMERA_HEIGHT ) ),
	created_by_ (),
	options_ ( OptionSystem::generateVerticalOptionSystem( Localization::getCurrentLanguage().getTitleOptions(), OPTIONS_TOP_Y ) ),
    screen_src_ ( 0, 0, Unit::WINDOW_WIDTH_PIXELS, CAMERA_HEIGHT ),
    screen_dest_ ( 0, 64, Unit::WINDOW_WIDTH_PIXELS, CAMERA_HEIGHT ),
    curtain_ ( "bg/stageplay.png", { 0, 0, 400, 224 }, { 0, 68, 400, 224 } ),
	logo_ ( "bosko_logo.png", { 0, 0, LOGO_WIDTH, LOGO_HEIGHT }, { ( Unit::WINDOW_WIDTH_PIXELS - LOGO_WIDTH ) / 2, LOGO_Y, LOGO_WIDTH, LOGO_HEIGHT } ),
	skyscrapers_bg_ ( "bg/title_skyscrapers.png", 496, 72, 0, 0, 1, 1, 1, MapLayerImage::REPEAT_INFINITE, 0, -1000 ),
	light_gradient_bg_ ( "bg/light_gradient.png", 400, 80, 0, Unit::WINDOW_HEIGHT_PIXELS - 100, 1, 1, 1, MapLayerImage::REPEAT_INFINITE, 0 ),
	skyline_bg_ ( "bg/title-skyline.png", 224, 72, 0, 0, 1, 1, 1, MapLayerImage::REPEAT_INFINITE, 0, -500 ),
	cloud_bg_ ( "bg/city_clouds.png", 400, 72, 0, 0, 1, 1, 1, MapLayerImage::REPEAT_INFINITE, 0, -250, 0, 1, false, 128 ),
    level_ ( LevelList::getIDFromCodeName( getRandomTrainerLevel() ), Difficulty::NORMAL, Health::START_MAX_HP, false, 0, { Unit::WINDOW_WIDTH_PIXELS, CAMERA_HEIGHT, 0, 0, Unit::WINDOW_WIDTH_PIXELS, CAMERA_HEIGHT } )
{
};

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
                case ( Option::PLAY ):
                {
                    Main::changeState( std::unique_ptr<GameState> ( new ChooseSaveState() ) );
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
        level_.updateForTrainer();
		if ( level_.trainerPaletteChanged() )
		{
			newPalette( level_.getNewPalette() );
            level_.reRenderInventory();
		}
        if ( Input::pressed( Input::Action::CONFIRM ) )
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
    level_.renderLevel();
    Render::releaseRenderTarget();
};

void TitleState::renderHeader()
{
    Render::renderRect( { 0, 72, Unit::WINDOW_WIDTH_PIXELS, 2 }, 6, 128 );
    Render::renderRect( { 0, 0, Unit::WINDOW_WIDTH_PIXELS, 72 }, 2 );
	cloud_bg_.render( sdl2::SDLRect{ 0, 0, Unit::WINDOW_WIDTH_PIXELS, 72 } );
	logo_.render();
	skyline_bg_.render( sdl2::SDLRect{ 0, 0, Unit::WINDOW_WIDTH_PIXELS, 72 } );
	created_by_.render();
};

void TitleState::init()
{
    Inventory::load( Save::createNew( U"" ) );
	newPalette( level_.currentMap().palette_ );
	WTextObj::generateTexture( created_by_, Localization::getCurrentLanguage().getTitleCreatedBy(), 0, CREATED_BY_Y, WTextCharacter::Color::WHITE, Unit::WINDOW_WIDTH_PIXELS, WTextObj::Align::CENTER, WTextCharacter::Color::BLACK, Unit::PIXELS_PER_MINIBLOCK );
	options_.init();
    level_.initForTrainer();
	Audio::changeSong( "title" );
    Audio::setTrainerModeOn();
};