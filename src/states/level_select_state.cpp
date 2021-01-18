#include "audio.hpp"
#include "main.hpp"
#include "input.hpp"
#include "inventory.hpp"
#include "level_data.hpp"
#include "level_list.hpp"
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
static constexpr int GRID_Y = 20;
static constexpr int ROW_HEIGHT = 24;
static constexpr int calculateEntryY( int entry )
{
	return entry * ROW_HEIGHT + GRID_Y;
};

static constexpr const char* PALETTES[ 4 ]
{
	"Pale Green",
	"Pale Blue",
	"Pale Purple",
	"Pale Red"
};

LevelSelectState::LevelSelectState( int current_level )
:
	GameState ( StateID::LEVEL_SELECT_STATE, { "Pale Red", 1 } ),
	show_target_scores_ ( false ),
	page_change_direction_ ( Direction::Horizontal::__NULL ),
	selection_timer_ ( 1 ),
	selection_ ( 0 ),
	current_page_ ( 0 ),
	next_page_ ( 1 ),
	flash_timer_ ( 0 ),
	flash_frame_ ( 0 ),
	percent_ ( Inventory::percent() ),
	background_ (),
	current_page_texture_ ( Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS, 0, 0 ),
	next_page_texture_ ( Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS, 0, 0 ),
	final_table_texture_ ( Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS, 0, 0 ),
	title_ ({ Localization::getCurrentLanguage().getLevelSelectTitle(), 0, 0, WTextCharacter::Color::LIGHT_GRAY, Unit::WINDOW_WIDTH_PIXELS, WTextObj::Align::CENTER, WTextCharacter::Color::BLACK, 8, 6 }),
	instruction_ ( mezun::stringReplace( mezun::charToChar32String( "Press %k to show the time & gem score challenge for the selected level if it’s unlocked." ), U"%k", Input::generateKeyNames()[ ( int )( Input::Action::CANCEL ) ] ), Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS - 10, WTextCharacter::Color::WHITE, Unit::WINDOW_WIDTH_PIXELS * 4, WTextObj::Align::LEFT, WTextCharacter::Color::BLACK, 8 ),
	completion_ ( Inventory::percentShown(), 0, GRID_Y - 16, WTextCharacter::Color::DARK_GRAY, Unit::WINDOW_WIDTH_PIXELS, WTextObj::Align::RIGHT, WTextCharacter::Color::__NULL, 8, 4 )
{
	Audio::changeSong( "level-select" );
};

LevelSelectState::~LevelSelectState()
{
	current_page_texture_.destroy();
	next_page_texture_.destroy();
	final_table_texture_.destroy();
};

void LevelSelectState::stateUpdate()
{
	background_.update();
	title_.update();
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
				if ( current_page_texture_.getX() >= Unit::WINDOW_WIDTH_PIXELS )
				{
					finishPageFlip();
				}
				else
				{
					current_page_texture_.moveRight( PAGE_MOVE_SPEED );
					next_page_texture_.moveRight( PAGE_MOVE_SPEED );
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
				if ( current_page_texture_.getX() <= -Unit::WINDOW_WIDTH_PIXELS )
				{
					finishPageFlip();
				}
				else
				{
					current_page_texture_.moveLeft( PAGE_MOVE_SPEED );
					next_page_texture_.moveLeft( PAGE_MOVE_SPEED );
					drawFlippedTexture();
				}
			}
			break;

			case ( Direction::Horizontal::__NULL ):
			{
				handleTargetScoreBehavior();
				redrawCurrentPage();

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
						if ( selection_ >= pages_[ current_page_ ].number_of_levels_ )
						{
							selection_ = 0;
						}
						selection_timer_ = INPUT_DELAY;
						//redrawStaticPageTexture();
						Audio::playSound( Audio::SoundType::SELECT );
					}
					else if ( Input::held( Input::Action::MOVE_UP ) )
					{
						--selection_;
						if ( selection_ < 0 )
						{
							selection_ = pages_[ current_page_ ].number_of_levels_ - 1;
						}
						selection_timer_ = INPUT_DELAY;
						//redrawStaticPageTexture();
						Audio::playSound( Audio::SoundType::SELECT );
					}
				}
				else
				{
					--selection_timer_;
				}

				animateTextFlashColor();
				instruction_.addToX( -2 );
				if ( instruction_.lines_[ 0 ].x_ < -( instruction_.getNumberOfCharacters() * WTextCharacter::SIZE_PIXELS ) )
				{
					resetInstructionPosition();
				}
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
	// Sort levels by cycle.
	std::vector<std::vector<const LevelData*>> cycle_levels;
	const auto& levels = LevelList::getLevelDataList();
	for ( const auto& level : levels )
	{
		while ( cycle_levels.size() < ( unsigned int )( level.cycle_ ) )
		{
			cycle_levels.emplace_back();
		}
		if ( level.cycle_ > 0 )
		{
			cycle_levels[ level.cycle_ - 1 ].emplace_back( &level );
		}
	}

	// Sort each cycle list by theme.
	for ( auto& cycle : cycle_levels )
	{
		std::sort
		(
			cycle.begin(),
			cycle.end(),
			[]( const LevelData* a, const LevelData* b )
			{
				return a->theme_ < b->theme_;
			}
		);
	}

	// Generate max cycle.
	int max_cycle = 1;
	for ( int i = 0; i < cycle_levels.size(); ++i )
	{
		for ( int j = 0; j < cycle_levels[ i ].size(); ++j )
		{
			if ( Inventory::beenToLevel( cycle_levels[ i ][ j ]->id_ ) )
			{
				max_cycle = std::max( max_cycle, i + 1 );
			}
		}
	}

	// Generate pages from cycle level lists.
	for ( int i = 0; i < max_cycle; ++i )
	{
		pages_.emplace_back();
		pages_.back().number_of_levels_ = 0;
		pages_.back().cycle_ = i + 1;
		for ( int j = 0; j < cycle_levels[ i ].size(); ++j )
		{
			if ( pages_.back().number_of_levels_ == LevelSelectPage::MAX_LEVELS )
			{
				pages_.emplace_back();
				pages_.back().number_of_levels_ = 0;
				pages_.back().cycle_ = i + 1;
			}
			pages_.back().levels_[ pages_.back().number_of_levels_ ] = *cycle_levels[ i ][ j ];
			++pages_.back().number_of_levels_;
		}
		if ( pages_.back().number_of_levels_ == 0 )
		{
			pages_.pop_back();
		}
	}

	// Set selection to current level.
	for ( int i = 0; i < pages_.size(); ++i )
	{
		for ( int j = 0; j < pages_[ i ].number_of_levels_; ++j )
		{
			if ( pages_[ i ].levels_[ j ].id_ == Inventory::currentLevel() )
			{
				current_page_ = i;
				selection_ = j;
			}
		}
	}

	// Initialize textures.
	updatePalette();
	current_page_texture_.init();
	next_page_texture_.init();
	final_table_texture_.init();
	renderPageToTexture( current_page_texture_, pages_[ current_page_ ] );
	final_table_texture_.startDrawing();
	Render::clearScreenTransparency();
	current_page_texture_.render();
	final_table_texture_.endDrawing();
};

unsigned int LevelSelectState::getSelectedLevel() const
{
	return pages_[ current_page_ ].levels_[ selection_ ].id_;
};

void LevelSelectState::renderPageToTexture( const TextureBox& texture, const LevelSelectPage& page, bool render_select ) const
{
	texture.startDrawing();
	Render::clearScreenTransparency();
	renderPage( page, render_select );
	texture.endDrawing();
};

void LevelSelectState::renderPage( const LevelSelectPage& page, bool render_select ) const
{
	int y = GRID_Y;
	for ( int i = 0; i < page.number_of_levels_; ++i )
	{
		const int source_y = ( render_select && i == selection_ ) ? 32 : ( i % 2 == 1 ) ? 26 : 20;
		Render::renderObject( "bg/level-select-bg-graphics.png", { 0, source_y, 394, 1 }, { 4, y, 394, 19 } );

		if ( i == page.number_of_levels_ - 1 )
		{
			Render::renderObject( "bg/level-select-bg-graphics.png", { 0, source_y + 1, 394, 5 }, { 4, y + 19, 394, 5 } );
		}
		else
		{
			Render::renderObject( "bg/level-select-bg-graphics.png", { 0, source_y, 394, 1 }, { 4, y + 19, 394, 5 } );
		}
		y += ROW_HEIGHT;
	}
	Render::renderObject( "bg/level-select-bg-graphics.png", { 0, 0, 394, 20 }, { 4, GRID_Y - 16, 394, 20 } );
	if ( render_select && selection_ == 0 )
	{
		Render::renderObject( "bg/level-select-bg-graphics.png", { 0, 38, 394, 5 }, { 4, GRID_Y - 1, 394, 5 } );
	}

	const WTextObj cycle { mezun::merge32Strings( Localization::getCurrentLanguage().getLevelSelectCycleName(), mezun::merge32Strings( U" ", mezun::intToChar32String( page.cycle_ ) ) ), 4, GRID_Y - 16, WTextCharacter::Color::DARK_GRAY, Unit::WINDOW_WIDTH_PIXELS, WTextObj::Align::LEFT, WTextCharacter::Color::__NULL, 4, 4 };
	cycle.render();

	completion_.render();

	y = GRID_Y;
	for ( int i = 0; i < page.number_of_levels_; ++i )
	{
		const LevelData& level = page.levels_[ i ];

		// Render level name.
		const std::u32string& level_name = Localization::getCurrentLanguage().getLevelName( level.code_name_ );
		std::u32string level_name_string = U"";
		if ( Inventory::beenToLevel( level.id_ ) )
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

		const WTextCharacter::Color color = ( render_select && Inventory::levelComplete( level.id_ ) ) ? ( WTextCharacter::Color )( FLASH_FRAMES[ flash_frame_ ] ) : WTextCharacter::Color::DARK_GRAY;
		const WTextObj level_name_header { LevelList::getLevelHeader( level.id_ ), 24, y, color, Unit::WINDOW_WIDTH_PIXELS, WTextObj::Align::LEFT, WTextCharacter::Color::__NULL, 4, 4 };
		level_name_header.render();
		const int padding = ( level_name_header.getNumberOfCharacters() == 0 ) ? 0 : ( level_name_header.getNumberOfCharacters() + 1 ) * WTextCharacter::SIZE_PIXELS;
		const WTextObj level_name_text { level_name_string, 24 + padding, y, color, 320 - padding, WTextObj::Align::LEFT, WTextCharacter::Color::__NULL, 4, 4 };
		level_name_text.render();

		// Render victory.
		if ( Inventory::victory( level.id_ ) )
		{
			const int color_offset = ( render_select && Inventory::levelComplete( level.id_ ) ) ? FLASH_FRAMES[ flash_frame_ ] : 3;
			Render::renderObject( "bg/level-select-characters.png", { color_offset * 8, 40, 8, 8 }, { 8, y + 4, 8, 8 } );
		}
		else
		{
			Render::renderObject( "bg/level-select-characters.png", { 8, 16, 8, 8 }, { 8, y + 4, 8, 8 } );
		}

		// Render card.
		if ( level.has_card_ )
		{
			if ( Inventory::haveDiamond( level.id_ ) )
			{
				const int color_offset = ( render_select && Inventory::levelComplete( level.id_ ) ) ? FLASH_FRAMES[ flash_frame_ ] : 3;
				Render::renderObject( "bg/level-select-characters.png", { color_offset * 8, 32, 8, 8 }, { 8, y + 12, 8, 8 } );
			}
			else
			{
				Render::renderObject( "bg/level-select-characters.png", { 8, 24, 8, 8 }, { 8, y + 12, 8, 8 } );
			}
		}

		// Render secret exit.
		if ( level.has_secret_goal_ )
		{
			const int color_offset = ( render_select && Inventory::getSecretGoal( level.id_ ) ) ? ( ( Inventory::levelComplete( level.id_ ) ) ? FLASH_FRAMES[ flash_frame_ ] : 3 ) : 1;
			Render::renderObject( "bg/level-select-characters.png", { color_offset * 8, 200, 8, 8 }, { 16, y + 12, 8, 8 } );
		}

		const bool show_top_score = render_select && i == selection_ && show_target_scores_;
		// Render ₧ score.
		if ( LevelList::hasGemScore( level.id_ ) )
		{
			const int color_offset = ( show_top_score ) ? 2 : ( ( render_select && Inventory::gemChallengeBeaten( level.id_ ) ) ? FLASH_FRAMES[ flash_frame_ ] : 1 );
			Render::renderObject( "bg/level-select-characters.png", { color_offset * 8, 0, 8, 8 }, { 344, y + 4, 8, 8 } );
			const WTextObj gem_score{ ( show_top_score ) ? LevelList::gemChallengeText( level.id_ ) : Inventory::gemScore( level.id_ ), 352, y, ( WTextCharacter::Color )( color_offset ), 312, WTextObj::Align::LEFT, WTextCharacter::Color::__NULL, 0, 4 };
			gem_score.render();
		}

		// Render time score.
		if ( LevelList::hasTimeScore( level.id_ ) )
		{
			const int color_offset = ( show_top_score ) ? 2 : ( ( render_select && Inventory::timeChallengeBeaten( level.id_ ) ) ? FLASH_FRAMES[ flash_frame_ ] : 1 );
			Render::renderObject( "bg/level-select-characters.png", { color_offset * 8, 8, 8, 8 }, { 352, y + 12, 8, 8 } );
			const WTextObj time_score{ ( show_top_score ) ? LevelList::timeChallengeText( level.id_ ) : Inventory::timeScore( level.id_ ), 360, y + 8, ( WTextCharacter::Color )( color_offset ), 312, WTextObj::Align::LEFT, WTextCharacter::Color::__NULL, 0, 4 };
			time_score.render();
		}

		y += ROW_HEIGHT;
	}

	if ( render_select )
	{
		instruction_.render();
		renderFlashingArrows();
	}
};

void LevelSelectState::flipPage( Direction::Horizontal direction, int next_page, int position )
{
	page_change_direction_ = direction;
	next_page_ = next_page;
	renderPageToTexture( current_page_texture_, pages_[ current_page_ ], false );
	renderPageToTexture( next_page_texture_, pages_[ next_page_ ], false );
	next_page_texture_.setX( position );
	Audio::playSound( Audio::SoundType::PAGE );
};

void LevelSelectState::flipPageLeft()
{
	const int next_page = ( current_page_ == 0 ) ? pages_.size() - 1 : current_page_ - 1;
	flipPage( Direction::Horizontal::LEFT, next_page, -Unit::WINDOW_WIDTH_PIXELS );
};

void LevelSelectState::flipPageRight()
{
	const int next_page = ( current_page_ == pages_.size() - 1 ) ? 0 : current_page_ + 1;
	flipPage( Direction::Horizontal::RIGHT, next_page, Unit::WINDOW_WIDTH_PIXELS );
};

void LevelSelectState::finishPageFlip()
{
	flash_frame_ = 0;
	resetInstructionPosition();
	current_page_ = next_page_;
	updatePalette();
	current_page_texture_.setX( 0 );
	redrawCurrentPage();
	page_change_direction_ = Direction::Horizontal::__NULL;
	if ( selection_ >= pages_[ current_page_ ].number_of_levels_ )
	{
		selection_ = pages_[ current_page_ ].number_of_levels_ - 1;
	}
};

void LevelSelectState::drawFlippedTexture() const
{
	final_table_texture_.startDrawing();
	Render::clearScreenTransparency();
	current_page_texture_.render();
	next_page_texture_.render();
	final_table_texture_.endDrawing();
};

void LevelSelectState::redrawCurrentPage() const
{
	renderPageToTexture( current_page_texture_, pages_[ current_page_ ] );
	final_table_texture_.startDrawing();
	Render::clearScreenTransparency();
	current_page_texture_.render();
	final_table_texture_.endDrawing();
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
		const WTextCharacter::Color completion_color = ( percent_ >= 1.0 ) ? ( WTextCharacter::Color )( flash_frame_ ) : WTextCharacter::Color::DARK_GRAY;
		completion_.changeColor( completion_color );
	}
	else
	{
		++flash_timer_;
	}
};

void LevelSelectState::renderFlashingArrows() const
{
	if ( flash_frame_ % ( int )( ( double )( NUMBER_OF_FLASH_FRAMES ) / 2.0 ) > 2 )
	{
		Render::renderObject( "bg/level-select-characters.png", { 16, 16, 16, 16 }, { 0, 116, 16, 16 } );
		Render::renderObject( "bg/level-select-characters.png", { 16, 16, 16, 16 }, { Unit::WINDOW_WIDTH_PIXELS - 16, 116, 16, 16 }, SDL_FLIP_HORIZONTAL );
	}
};

void LevelSelectState::handleTargetScoreBehavior()
{
	if ( !show_target_scores_ )
	{
		if ( Input::held( Input::Action::CANCEL ) && Inventory::beenToLevel( getSelectedLevel() ) )
		{
			show_target_scores_ = true;
			Audio::playSound( Audio::SoundType::SELECT );
		}
	}
	else
	{
		if ( !Input::held( Input::Action::CANCEL ) )
		{
			show_target_scores_ = false;
		}
	}
};

void LevelSelectState::updatePalette()
{
	newPalette( PALETTES[ ( pages_[ current_page_ ].cycle_ - 1 ) % 4 ] );
};

void LevelSelectState::resetInstructionPosition()
{
	instruction_.setX( Unit::WINDOW_WIDTH_PIXELS );
};