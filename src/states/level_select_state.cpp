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

static constexpr int PAGE_MOVE_SPEED = 8;

static constexpr int calculateEntryY( int entry )
{
	return ( entry + 1 ) * 24;
};

static void renderGemScoreOfColor( WTextCharacter::Color color, int level, int y )
{
	WTextObj gem_score{ mezun::charToChar32String( Inventory::gemScore( level ).c_str() ), 352, y, color, 312, WTextObj::Align::LEFT, WTextCharacter::Color::__NULL, 0, 4 };
	gem_score.render();
};

static void renderTimeScoreOfColor( WTextCharacter::Color color, int level, int y )
{
	WTextObj time_score{ mezun::charToChar32String( Inventory::timeScore( level ).c_str() ), 360, y + 8, color, 312, WTextObj::Align::LEFT, WTextCharacter::Color::__NULL, 0, 4 };
	time_score.render();
};

static void renderGemScore( int level, int y )
{
	renderGemScoreOfColor( WTextCharacter::Color::DARK_GRAY, level, y );
};

static void renderTimeScore( int level, int y )
{
	renderTimeScoreOfColor( WTextCharacter::Color::DARK_GRAY, level, y );
};

static void renderVictoryCheckOfColorOffset( int color_offset, int y )
{
	Render::renderObject( "bg/level-select-characters.png", { color_offset * 8, 40, 8, 8 }, { 8, y + 4, 8, 8 } );
}

static void renderDiamondIconOfColorOffset( int color_offset, int y )
{
	Render::renderObject( "bg/level-select-characters.png", { color_offset * 8, 32, 8, 8 }, { 8, y + 12, 8, 8 } );
}

static void renderVictoryCheck( int y )
{
	renderVictoryCheckOfColorOffset( 1, y );
};

static void renderDiamondWinIcon( int y )
{
	renderDiamondIconOfColorOffset( 1, y );
};

static void renderEmptyVictoryCheck( int y )
{
	Render::renderObject( "bg/level-select-characters.png", { 8, 16, 8, 8 }, { 8, y + 4, 8, 8 } );
};

static void renderEmptyDiamondIcon( int y )
{
	Render::renderObject( "bg/level-select-characters.png", { 8, 24, 8, 8 }, { 8, y + 12, 8, 8 } );
};

static void renderPtSymbolOfColorOffset( int color_offset, int y )
{
	Render::renderObject( "bg/level-select-characters.png", { color_offset * 8, 0, 8, 8 }, { 344, y + 4, 8, 8 } );
};

static void renderClockSymbolOfColorOffset( int color_offset, int y )
{
	Render::renderObject( "bg/level-select-characters.png", { color_offset * 8, 8, 8, 8 }, { 352, y + 12, 8, 8 } );
};

static void renderPtSymbol( int y )
{
	renderPtSymbolOfColorOffset( 1, y );
};

static void renderClockSymbol( int y )
{
	renderClockSymbolOfColorOffset( 1, y );
};

static void renderLevelNameOfColor( WTextCharacter::Color color, int level, int cycle, int y )
{
	const std::u32string& level_name = U" " + mezun::intToChar32String( cycle ) + U": " + Level::getLevelNames()[ level ];
	WTextObj level_name_text{ level_name, 24, y, color, 312, WTextObj::Align::LEFT, WTextCharacter::Color::__NULL, 4, 4 };
	level_name_text.render();
};

static void renderLevelName( int level, int cycle, int y )
{
	renderLevelNameOfColor( WTextCharacter::Color::DARK_GRAY, level, cycle, y );
};

static void renderThemeIconOfColorOffset( int color_offset, int theme, int y )
{
	Render::renderObject( "bg/level-select-characters.png", { color_offset * 8, 48 + ( theme * 8 ), 8, 8 }, { 28, y + 4, 8, 8 } );
};

static void renderThemeIcon( int theme, int y )
{
	renderThemeIconOfColorOffset( 1, theme, y );
};

static int getThemeFromLevel( int level )
{
	return level % Level::NUMBER_OF_THEMES;
};

LevelSelectState::LevelSelectState( int current_level )
:
	GameState ( StateID::LEVEL_SELECT_STATE, { "Pale Green", 1 } ),
	title_ (),
	screen_ ( 0, 0, Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS ),
	back_position_ ( 0, 0, Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS ),
	current_page_texture_ ( &page_textures_[ 0 ] ),
	next_page_texture_ ( &page_textures_[ 1 ] ),
	back_position_timer_ ( 0 ),
	selection_timer_ ( 0 ),
	selection_ ( 0 ),
	page_timer_ ( 0 ),
	page_ ( 0 ),
	flash_timer_ ( 0 ),
	flash_frame_ ( 0 ),
	title_character_ ( -1 ),
	title_highlight_timer_ ( 0 ),
	page_change_direction_ ( Direction::Horizontal::__NULL ),
	next_page_ ( 1 ),
	page_textures_ ()
{
	Audio::changeSong( "level-select" );
};

LevelSelectState::~LevelSelectState()
{
	for ( int page = 0; page < NUMBER_OF_PAGES; ++page )
	{
		page_textures_[ page ].destroy();
	}
};

void LevelSelectState::stateUpdate()
{
	if ( Input::held( Input::Action::MENU ) )
	{
		Main::popState();
	}
	else
	{
		switch ( page_change_direction_ )
		{
			case ( Direction::Horizontal::LEFT ):
			{
				if ( current_page_texture_->getX() >= Unit::WINDOW_WIDTH_PIXELS )
				{
					page_ = next_page_;
					current_page_texture_ = &page_textures_[ page_ ];
					current_page_texture_->setX( 0 );
					page_change_direction_ = Direction::Horizontal::__NULL;
				}
				else
				{
					current_page_texture_->moveRight( PAGE_MOVE_SPEED );
					next_page_texture_->moveRight( PAGE_MOVE_SPEED );
				}
			}
			break;

			case ( Direction::Horizontal::RIGHT ):
			{
				if ( current_page_texture_->getX() <= -Unit::WINDOW_WIDTH_PIXELS )
				{
					page_ = next_page_;
					current_page_texture_ = &page_textures_[ page_ ];
					current_page_texture_->setX( 0 );
					page_change_direction_ = Direction::Horizontal::__NULL;
				}
				else
				{
					current_page_texture_->moveLeft( PAGE_MOVE_SPEED );
					next_page_texture_->moveLeft( PAGE_MOVE_SPEED );
				}
			}
			break;

			case ( Direction::Horizontal::__NULL ):
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

				if ( selection_timer_ == 0 )
				{
					if ( Input::held( Input::Action::MOVE_RIGHT ) )
					{
						page_change_direction_ = Direction::Horizontal::RIGHT;
						next_page_ = ( page_ < NUMBER_OF_PAGES - 1 ) ? page_ + 1 : 0;
						next_page_texture_ = &page_textures_[ next_page_ ];
						next_page_texture_->setX( Unit::WINDOW_WIDTH_PIXELS );
						turnOffAndResetAnimations();
					}
					else if ( Input::held( Input::Action::MOVE_LEFT ) )
					{
						page_change_direction_ = Direction::Horizontal::LEFT;
						next_page_ = ( page_ > 0 ) ? page_ - 1 : NUMBER_OF_PAGES - 1;
						next_page_texture_ = &page_textures_[ next_page_ ];
						next_page_texture_->setX( -Unit::WINDOW_WIDTH_PIXELS );
						turnOffAndResetAnimations();
					}
					else if ( Input::held( Input::Action::MOVE_DOWN ) )
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

				if ( title_character_ == -1 )
				{
					if ( title_highlight_timer_ > 30 )
					{
						++title_character_;
						title_highlight_timer_ = 0;
					}
					else
					{
						++title_highlight_timer_;
					}
				}
				else
				{
					if ( title_highlight_timer_ > 4 )
					{
						++title_character_;
						title_highlight_timer_ = 0;
					}
					else
					{
						++title_highlight_timer_;
						if ( title_character_ == title_.lines_[ 0 ].frames_.size() )
						{
							title_character_ = -1;
						}

						if ( title_character_ == 0 )
						{
							title_.lines_[ 0 ].frames_[ 0 ].changeColor( WTextCharacter::Color::WHITE );
						}
						else if ( title_character_ == -1 )
						{
							title_.lines_[ 0 ].frames_[ title_.lines_[ 0 ].frames_.size() - 1 ].changeColor( WTextCharacter::Color::LIGHT_GRAY );
						}
						else
						{
							title_.lines_[ 0 ].frames_[ title_character_ ].changeColor( WTextCharacter::Color::WHITE );
							title_.lines_[ 0 ].frames_[ title_character_ - 1 ].changeColor( WTextCharacter::Color::LIGHT_GRAY );
						}
					}
				}

			}
			break;
		}
	}
};

void LevelSelectState::stateRender()
{
	Render::renderObject( "bg/level-select-back.png", back_position_, screen_ );

	if ( page_change_direction_ == Direction::Horizontal::__NULL )
	{
		current_page_texture_->render();
		renderSelectHighlight();

		const int cycle = ( int )( std::floor( ( double )( page_ ) / ( double )( PAGES_PER_CYCLE ) ) ) + 1;
		int level = ( ( cycle - 1 ) * Level::NUMBER_OF_THEMES ) + ( ( page_ % PAGES_PER_CYCLE ) * LEVELS_PER_PAGE );
		const auto* level_names = Level::getLevelNames();
		for ( int entry = 0; entry < calculateNumberOfLevelsInPage( page_ ); ++entry )
		{
			const int y = calculateEntryY( entry );
			const int theme = getThemeFromLevel( level );

			if ( Inventory::levelComplete( level ) )
			{
				renderFlashingLevelName( level, cycle, y );
				renderFlashingThemeIcon( theme, y );
				renderFlashingVictoryCheck( y );
				renderFlashingDiamondWinIcon( y );
			}
			else if ( entry == selection_ )
			{
				renderLevelName( level, cycle, y );
				renderThemeIcon( theme, y );

				if ( Inventory::victory( level ) )
				{
					renderVictoryCheck( y );
				}
				else
				{
					renderEmptyVictoryCheck( y );
				}

				if ( Inventory::haveDiamond( level ) )
				{
					renderDiamondWinIcon( y );
				}
				else
				{
					renderEmptyDiamondIcon( y );
				}
			}

			if ( Inventory::gemChallengeBeaten( level ) )
			{
				renderFlashingPtSymbol( y );
				renderFlashingGemScore( level, y );
			}
			else if ( entry == selection_ )
			{
				renderPtSymbol( y );
				renderGemScore( level, y );
			}

			if ( Inventory::timeChallengeBeaten( level ) )
			{
				renderFlashingClockSymbol( y );
				renderFlashingTimeScore( level, y );
			}
			else if ( entry == selection_ )
			{
				renderClockSymbol( y );
				renderTimeScore( level, y );
			}

			++level;
		}
	}
	else
	{
		current_page_texture_->render();
		next_page_texture_->render();
	}

	title_.render();
};

void LevelSelectState::init()
{
	title_ = { Localization::getCurrentLanguage().getLevelSelectTitle(), 0, 0, WTextCharacter::Color::LIGHT_GRAY, Unit::WINDOW_WIDTH_PIXELS, WTextObj::Align::CENTER, WTextCharacter::Color::BLACK, 8, 8 };
	generatePageTextures();
};

void LevelSelectState::generatePageTextures()
{
	const double pages_per_cycle_double = ( double )( PAGES_PER_CYCLE );
	const std::u32string completion_string = mezun::intToChar32String( 15 ) + U"%";
	const WTextObj completion_text{ completion_string, 0, 8, WTextCharacter::Color::DARK_GRAY, Unit::WINDOW_WIDTH_PIXELS, WTextObj::Align::RIGHT, WTextCharacter::Color::__NULL, 8, 4 };
	const std::u32string& cycle_name = Localization::getCurrentLanguage().getLevelSelectCycleName();
	int cycle = 1;
	WTextObj cycle_text;
	int level = 0;
	for ( int page = 0; page < NUMBER_OF_PAGES; ++page )
	{
		if ( page % PAGES_PER_CYCLE == 0 )
		{
			const std::u32string cycle_string = mezun::removeEndingZeroFrom32String( cycle_name ) + U" " + mezun::intToChar32String( cycle );
			cycle_text = { cycle_string, 4, 8, WTextCharacter::Color::DARK_GRAY, Unit::WINDOW_WIDTH_PIXELS, WTextObj::Align::LEFT, WTextCharacter::Color::__NULL, 4, 4 };
			++cycle;
		}

		page_textures_[ page ].init();
		page_textures_[ page ].startDrawing();
		Render::clearScreenTransparency();
		Render::renderObject( "bg/level-select.png", screen_, screen_ );
		cycle_text.render();
		completion_text.render();

		Render::renderObject( "bg/level-select-icons.png", screen_, screen_ );
		for ( int entry = 0; entry < calculateNumberOfLevelsInPage( page ); ++entry )
		{
			const int theme = getThemeFromLevel( level );
			const int y = calculateEntryY( entry );

			renderLevelName( level, cycle - 1, y );
			renderThemeIcon( theme, y );

			if ( Inventory::victory( level ) )
			{
				renderVictoryCheck( y );
			}
			if ( Inventory::haveDiamond( level ) )
			{
				renderDiamondWinIcon( y );
			}
			renderGemScore( level, y );
			renderTimeScore( level, y );
			++level;
		}

		page_textures_[ page ].endDrawing();
	}
};

void LevelSelectState::renderSelectHighlight() const
{
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
}

void LevelSelectState::turnOffAndResetAnimations()
{
	selection_timer_ = 0;
	flash_timer_ = 0;
	title_character_ = -1;
	title_highlight_timer_ = 0;
};

WTextCharacter::Color LevelSelectState::flashOnCondition( bool condition ) const
{
	return ( condition )
		? ( WTextCharacter::Color )( FLASH_FRAMES[ flash_frame_ ] )
		: WTextCharacter::Color::DARK_GRAY;
}

int LevelSelectState::calculateNumberOfLevelsInPage( int page )
{
	return ( page % PAGES_PER_CYCLE == PAGES_PER_CYCLE - 1 ) ? Level::NUMBER_OF_THEMES % LEVELS_PER_PAGE : LEVELS_PER_PAGE;
};

void LevelSelectState::renderFlashingPtSymbol( int y ) const
{
	renderPtSymbolOfColorOffset( FLASH_FRAMES[ flash_frame_ ], y );
};

void LevelSelectState::renderFlashingClockSymbol( int y ) const
{
	renderClockSymbolOfColorOffset( FLASH_FRAMES[ flash_frame_ ], y );
};

void LevelSelectState::renderFlashingLevelName( int level, int cycle, int y ) const
{
	renderLevelNameOfColor( ( WTextCharacter::Color )( FLASH_FRAMES[ flash_frame_ ] ), level, cycle, y );
};

void LevelSelectState::renderFlashingThemeIcon( int theme, int y ) const
{
	renderThemeIconOfColorOffset( FLASH_FRAMES[ flash_frame_ ], theme, y );
};

void LevelSelectState::renderFlashingVictoryCheck( int y ) const
{
	renderVictoryCheckOfColorOffset( FLASH_FRAMES[ flash_frame_ ], y );
};

void LevelSelectState::renderFlashingDiamondWinIcon( int y ) const
{
	renderDiamondIconOfColorOffset( FLASH_FRAMES[ flash_frame_ ], y );
};

void LevelSelectState::renderFlashingGemScore( int level, int y ) const
{
	renderGemScoreOfColor( ( WTextCharacter::Color )( FLASH_FRAMES[ flash_frame_ ] ), level, y );
};

void LevelSelectState::renderFlashingTimeScore( int level, int y ) const
{
	renderTimeScoreOfColor( ( WTextCharacter::Color )( FLASH_FRAMES[ flash_frame_ ] ), level, y );
};