#include "audio.hpp"
#include "input.hpp"
#include "inventory.hpp"
#include "level.hpp"
#include "ow_inventory.hpp"
#include "render.hpp"

static constexpr sdl2::SDLRect AREA = { 0, Unit::WINDOW_HEIGHT_PIXELS - OWInventory::HEIGHT, Unit::WINDOW_WIDTH_PIXELS, OWInventory::HEIGHT };
static constexpr int ROW_1 = AREA.y + 8;
static constexpr int ROW_2 = AREA.y + 24;
static constexpr int LEFT_EDGE = AREA.x + 8;
static constexpr int RIGHT_EDGE = AREA.w - 8;
static constexpr int LEVEL_NAME_X = LEFT_EDGE + 8;
static constexpr int FLASH_SPEED = 4;
static constexpr int HAVE_X = 48;
static constexpr int DONT_HAVE_X = 56;
// From dark to light, delaying a li’l @ either end.
static constexpr int NUMBER_OF_FLASH_FRAMES = 12;
static constexpr int FLASH_FRAMES[ NUMBER_OF_FLASH_FRAMES ] =
{
	0, 1, 2, 3, 4, 5, 5, 4, 3, 2, 1, 0
};

OWInventory::OWInventory()
:
	color_animation_ ( 0 ),
	color_animation_timer_ ( 0 ),
	prev_level_ ( 0 ),
	show_challenges_ ( false ),
	show_challenges_lock_ ( true ),
	color_going_up_ ( true ),
	sound_lock_ ( false ),
	level_ ( 0 ),
	negative_pts_icon_ ( "bg/level-select-characters.png", { 8, 0, 8, 8 }, { LEFT_EDGE, ROW_2, 8, 8 } ),
	win_icon_ ( "bg/level-select-characters.png", { 0, 16, 8, 8 }, { LEFT_EDGE, ROW_1, 8, 8 } ),
	diamond_icon_ ( "bg/level-select-characters.png", { 0, 24, 8, 8 }, { LEFT_EDGE, ROW_1+8, 8, 8 } ),
	level_name_ ()
{};

void OWInventory::update( int level )
{
	level_ = level;
	Inventory::update();

	if ( !Input::held( Input::Action::CANCEL ) )
	{
		show_challenges_lock_ = false;
	}
	else if ( level <= 0 )
	{
		show_challenges_lock_ = true;
	}

	if ( !show_challenges_lock_ && Input::held( Input::Action::CANCEL ) )
	{
		show_challenges_ = true;
		if ( !sound_lock_ )
		{
			Audio::playSound( Audio::SoundType::SELECT );
			sound_lock_ = true;
		}
	}
	else
	{
		show_challenges_ = false;
		sound_lock_ = false;
	}

	if ( color_animation_timer_ == FLASH_SPEED )
	{
		color_animation_timer_ = 0;
		if ( color_animation_ == NUMBER_OF_FLASH_FRAMES )
		{
			color_animation_ = 0;
		}
		else
		{
			++color_animation_;
		}
	}
	else
	{
		++color_animation_timer_;
	}

	if ( prev_level_ != level_ && level_ > -1 )
	{
		win_icon_.src_.y = ( Inventory::victory( level_ ) ) ? 40 : 16;
		diamond_icon_.src_.y = ( Inventory::haveDiamond( level_ ) ) ? 32 : 24;
		level_name_ = { Level::getLevelNames()[ level_ ], LEVEL_NAME_X, ROW_1, WTextCharacter::Color::BLACK, WTextObj::DEFAULT_WIDTH, WTextObj::Align::LEFT, WTextCharacter::Color::__NULL, 8 };
		prev_level_ = level_;
	}
};

void OWInventory::render()
{
	Render::renderObject( "bg/overworld-level-name-frame.png", { 0, 0, Unit::WINDOW_WIDTH_PIXELS, 40 }, AREA );
	renderLevelInfo();
	renderPts();
};

void OWInventory::renderLevelInfo()
{
	if ( level_ > -1 )
	{
		/*
		Text::FontColor name_shade = Text::FontColor::BLACK;
		Text::FontColor gem_shade = Text::FontColor::BLACK;
		Text::FontColor time_shade = Text::FontColor::BLACK;
		std::string gem_score;
		std::string time_score;

		if ( show_challenges_ )
		{
			gem_score  = Level::gemChallengeText( level_ );
			time_score = Level::timeChallengeText( level_ );
			name_shade = Text::FontColor::DARK_MID_GRAY;
			gem_shade = Text::FontColor::DARK_MID_GRAY;
			time_shade = Text::FontColor::DARK_MID_GRAY;
		}
		else
		{
			gem_score  = Inventory::gemScore( level_ );
			time_score = Inventory::timeScore( level_ );

			if ( Inventory::levelComplete( level_ ) )
			{
				name_shade = ( Text::FontColor )( FLASH_FRAMES[ color_animation_ ] );
			}

			if ( Inventory::gemChallengeBeaten( level_ ) )
			{
				gem_shade = ( Text::FontColor )( FLASH_FRAMES[ color_animation_ ] );
			}

			if ( Inventory::timeChallengeBeaten( level_ ) )
			{
				time_shade = ( Text::FontColor )( FLASH_FRAMES[ color_animation_ ] );
			}
		}

		if ( prev_level_ != level_ )
		{
			level_name_ = { Text::autoformat( Level::NameOLevel( ( unsigned int )( level_ ) ), 39 ), LEFT_EDGE + 16, ROW_1, name_shade, Text::FontAlign::LEFT, Text::FontColor::__NULL, false, 39 };
			prev_level_ = level_;
		}
		level_name_.render( nullptr );

		gem_icon_gfx_.render( gem_icon_pos_ );
		Text::renderText( gem_score, RIGHT_EDGE - ( 5 * 8 ), ROW_1, nullptr, gem_shade );

		time_icon_gfx_.render( time_icon_pos_ );
		Text::renderText( time_score, RIGHT_EDGE - ( 4 * 8 ), ROW_1 + 8, nullptr, time_shade );*/

		win_icon_.render();
		diamond_icon_.render();
		level_name_.render();
	}
}

void OWInventory::renderPts()
{
	Text::FontColor color = Text::FontColor::BLACK;
	if ( Inventory::totalFundsShown() < 0 )
	{
		color = Text::FontColor::DARK_GRAY;
		negative_pts_icon_.render();
	}
	Text::renderNumber( Inventory::totalFundsShown(), AREA.x + 24, ROW_2, 9, color );
}
