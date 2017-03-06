


// Name
//===================================
//
// LevelState
//


// DEPENDENCIES
//===================================

    #include "game.h"
    #include "goal.h"
    #include "level_state.h"
    #include "message_state.h"
	#include "mezun_exceptions.h"
    #include "pause_state.h"
    #include "level_message_state.h"
    #include "level_select_state.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    LevelState::LevelState( EventSystem events, InventoryLevel inventory, int lvname, Game& game )
    try :
        GameState ( StateID::LEVEL_STATE ),
        inventory_ ( inventory ),
        events_ ( events ),
        level_ ( Level::getLevel( lvname ) ),
        camera_ ( { level_.cameraX(), level_.cameraY() } ),
        sprites_ ( level_.entranceX(), level_.entranceY() ),
		blocks_ ( level_.currentMap() )
    {
    }
	catch ( const mezun::CantLoadTileset& e )
	{
		game.pushState
		(
			std::move( MessageState::error
			(
				e.what(),
				false,
				std::unique_ptr<GameState> ( LevelSelectState::continueLevelSelect( events_, inventory_, level_.id() ) ),
				false
			) )
		);
	};

    LevelState::~LevelState() {};

    void LevelState::update( Game& game, const Input& input, Graphics& graphics )
    {
		try
		{
        	blocks_.blocksFromMap( level_.currentMap(), camera_ );
		}
		catch ( const mezun::InvalidBlockType& e )
		{
            game.pushState
			(
				std::move( MessageState::error
				(
					e.what(),
					false,
					std::unique_ptr<GameState> ( LevelSelectState::continueLevelSelect( events_, inventory_, level_.id() ) ),
					false
				) )
			);
		}
		
        blocks_.update( events_ );
		camera_.update();
        events_.update();
        level_.update( events_ );
        sprites_.update( input, camera_, level_.currentMap(), game, events_, blocks_ );
        sprites_.interact( blocks_, level_, events_, inventory_, camera_ );
        sprites_.spriteInteraction( camera_, blocks_, level_.currentMap() );
		level_.interact( sprites_ );
        inventory_.update( events_, sprites_.hero() );

        if ( level_.goal() != nullptr )
            level_.goal()->update( sprites_, inventory_, events_, input );

        if ( events_.message_lock_ && !events_.message_ )
            events_.message_lock_ = false;

        if ( events_.message_ && !events_.message_lock_ )
        {
            game.pushState( std::unique_ptr<GameState> ( new LevelMessageState( level_.currentMap().palette(), level_.message() ) ) );
            events_.message_lock_ = true;
        }
        events_.message_ = false;

        if ( events_.change_map_ )
        {
            level_.warp( sprites_, camera_, this, graphics, inventory_ );
            events_.change_map_ = false;
        }

        if ( events_.paletteChanged() )
        {
            newPalette( graphics, events_.getPalette() );
        }


        if ( !sprites_.hero().isDead() )
        {
            if ( input.pressed( Input::Action::MENU ) )
            {
                sprites_.hero().jump_lock_ = true;
                game.pushState( std::unique_ptr<GameState> ( new PauseState( level_.currentMap().palette(), events_, inventory_.inventory().victory( level_.id() ) ) ) );
            }
        }

        if ( events_.failed_ )
        {
            inventory_.failed();
            game.pushState( std::unique_ptr<GameState> ( new MessageState( "¡Failure!", false, std::unique_ptr<GameState> ( LevelSelectState::continueLevelSelect( events_, inventory_, level_.id() ) ), false, { "Mountain Red", 2 }, Text::FontShade::DARK_GRAY ) ) );
        }
        else if ( events_.won_ )
        {
            inventory_.won( level_.id() );
            game.pushState( std::unique_ptr<GameState> ( new MessageState( "¡Success!", false, std::unique_ptr<GameState> ( LevelSelectState::continueLevelSelect( events_, inventory_, level_.id() ) ), false, { "Classic Green", 2 }, Text::FontShade::DARK_GRAY ) ) );
        }

        if ( events_.quit_level_ )
        {
            inventory_.quit( level_.id() );
            game.changeState( std::unique_ptr<GameState> ( LevelSelectState::continueLevelSelect( events_, inventory_, level_.id() ) ) );
        }
    };

    void LevelState::stateRender( Graphics& graphics )
    {
        level_.currentMap().renderBG( graphics, camera_ );
        blocks_.render( graphics, camera_, false );
        sprites_.render( graphics, camera_, false );
        blocks_.render( graphics, camera_, true );
        sprites_.render( graphics, camera_, true );
        level_.currentMap().renderFG( graphics, camera_ );
        inventory_.render( graphics, level_.id(), events_ );
    };

    void LevelState::init( Game& game, Graphics& graphics )
    {
        newPalette( graphics, level_.currentMap().palette() );
        if ( level_.goal() != nullptr )
        {
            game.pushState( std::unique_ptr<GameState> ( new MessageState( level_.goal()->message(), true, nullptr, false, { "Sunny Yellow", 2 }, Text::FontShade::DARK_GRAY ) ) );
        }

        sprites_.reset( level_, inventory_ );
        inventory_.init();

        if ( level_.goal() != nullptr )
        {
            level_.goal()->init( sprites_.hero(), inventory_, events_ );
        }

        camera_.setPosition( level_.cameraX(), level_.cameraY() );
        events_.reset();

    };


    void LevelState::backFromPop( Game& game, Graphics& graphics ) {};
