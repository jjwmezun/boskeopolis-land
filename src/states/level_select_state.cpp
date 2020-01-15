#include "audio.hpp"
#include "main.hpp"
#include "input.hpp"
#include "inventory.hpp"
#include "level.hpp"
#include "level_select_state.hpp"
#include "localization.hpp"
#include "localization_language.hpp"
#include "mezun_helpers.hpp"
#include "render.hpp"

static constexpr int PAGES_PER_CYCLE = 2;
static constexpr int NUMBER_OF_PAGES = PAGES_PER_CYCLE * Level::NUMBER_OF_CYCLES;
static constexpr int NUMBER_OF_FLASH_FRAMES = 12;
static constexpr int FLASH_FRAMES[ NUMBER_OF_FLASH_FRAMES ] =
{
	0, 1, 2, 3, 4, 5, 5, 4, 3, 2, 1, 0
};

LevelSelectState::LevelSelectState( int current_level )
:
	GameState ( StateID::LEVEL_SELECT_STATE, { "Pale Green", 1 } ),
	screen_ ( 0, 0, Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS ),
	back_position_ ( 0, 0, Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS ),
	current_page_ ( nullptr ),
	previous_page_ ( nullptr ),
	back_position_timer_ ( 0 ),
	selection_timer_ ( 0 ),
	selection_ ( 0 ),
	page_ ( 0 ),
	flash_timer_ ( 0 ),
	flash_frame_ ( 0 )
{
	Audio::changeSong( "level-select" );
};

LevelSelectState::~LevelSelectState() {};

void LevelSelectState::stateUpdate()
{
	if ( back_position_timer_ == 4 )
	{
		back_position_timer_ = 0;
		++back_position_.x;
		if ( back_position_.x == 32 )
		{
			back_position_.x = 0;
		}
		back_position_.y = back_position_.x;
	}
	else
	{
		++back_position_timer_;
	}

	if ( Input::held( Input::Action::MENU ) )
	{
		Main::popState();
	}
	else
	{
		if ( selection_timer_ == 0 )
		{
			// TODO: PAGE CHANGES
			if ( Input::held( Input::Action::MOVE_DOWN ) )
			{
				++selection_;
				if ( selection_ > 7 )
				{
					selection_ = 0;
				}
				selection_timer_ = 8;
			}
			else if ( Input::held( Input::Action::MOVE_UP ) )
			{
				--selection_;
				if ( selection_ < 0 )
				{
					selection_ = 7;
				}
				selection_timer_ = 8;
			}
		}
		else
		{
			--selection_timer_;
		}
	}

	if ( flash_timer_ > 7 )
	{
		flash_timer_ = 0;
		++flash_frame_;
		if ( flash_frame_ == NUMBER_OF_FLASH_FRAMES )
		{
			flash_frame_ = 0;
		}
	}
	else
	{
		++flash_timer_;
	}
};

void LevelSelectState::stateRender()
{
	Render::colorCanvas( 1 );
	Render::renderObject( "bg/level-select-back.png", back_position_, screen_ );
	Render::renderObject( "bg/level-select.png", screen_, screen_ );

	WTextObj title{ Localization::getCurrentLanguage().getLevelSelectTitle(), 0, 0, WTextObj::Color::WHITE, Unit::WINDOW_WIDTH_PIXELS, WTextObj::Align::CENTER, WTextObj::Color::BLACK, 8, 8 };
	title.render();

	std::u32string completion_string = mezun::intToChar32String( 15 ) + U"%";
	WTextObj completion_text{ completion_string, 0, 8, WTextObj::Color::DARK_GRAY, Unit::WINDOW_WIDTH_PIXELS, WTextObj::Align::RIGHT, WTextObj::Color::__NULL, 8, 4 };
	completion_text.render();

	const std::u32string& cycle_name = Localization::getCurrentLanguage().getLevelSelectCycleName();
	std::u32string cycle_string = mezun::removeEndingZeroFrom32String( cycle_name ) + U" " + mezun::intToChar32String( 1 );
	WTextObj cycle_text{ cycle_string, 4, 8, WTextObj::Color::DARK_GRAY, Unit::WINDOW_WIDTH_PIXELS, WTextObj::Align::LEFT, WTextObj::Color::__NULL, 4, 4 };
	cycle_text.render();

	if ( selection_ == 0 )
	{
		Render::renderObject( "bg/level-select-bar.png", { 0, 0, 392, 24 }, { 4, 24, 392, 24 } );
	}
	else if ( selection_ == 7 )
	{
		Render::renderObject( "bg/level-select-bar.png", { 0, 0, 392, 24 }, { 4, 192, 392, 24 }, SDL_FLIP_VERTICAL );
	}
	else
	{
		Render::renderRect( { 4, ( selection_ + 1 ) * 24, 392, 24 }, 1 );
	}
	Render::renderObject( "bg/level-select-icons.png", screen_, screen_ );

	const auto* level_names = Level::getLevelNames();
	for ( int i = 0; i < 8; ++i )
	{
		const int y = ( i + 1 ) * 24;
		const std::u32string& level_name = U" " + mezun::intToChar32String( 1 ) + U": " + level_names[ i ];
		const WTextObj::Color name_color = flashOnCondition( Inventory::levelComplete( i ) );
		WTextObj level_name_text{ level_name, 24, y, name_color, 312, WTextObj::Align::LEFT, WTextObj::Color::__NULL, 4, 4 };
		const int theme_icon_color = ( Inventory::levelComplete( i ) ) ? FLASH_FRAMES[ flash_frame_ ] * 8 : 8;
		Render::renderObject( "bg/level-select-characters.png", { theme_icon_color, 48 + ( i * 8 ), 8, 8 }, { 28, y + 4, 8, 8 } );
		level_name_text.render();
		if ( Inventory::levelComplete( i ) )
		{
			Render::renderObject( "bg/level-select-characters.png", { FLASH_FRAMES[ flash_frame_ ] * 8, 40, 8, 8 }, { 8, y + 4, 8, 8 } );
			Render::renderObject( "bg/level-select-characters.png", { FLASH_FRAMES[ flash_frame_ ] * 8, 32, 8, 8 }, { 8, y + 12, 8, 8 } );
		}
		else
		{
			if ( Inventory::victory( i ) )
			{
				Render::renderObject( "tilesets/universal.png", { 48, 8, 8, 8 }, { 8, y + 4, 8, 8 } );
			}
			if ( Inventory::haveDiamond( i ) )
			{
				Render::renderObject( "tilesets/universal.png", { 48, 0, 8, 8 }, { 8, y + 12, 8, 8 } );
			}
		}
		const WTextObj::Color gem_color = flashOnCondition( Inventory::gemChallengeBeaten( i ) );
		WTextObj gem_score{ mezun::charToChar32String( Inventory::gemScore( i ).c_str() ), 352, y, gem_color, 312, WTextObj::Align::LEFT, WTextObj::Color::__NULL, 0, 4 };
		gem_score.render();
		const WTextObj::Color time_color = flashOnCondition( Inventory::timeChallengeBeaten( i ) );
		WTextObj time_score{ mezun::charToChar32String( Inventory::timeScore( i ).c_str() ), 360, y + 8, time_color, 312, WTextObj::Align::LEFT, WTextObj::Color::__NULL, 0, 4 };
		time_score.render();
		if ( Inventory::gemChallengeBeaten( i ) )
		{
			Render::renderObject( "bg/level-select-characters.png", { FLASH_FRAMES[ flash_frame_ ] * 8, 0, 8, 8 }, { 344, y + 4, 8, 8 } );
		}
		if ( Inventory::timeChallengeBeaten( i ) )
		{
			Render::renderObject( "bg/level-select-characters.png", { FLASH_FRAMES[ flash_frame_ ] * 8, 8, 8, 8 }, { 352, y + 12, 8, 8 } );
		}
	}
};

void LevelSelectState::init()
{
};

WTextObj::Color LevelSelectState::flashOnCondition( bool condition ) const
{
	return ( condition )
		? ( WTextObj::Color )( FLASH_FRAMES[ flash_frame_ ] )
		: WTextObj::Color::DARK_GRAY;
}