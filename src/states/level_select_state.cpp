#include "audio.hpp"
#include "main.hpp"
#include "input.hpp"
#include "inventory.hpp"
#include "level_select_state.hpp"
#include "localization.hpp"
#include "localization_language.hpp"
#include "mezun_helpers.hpp"
#include "overworld_state.hpp"
#include "render.hpp"

// From dark to light, delaying a li’l @ either end.
static constexpr int NUMBER_OF_FLASH_FRAMES = 12;
static constexpr int FLASH_FRAMES[ NUMBER_OF_FLASH_FRAMES ] =
{
	0, 1, 2, 3, 4, 5, 5, 4, 3, 2, 1, 0
};

static constexpr int INPUT_DELAY = 8;
static constexpr int PAGE_MOVE_SPEED = 8;
static constexpr int FLASH_SPEED = 8;
static constexpr int GRID_Y = 24;
static constexpr int ROW_HEIGHT = 24;
static constexpr int calculateEntryY( int entry )
{
	return entry * ROW_HEIGHT + GRID_Y;
};
static constexpr int calculateNumberOfLevelsInPage( int page )
{
	// If last page o’ cycle, calculate remaining levels that fit ( since it could be less than max ); else, max per page.
	const int remainder = Level::NUMBER_OF_THEMES % LevelSelectState::LEVELS_PER_PAGE;
	return ( page % LevelSelectState::PAGES_PER_CYCLE == LevelSelectState::PAGES_PER_CYCLE - 1 )
			? ( ( remainder == 0 ) ? LevelSelectState::LEVELS_PER_PAGE : remainder )
			: LevelSelectState::LEVELS_PER_PAGE;
};

static void renderGemScoreOfColor( WTextCharacter::Color color, int level, int y );
static void renderTimeScoreOfColor( WTextCharacter::Color color, int level, int y );
static void renderGemScore( int level, int y );
static void renderTimeScore( int level, int y );
static void renderVictoryCheckOfColorOffset( int color_offset, int y );
static void renderDiamondIconOfColorOffset( int color_offset, int y );
static void renderVictoryCheck( int y );
static void renderDiamondWinIcon( int y );
static void renderEmptyVictoryCheck( int y );
static void renderEmptyDiamondIcon( int y );
static void renderPtSymbolOfColorOffset( int color_offset, int y );
static void renderClockSymbolOfColorOffset( int color_offset, int y );
static void renderPtSymbol( int y );
static void renderClockSymbol( int y );
static int getThemeFromLevel( int level );
static void renderGemTargetScore( int level, int y );
static void renderTimeTargetScore( int level, int y );
static void renderPtTargetSymbol( int y );
static void renderClockTargetSymbol( int y );
static void renderCrownIconOfColorOffset( int color_offset, int y );
static void renderSecretGoalIconOfColor( int color_offset, int y );
static void renderCrownWinIcon( int y );
static void renderEmptyCrownIcon( int y );
static void renderSecretGoalIcon( int y );
static void renderEmptySecretGoalIcon( int y );

LevelSelectState::LevelSelectState( int current_level )
:
	GameState ( StateID::LEVEL_SELECT_STATE, { "Pale Green", 1 } ),
	show_target_scores_ ( false ),
	page_change_direction_ ( Direction::Horizontal::__NULL ),
	selection_timer_ ( 0 ),
	selection_ ( 0 ),
	page_ ( 0 ),
	flash_timer_ ( 0 ),
	flash_frame_ ( 0 ),
	next_page_ ( 1 ),
	first_level_of_page_ ( 0 ),
	number_of_pages_ ( 2 ),
	current_page_texture_ ( &moving_page_textures_[ 0 ] ),
	next_page_texture_ ( &moving_page_textures_[ 1 ] ),
	screen_ ( 0, 0, Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS ),
	background_ (),
	moving_page_textures_ (),
	static_page_texture_ (),
	final_table_texture_ (),
	title_ ({ Localization::getCurrentLanguage().getLevelSelectTitle(), 0, 0, WTextCharacter::Color::LIGHT_GRAY, Unit::WINDOW_WIDTH_PIXELS, WTextObj::Align::CENTER, WTextCharacter::Color::BLACK, 8, 8 }),
	level_names_ (),
	level_name_headers_ ()
{
	Audio::changeSong( "level-select" );
};

LevelSelectState::~LevelSelectState()
{
	// Destroy all textures.
	for ( int page = 0; page < NUMBER_OF_PAGES; ++page )
	{
		moving_page_textures_[ page ].destroy();
	}
	static_page_texture_.destroy();
	final_table_texture_.destroy();
};

void LevelSelectState::stateUpdate()
{
	if ( Input::pressed( Input::Action::CONFIRM ) )
	{
		if ( Inventory::beenToLevel( getSelectedLevel() ) )
		{
			Audio::playSound( Audio::SoundType::CONFIRM );
			Main::changeState( std::make_unique<OverworldState> ( OWTile::createLevel( getSelectedLevel() ) ) );
			return;
		}
		else
		{
			Audio::playSound( Audio::SoundType::CANCEL );
		}
	}
	else if ( Input::pressed( Input::Action::MENU ) )
	{
		Audio::playSound( Audio::SoundType::CANCEL );
		Main::popState( true );
	}
	else
	{
		background_.update();
		title_.update();
		switch ( page_change_direction_ )
		{
			// Move both pages rightward ( so the left page is where the current is — its right )
			// till the current page is offscreen ( past the rightmost edge )
			//
			// Then it switches the next page to current, rerenders, & turns off movement direction.
			case ( Direction::Horizontal::LEFT ):
			{
				if ( current_page_texture_->getX() >= Unit::WINDOW_WIDTH_PIXELS )
				{
					finishPageFlip();
				}
				else
				{
					current_page_texture_->moveRight( PAGE_MOVE_SPEED );
					next_page_texture_->moveRight( PAGE_MOVE_SPEED );
					drawFlippedTexture();
				}
			}
			break;

			// Move both pages leftward ( so the left page is where the current is — its left )
			// till the current page is offscreen ( past the leftmost edge )
			//
			// Then it switches the next page to current, rerenders, & turns off movement direction.
			case ( Direction::Horizontal::RIGHT ):
			{
				if ( current_page_texture_->getX() <= -Unit::WINDOW_WIDTH_PIXELS )
				{
					finishPageFlip();
				}
				else
				{
					current_page_texture_->moveLeft( PAGE_MOVE_SPEED );
					next_page_texture_->moveLeft( PAGE_MOVE_SPEED );
					drawFlippedTexture();
				}
			}
			break;

			case ( Direction::Horizontal::__NULL ):
			{
				handleTargetScoreBehavior();

				// Only allow actions if not in the middle o’ movement animation.
				if ( selection_timer_ == 0 )
				{
					if ( Input::held( Input::Action::MOVE_RIGHT ) )
					{
						flipPageRight();
					}
					else if ( Input::held( Input::Action::MOVE_LEFT ) )
					{
						flipPageLeft();
					}
					else if ( Input::held( Input::Action::MOVE_DOWN ) )
					{
						++selection_;
						if ( selection_ > LEVELS_PER_PAGE - 1 )
						{
							selection_ = 0;
						}
						selection_timer_ = INPUT_DELAY;
						redrawStaticPageTexture();
						Audio::playSound( Audio::SoundType::SELECT );
					}
					else if ( Input::held( Input::Action::MOVE_UP ) )
					{
						--selection_;
						if ( selection_ < 0 )
						{
							selection_ = LEVELS_PER_PAGE - 1;
						}
						selection_timer_ = INPUT_DELAY;
						redrawStaticPageTexture();
						Audio::playSound( Audio::SoundType::SELECT );
					}
				}
				else
				{
					--selection_timer_;
				}

				animateTextFlashColor();
			}
			break;
		}
	}
};

void LevelSelectState::stateRender()
{
	background_.render();
	final_table_texture_.render();
	title_.render();
};

void LevelSelectState::init()
{
	generateLevelNames();
	generatePageTextures();
};

void LevelSelectState::handleTargetScoreBehavior()
{
	if ( !show_target_scores_ )
	{
		if ( Input::held( Input::Action::CANCEL ) && Inventory::beenToLevel( getSelectedLevel() ) )
		{
			show_target_scores_ = true;
			Audio::playSound( Audio::SoundType::SELECT );
			redrawStaticPageTexture(); // Score state changes, so we must redraw.
		}
	}
	else
	{
		if ( !Input::held( Input::Action::CANCEL ) )
		{
			show_target_scores_ = false;
			redrawStaticPageTexture(); // Score state changes, so we must redraw.
		}
	}
};

void LevelSelectState::flipPage( Direction::Horizontal direction, int next_page, int position )
{
	page_change_direction_ = direction;
	next_page_ = next_page;
	next_page_texture_ = &moving_page_textures_[ next_page_ ];
	next_page_texture_->setX( position );
	Audio::playSound( Audio::SoundType::PAGE );
	turnOffAndResetAnimations();
};

void LevelSelectState::flipPageLeft()
{
	const int next_page = ( number_of_pages_ == 2 ) ? ( ( page_ == 0 ) ? 1 : 0 ) : ( ( page_ > 0 ) ? page_ - 1 : number_of_pages_ - 1 );
	flipPage( Direction::Horizontal::LEFT, next_page, -Unit::WINDOW_WIDTH_PIXELS );
};

void LevelSelectState::flipPageRight()
{
	const int next_page = ( number_of_pages_ == 2 ) ? ( ( page_ == 0 ) ? 1 : 0 ) : ( ( page_ < number_of_pages_ - 1 ) ? page_ + 1 : 0 );
	flipPage( Direction::Horizontal::RIGHT, next_page, Unit::WINDOW_WIDTH_PIXELS );
};

void LevelSelectState::finishPageFlip()
{
	setInfoToNextPage();
	current_page_texture_ = &moving_page_textures_[ page_ ];
	current_page_texture_->setX( 0 );
	page_change_direction_ = Direction::Horizontal::__NULL;
	redrawStaticPageTexture();
};

void LevelSelectState::generatePageTextures()
{
	generateMovingPageTextures();
	static_page_texture_.init();
	final_table_texture_.init();
	redrawStaticPageTexture();
};

// Used for generating the textures for all pages used when they are moving ( during which they don’t animate ).
// Since these textures ne’er change, they can be generated once @ the start & then just drawn directly during moving states.
void LevelSelectState::generateMovingPageTextures()
{
	const WTextObj completion_text{ Inventory::percentShown(), 0, 8, WTextCharacter::Color::DARK_GRAY, Unit::WINDOW_WIDTH_PIXELS, WTextObj::Align::RIGHT, WTextCharacter::Color::__NULL, 8, 4 };
	const std::u32string& cycle_name = Localization::getCurrentLanguage().getLevelSelectCycleName();
	WTextObj cycle_text;
	int cycle = 1;
	int level = 0;
	for ( int page = 0; page < NUMBER_OF_PAGES; ++page )
	{
		const bool is_new_cycle = page % PAGES_PER_CYCLE == 0;
		if ( is_new_cycle )
		{
			const std::u32string cycle_string = mezun::removeEndingZeroFrom32String( cycle_name ) + U" " + mezun::intToChar32String( cycle );
			cycle_text = { cycle_string, 4, 8, WTextCharacter::Color::DARK_GRAY, Unit::WINDOW_WIDTH_PIXELS, WTextObj::Align::LEFT, WTextCharacter::Color::__NULL, 4, 4 };
			++cycle;
		}

		moving_page_textures_[ page ].init();
		moving_page_textures_[ page ].startDrawing();
		Render::clearScreenTransparency();
		Render::renderObject( "bg/level-select.png", screen_, screen_ );
		cycle_text.render();
		completion_text.render();

		Render::renderObject( "bg/level-select-icons.png", screen_, screen_ );
		for ( int entry = 0; entry < calculateNumberOfLevelsInPage( page ); ++entry )
		{
			const int theme = getThemeFromLevel( level );
			const int y = calculateEntryY( entry );

			renderLevelName( level );

			if ( Inventory::victory( level ) )
			{
				renderVictoryCheck( y );
			}
			if ( Inventory::haveDiamond( level ) )
			{
				renderDiamondWinIcon( y );
			}
			if ( Inventory::hasCrown( level ) )
			{
				renderCrownWinIcon( y );
			}
			if ( Level::hasSecretGoal( level ) )
			{
				if ( Inventory::getSecretGoal( level ) )
				{
					renderSecretGoalIcon( y );
				}
				else
				{
					renderEmptySecretGoalIcon( y );
				}
			}
			renderGemScore( level, y );
			renderTimeScore( level, y );

			// If we have been to this level, update the latest cycle.
			// This means the latest cycle doesn’t include levels we haven’t been to yet.
			if ( Inventory::beenToLevel( level ) )
			{
				number_of_pages_ = ( cycle - 1 ) * 2;
			}
			++level;
		}

		moving_page_textures_[ page ].endDrawing();
	}
};

void LevelSelectState::redrawStaticPageTexture()
{
	static_page_texture_.startDrawing();
	Render::clearScreenTransparency();
	current_page_texture_->render();
	renderSelectHighlight();

	int level = first_level_of_page_;
	for ( int entry = 0; entry < calculateNumberOfLevelsInPage( page_ ); ++entry )
	{
		const int y = calculateEntryY( entry );
		const int theme = getThemeFromLevel( level );

		if ( entry == selection_ )
		{
			renderLevelName( level );

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

			if ( Inventory::hasCrown( level ) )
			{
				renderCrownWinIcon( y );
			}
			else
			{
				renderEmptyCrownIcon( y );
			}

			if ( Level::hasSecretGoal( level ) )
			{
				if ( Inventory::getSecretGoal( level ) )
				{
					renderSecretGoalIcon( y );
				}
				else
				{
					renderEmptySecretGoalIcon( y );
				}
			}
		}

		if ( show_target_scores_ && entry == selection_ )
		{
			renderPtTargetSymbol( y );
			renderClockTargetSymbol( y );
			renderGemTargetScore( level, y );
			renderTimeTargetScore( level, y );
		}
		else
		{
			if ( entry == selection_ )
			{
				renderPtSymbol( y );
				renderGemScore( level, y );
			}

			if ( entry == selection_ )
			{
				renderClockSymbol( y );
				renderTimeScore( level, y );
			}
		}

		++level;
	}
	static_page_texture_.endDrawing();
	renderFlashFrame();
};

void LevelSelectState::drawFlippedTexture()
{
	final_table_texture_.startDrawing();
	Render::clearScreenTransparency();
	current_page_texture_->render();
	next_page_texture_->render();
	final_table_texture_.endDrawing();
}

void LevelSelectState::renderSelectHighlight() const
{
	if ( selection_ == 0 )
	{
		Render::renderObject( "bg/level-select-bar.png", { 0, 0, 392, 24 }, { 4, 24, 392, 24 } );
	}
	else if ( selection_ == LEVELS_PER_PAGE - 1 )
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
};

WTextCharacter::Color LevelSelectState::flashOnCondition( bool condition ) const
{
	return ( condition )
		? ( WTextCharacter::Color )( FLASH_FRAMES[ flash_frame_ ] )
		: WTextCharacter::Color::DARK_GRAY;
};

void LevelSelectState::animateTextFlashColor()
{
	if ( flash_timer_ > FLASH_SPEED - 1 )
	{
		flash_timer_ = 0;
		++flash_frame_;
		if ( flash_frame_ == NUMBER_OF_FLASH_FRAMES )
		{
			flash_frame_ = 0;
		}
		renderFlashFrame();
	}
	else
	{
		++flash_timer_;
	}
};

void LevelSelectState::setInfoToNextPage()
{
	page_ = next_page_;
	const int cycle = ( int )( std::floor( ( double )( page_ ) / ( double )( PAGES_PER_CYCLE ) ) ) + 1;
	first_level_of_page_ = ( ( cycle - 1 ) * Level::NUMBER_OF_THEMES ) + ( ( page_ % PAGES_PER_CYCLE ) * LEVELS_PER_PAGE );
}

void LevelSelectState::renderFlashingPtSymbol( int y ) const
{
	renderPtSymbolOfColorOffset( FLASH_FRAMES[ flash_frame_ ], y );
};

void LevelSelectState::renderFlashingClockSymbol( int y ) const
{
	renderClockSymbolOfColorOffset( FLASH_FRAMES[ flash_frame_ ], y );
};

void LevelSelectState::renderLevelNameOfColor( WTextCharacter::Color color, int level )
{
	level_name_headers_[ level ].changeColor( color );
	level_name_headers_[ level ].render();
	level_names_[ level ].changeColor( color );
	level_names_[ level ].render();
};

void LevelSelectState::renderLevelName( int level )
{
	renderLevelNameOfColor( WTextCharacter::Color::DARK_GRAY, level );
};

void LevelSelectState::renderFlashingLevelName( int level )
{
	renderLevelNameOfColor( ( WTextCharacter::Color )( FLASH_FRAMES[ flash_frame_ ] ), level );
};

void LevelSelectState::renderFlashingVictoryCheck( int y ) const
{
	renderVictoryCheckOfColorOffset( FLASH_FRAMES[ flash_frame_ ], y );
};

void LevelSelectState::renderFlashingDiamondWinIcon( int y ) const
{
	renderDiamondIconOfColorOffset( FLASH_FRAMES[ flash_frame_ ], y );
};

void LevelSelectState::renderFlashingCrownWinIcon( int y ) const
{
	renderCrownIconOfColorOffset( FLASH_FRAMES[ flash_frame_ ], y );
};

void LevelSelectState::renderFlashingGemScore( int level, int y ) const
{
	renderGemScoreOfColor( ( WTextCharacter::Color )( FLASH_FRAMES[ flash_frame_ ] ), level, y );
};

void LevelSelectState::renderFlashingTimeScore( int level, int y ) const
{
	renderTimeScoreOfColor( ( WTextCharacter::Color )( FLASH_FRAMES[ flash_frame_ ] ), level, y );
};

void LevelSelectState::renderFlashingSecretGoalIcon( int y ) const
{
	renderSecretGoalIconOfColor( FLASH_FRAMES[ flash_frame_ ], y );
};

void LevelSelectState::renderFlashingArrows() const
{
	if ( flash_frame_ % ( int )( ( double )( NUMBER_OF_FLASH_FRAMES ) / 2.0 ) > 2 )
	{
		Render::renderObject( "bg/level-select-characters.png", { 16, 16, 16, 16 }, { 0, 116, 16, 16 } );
		Render::renderObject( "bg/level-select-characters.png", { 16, 16, 16, 16 }, { Unit::WINDOW_WIDTH_PIXELS - 16, 116, 16, 16 }, SDL_FLIP_HORIZONTAL );
	}
};

void LevelSelectState::renderFlashFrame()
{
	final_table_texture_.startDrawing();
	Render::clearScreenTransparency();
	static_page_texture_.render();
	int level = first_level_of_page_;
	for ( int entry = 0; entry < calculateNumberOfLevelsInPage( page_ ); ++entry )
	{
		const int y = calculateEntryY( entry );
		const int theme = getThemeFromLevel( level );

		if ( Inventory::levelComplete( level ) )
		{
			renderFlashingLevelName( level );
			renderFlashingVictoryCheck( y );
			renderFlashingDiamondWinIcon( y );
			renderFlashingCrownWinIcon( y );
			if ( Level::hasSecretGoal( level ) )
			{
				renderFlashingSecretGoalIcon( y );
			}
		}

		if ( !show_target_scores_ || entry != selection_ )
		{
			if ( Inventory::gemChallengeBeaten( level ) )
			{
				renderFlashingPtSymbol( y );
				renderFlashingGemScore( level, y );
			}

			if ( Inventory::timeChallengeBeaten( level ) )
			{
				renderFlashingClockSymbol( y );
				renderFlashingTimeScore( level, y );
			}
		}

		++level;
	}
	renderFlashingArrows();
	final_table_texture_.endDrawing();
};

void LevelSelectState::generateLevelNames()
{
	int theme_positions[ Level::NUMBER_OF_THEMES ];
	int entry_per_page = 0;
	for ( int entry = 0; entry < Level::NUMBER_OF_THEMES; ++entry )
	{
		theme_positions[ entry ] = calculateEntryY( entry_per_page );
		++entry_per_page;
		if ( entry_per_page == LEVELS_PER_PAGE )
		{
			entry_per_page = 0;
		}
	};

	for ( int level = 0; level < Level::NUMBER_OF_LEVELS; ++level )
	{
		const int cycle = ( int )( std::floor( ( double )( level ) / ( double )( Level::NUMBER_OF_THEMES ) ) ) + 1;
		const int theme = level % Level::NUMBER_OF_THEMES;
		const std::u32string& level_name = Localization::getCurrentLanguage().getLevelName( Level::getCodeName( level ) );
		std::u32string level_name_string = U"";
		if ( Inventory::beenToLevel( level ) )
		{
			level_name_string = level_name;
		}
		else
		{
			const int number_of_levels = ( int )( level_name.size() );
			for ( int i = 0; i < number_of_levels; ++i )
			{
				if ( level_name[ i ] == char32_t( ' ' ) )
				{
					level_name_string += U" ";
				}
				else
				{
					level_name_string += U"?";
				}
			}
		}
		level_name_headers_[ level ] = { Level::getLevelHeader( level ), 24, theme_positions[ theme ], WTextCharacter::Color::DARK_GRAY, Unit::WINDOW_WIDTH_PIXELS, WTextObj::Align::LEFT, WTextCharacter::Color::__NULL, 4, 4 };
		const int padding = ( level_name_headers_[ level ].getNumberOfCharacters() + 1 ) * WTextCharacter::SIZE_PIXELS;
		level_names_[ level ] = { level_name_string, 24 + padding, theme_positions[ theme ], WTextCharacter::Color::DARK_GRAY, 320 - padding, WTextObj::Align::LEFT, WTextCharacter::Color::__NULL, 4, 4 };
	}
};

int LevelSelectState::getSelectedLevel() const
{
	return first_level_of_page_ + selection_;
};

static void renderGemScoreOfColor( WTextCharacter::Color color, int level, int y )
{
	WTextObj gem_score{ Inventory::gemScore( level ), 352, y, color, 312, WTextObj::Align::LEFT, WTextCharacter::Color::__NULL, 0, 4 };
	gem_score.render();
};

static void renderTimeScoreOfColor( WTextCharacter::Color color, int level, int y )
{
	WTextObj time_score{ Inventory::timeScore( level ), 360, y + 8, color, 312, WTextObj::Align::LEFT, WTextCharacter::Color::__NULL, 0, 4 };
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

static void renderCrownIconOfColorOffset( int color_offset, int y )
{
	Render::renderObject( "bg/level-select-characters.png", { color_offset * 8, 192, 8, 8 }, { 16, y + 4, 8, 8 } );
};

static void renderSecretGoalIconOfColor( int color_offset, int y )
{
	Render::renderObject( "bg/level-select-characters.png", { color_offset * 8, 200, 8, 8 }, { 16, y + 12, 8, 8 } );
};

static void renderCrownWinIcon( int y )
{
	renderCrownIconOfColorOffset( 3, y );
};

static void renderEmptyCrownIcon( int y )
{
	renderCrownIconOfColorOffset( 1, y );
};

static void renderSecretGoalIcon( int y )
{
	renderSecretGoalIconOfColor( 3, y );
};

static void renderEmptySecretGoalIcon( int y )
{
	renderSecretGoalIconOfColor( 1, y );
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

static int getThemeFromLevel( int level )
{
	return level % Level::NUMBER_OF_THEMES;
};

static void renderGemTargetScore( int level, int y )
{
	WTextObj gem_score{ Level::gemChallengeText( level ), 352, y, WTextCharacter::Color::LIGHT_MID_GRAY, 312, WTextObj::Align::LEFT, WTextCharacter::Color::__NULL, 0, 4 };
	gem_score.render();
};

static void renderTimeTargetScore( int level, int y )
{
	WTextObj time_score{ Level::timeChallengeText( level ), 360, y + 8, WTextCharacter::Color::LIGHT_MID_GRAY, 312, WTextObj::Align::LEFT, WTextCharacter::Color::__NULL, 0, 4 };
	time_score.render();
};

static void renderPtTargetSymbol( int y )
{
	renderPtSymbolOfColorOffset( 3, y );
};

static void renderClockTargetSymbol( int y )
{
	renderClockSymbolOfColorOffset( 3, y );
};