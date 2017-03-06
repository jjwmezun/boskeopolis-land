


// Name
//===================================
//
// LevelSelectState
//


// DEPENDENCIES
//===================================

    #include "game.h"
    #include "input.h"
    #include "level.h"
    #include "level_state.h"
    #include "level_select_state.h"
	#include "message_state.h"
    #include "mezun_exceptions.h"
	#include "mezun_helpers.h"
    #include "title_state.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    LevelSelectState::LevelSelectState( Status status, EventSystem events, Inventory inventory, int level )
    :
        GameState ( StateID::LEVEL_SELECT_STATE ),
        events_ ( events ),
        inventory_ ( inventory ),
        prev_level_ ( level ),
        selection_ ( Counter( level, Level::realLevelNum()-1, 0 ) ),
        camera_ ( { 0, 0, Unit::WINDOW_WIDTH_BLOCKS, 7, 0, START_Y } ),
        status_ ( status )
    {
    };

    LevelSelectState::~LevelSelectState() {};

    LevelSelectState* LevelSelectState::continueLevelSelect( EventSystem events, InventoryLevel inventory, int level )
    {
        return new LevelSelectState
        (
            Status::CONTINUE,
            events,
            inventory.inventory(),
            level
        );
    };

    LevelSelectState* LevelSelectState::newLevelSelect()
    {
        return new LevelSelectState
        (
            Status::NEW,
            {},
            {},
            DEFAULT_LEVEL
            //Level::LevelName::LV_CITY_2
        );
    };

    LevelSelectState* LevelSelectState::loadLevelSelect()
    {
        return new LevelSelectState
        (
            Status::LOAD,
            {},
            {},
            DEFAULT_LEVEL
        );
    };

    void LevelSelectState::update( Game& game, const Input& input, Graphics& graphics )
    {
        highlight_dest_.y = ( 8 * selection_() );

        if ( highlight_dest_.y + highlight_dest_.h > camera_.bottom() )
        {
            camera_.moveDown( 8 );
        }
        if ( highlight_dest_.y < camera_.y() )
        {
            camera_.moveUp( 8 );
        }

        highlight_dest_.y = camera_.relativeY( highlight_dest_ );

        if ( delay_ <= 0 )
        {
            if ( input.held( Input::Action::MOVE_UP ) )
            {
                --selection_;
                delay_ = delay_length_;
            }
            if ( input.held( Input::Action::MOVE_DOWN ) )
            {
                ++selection_;
                delay_ = delay_length_;
            }
        }
        else
        {
            --delay_;
        }

        if ( input.held( Input::Action::MOVE_UP ) || input.held( Input::Action::MOVE_DOWN ) )
            delay_length_ = 4;
        else
            delay_length_ = 8;

		inventory_.update();

        if ( input.pressed( Input::Action::CONFIRM ) )
        {
            game.changeState( std::unique_ptr<GameState> ( new LevelState( events_, inventory_, level_ids_.at( selection_() ), game ) ) );
        }
        else if ( input.pressed( Input::Action::MENU ) )
        {
            game.changeState( std::unique_ptr<GameState> ( new TitleState() ) );
        }
    };

    void LevelSelectState::stateRender( Graphics& graphics )
    {
        graphics.renderRect( highlight_dest_, 6 );

        for ( int i = 0; i < level_ids_.size(); ++i )
        {
            Text::FontShade shade = Text::FontShade::BLACK;

            if ( i == selection_.value() )
                shade = Text::FontShade::WHITE;

			if ( i < level_names_.size() )
            	level_names_[ i ].render( graphics, &camera_, shade );

            if ( i < gem_scores_.size() )
                gem_scores_[ i ].render( graphics, &camera_, shade );

            if ( i < time_scores_.size() )
                time_scores_[ i ].render( graphics, &camera_, shade );
        }

        for ( int j = 0; j < win_icon_dests_.size(); ++j )
        {
            win_icon_gfx_.render( graphics, win_icon_dests_[ j ], &camera_ );
        }

        for ( int i = 0; i < diamond_icon_dests_.size(); ++i )
        {
            diamond_gfx_.render( graphics, diamond_icon_dests_[ i ], &camera_ );
        }

        graphics.renderRect( INVENTORY_BG_DEST, BG_COLOR );
        gem_icon_gfx_.render( graphics, GEM_ICON_DEST, nullptr );

        Text::FontShade shade = Text::FontShade::BLACK;

        if ( inventory_.totalFundsShown() < 0 )
		{
            shade = Text::FontShade::DARK_MID_GRAY;
		}

        Text::renderNumber( graphics, inventory_.totalFundsShown(), FUNDS_X, INVENTORY_Y + 8, 9, shade );

        graphics.renderRect( HEADER_BG_DEST, BG_COLOR );
        title_.render( graphics );
    };

    void LevelSelectState::init( Game& game, Graphics& graphics )
    {
        switch ( status_ )
        {
            case ( Status::LOAD ):
				try
				{
                	inventory_.load();
				}
				catch( const mezun::InvalidSaveSizeException& e )
				{	
					game.pushState
					(
						std::move( MessageState::error
						(
							e.what(),
							false,
							std::make_unique<TitleState> (),
							false
						) )
					);
				}
            break;
        }

        constexpr int X = 24;

		try
		{
			
			int reali = 0;
			
			for ( int i = 0; i < Level::NUM_O_LEVELS; ++i )
			{
				const std::string lvname = Level::NameOLevel( i );

				if ( !mezun::isStringEmpty( lvname ) )
				{				
					level_names_.emplace_back( lvname, X + 8, ( 8 * reali ) );
					
					level_ids_.emplace_back( i );

					gem_scores_.emplace_back( inventory_.gemScore( i ), X + ( 23 * 8 ), ( 8 * reali ) );
					time_scores_.emplace_back( inventory_.timeScore( i ), X + ( 30 * 8 ), ( 8 * reali ) );

					if ( inventory_.victory( i ) )
					{
						win_icon_dests_.emplace_back( X, ( 8 * reali ), 8, 8 );
					}

					if ( inventory_.haveDiamond( i ) )
					{
						diamond_icon_dests_.emplace_back( X - 8, ( 8 * reali ), 8, 8 );
					}
					
					++reali;
				}
			}
			
			auto select = std::find( level_ids_.begin(), level_ids_.end(), prev_level_ );
			
			if ( select != level_ids_.end() )
			{
				selection_ = std::distance( level_ids_.begin(), select );
			}
		}
		catch ( const mezun::CantLoadLevels& e )
		{
			game.pushState
			(
				std::move( MessageState::error
				(
					e.what(),
					false,
					std::make_unique<TitleState> (),
					false
				) )
			);
		}

        events_.reset();
    };

    void LevelSelectState::backFromPop( Game& game, Graphics& graphics ) {};
