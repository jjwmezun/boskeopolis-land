#include "audio.hpp"
#include "input.hpp"
#include "inventory.hpp"
#include "level.hpp"
#include "ow_inventory.hpp"
#include "render.hpp"

OWInventory::OWInventory()
:
	win_icon_gfx_ ( "tilesets/universal.png", HAVE_X, 8 ),
	diamond_gfx_ ( "tilesets/universal.png", HAVE_X, 0 ),
	gem_icon_gfx_ ( "charset.png", 32, 16 ),
	time_icon_gfx_ ( "tilesets/universal.png", 40, 0 ),
	level_name_ ( "" ),
	area_ ( AREA ),
	win_icon_pos_ ( LEFT_EDGE, ROW_1, 8, 8 ),
	diamond_icon_pos_ ( LEFT_EDGE, ROW_1+8, 8, 8 ),
	gem_icon_pos_ ( RIGHT_EDGE - ( 6 * 8 ), ROW_1, 8, 8 ),
	time_icon_pos_ ( RIGHT_EDGE - ( 5 * 8 ), ROW_1 + 8, 8, 8 ),
	pts_icon_pos_ ( LEFT_EDGE, ROW_2, 8, 8 ),
	color_animation_ ( COLOR_START ),
	color_animation_timer_ ( 0 ),
	prev_level_select_ ( 0 ),
	show_challenges_ ( false ),
	show_challenges_lock_ ( true ),
	color_going_up_ ( true ),
	sound_lock_ ( false )
{};

void OWInventory::update( int lv_select )
{
	Inventory::update();

	if ( !Input::held( Input::Action::CANCEL ) )
	{
		show_challenges_lock_ = false;
	}
	else if ( lv_select <= 0 )
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

	if ( color_animation_timer_ >= COLOR_TIMER_LIMIT )
	{
		color_animation_timer_ = 0;
		if ( color_going_up_ )
		{
			++color_animation_;
			if ( color_animation_ > COLOR_END )
			{
				color_animation_ = COLOR_END;
				color_going_up_ = false;
			}
		}
		else
		{
			--color_animation_;
			if ( color_animation_ < COLOR_START )
			{
				color_animation_ = COLOR_START;
				color_going_up_ = true;
			}
		}
	}
	else
	{
		++color_animation_timer_;
	}
};

void OWInventory::render( int lv_select )
{
	Render::renderRect( area_ );
	renderLevelInfo( lv_select );
	renderPts();
};

void OWInventory::renderLevelInfo( int lv_select )
{
	if ( lv_select > 0 )
	{
		Text::FontColor name_shade = Text::FontColor::BLACK;
		Text::FontColor gem_shade = Text::FontColor::BLACK;
		Text::FontColor time_shade = Text::FontColor::BLACK;
		std::string gem_score;
		std::string time_score;

		if ( show_challenges_ )
		{
			gem_score  = Level::gemChallengeText( lv_select );
			time_score = Level::timeChallengeText( lv_select );
			name_shade = Text::FontColor::DARK_MID_GRAY;
			gem_shade = Text::FontColor::DARK_MID_GRAY;
			time_shade = Text::FontColor::DARK_MID_GRAY;
		}
		else
		{
			gem_score  = Inventory::gemScore( lv_select );
			time_score = Inventory::timeScore( lv_select );

			if ( Inventory::levelComplete( lv_select ) )
			{
				name_shade = ( Text::FontColor )( color_animation_ );
			}

			if ( Inventory::gemChallengeBeaten( lv_select ) )
			{
				gem_shade = ( Text::FontColor )( color_animation_ );
			}

			if ( Inventory::timeChallengeBeaten( lv_select ) )
			{
				time_shade = ( Text::FontColor )( color_animation_ );
			}
		}

		if ( prev_level_select_ != lv_select )
		{
			level_name_ = { Text::autoformat( Level::NameOLevel( ( unsigned int )( lv_select ) ), 39 ), LEFT_EDGE + 16, ROW_1, name_shade, Text::FontAlign::LEFT, Text::FontColor::__NULL, false, 39 };
			prev_level_select_ = lv_select;
		}
		level_name_.render( nullptr );

		gem_icon_gfx_.render( gem_icon_pos_ );
		Text::renderText( gem_score, RIGHT_EDGE - ( 5 * 8 ), ROW_1, nullptr, gem_shade );

		time_icon_gfx_.render( time_icon_pos_ );
		Text::renderText( time_score, RIGHT_EDGE - ( 4 * 8 ), ROW_1 + 8, nullptr, time_shade );

		win_icon_gfx_.current_frame_x_ = DONT_HAVE_X;
		if ( Inventory::victory( lv_select ) )
		{
			win_icon_gfx_.current_frame_x_ = HAVE_X;
		}
		win_icon_gfx_.render( win_icon_pos_ );

		diamond_gfx_.current_frame_x_ = DONT_HAVE_X;
		if ( Inventory::haveDiamond( lv_select ) )
		{
			diamond_gfx_.current_frame_x_ = HAVE_X;
		}
		diamond_gfx_.render( diamond_icon_pos_ );
	}
}

void OWInventory::renderPts()
{
	gem_icon_gfx_.render( pts_icon_pos_ );
	const Text::FontColor color = ( Inventory::totalFundsShown() < 0 )
		? Text::FontColor::DARK_MID_GRAY
		: Text::FontColor::BLACK;
	Text::renderNumber( Inventory::totalFundsShown(), AREA.x + 24, ROW_2, 9, color );
}
