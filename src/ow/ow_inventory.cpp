#include "input.hpp"
#include "inventory.hpp"
#include "level.hpp"
#include "ow_inventory.hpp"
#include "render.hpp"
#include "text.hpp"

constexpr sdl::rect OWInventory::AREA;

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
	}
	else
	{
		show_challenges_ = false;
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
	Render::renderRect( AREA );
	
	if ( lv_select > 0 )
	{
		Text::FontShade name_shade = Text::FontShade::BLACK;
		Text::FontShade gem_shade = Text::FontShade::BLACK;
		Text::FontShade time_shade = Text::FontShade::BLACK;

		std::string gem_score;
		std::string time_score;
		
		if ( show_challenges_ )
		{
			gem_score  = Level::gemChallengeText( lv_select );
			time_score = Level::timeChallengeText( lv_select );
			name_shade = Text::FontShade::DARK_MID_GRAY;
			gem_shade = Text::FontShade::DARK_MID_GRAY;
			time_shade = Text::FontShade::DARK_MID_GRAY;
		}
		else
		{
			gem_score  = Inventory::gemScore( lv_select );
			time_score = Inventory::timeScore( lv_select );
			
			if ( Inventory::levelComplete( lv_select ) )
			{
				name_shade = ( Text::FontShade )( color_animation_ );
			}
			
			if ( Inventory::gemChallengeBeaten( lv_select ) )
			{
				gem_shade = ( Text::FontShade )( color_animation_ );
			}
			
			if ( Inventory::timeChallengeBeaten( lv_select ) )
			{
				time_shade = ( Text::FontShade )( color_animation_ );
			}
		}
		
		Text::renderText( Level::NameOLevel( ( unsigned int )( lv_select ) ), LEFT_EDGE + 24, ROW_1, nullptr, name_shade );

		gem_icon_gfx_.render( { RIGHT_EDGE - ( 12 * 8 ), ROW_1, 8, 8 } );
		Text::renderText( gem_score, RIGHT_EDGE - ( 11 * 8 ), ROW_1, nullptr, gem_shade );
		
		time_icon_gfx_.render( { RIGHT_EDGE - ( 5 * 8 ), ROW_1, 8, 8 } );
		Text::renderText( time_score, RIGHT_EDGE - ( 4 * 8 ), ROW_1, nullptr, time_shade );
		
		win_icon_gfx_.current_frame_x_ = DONT_HAVE_X;
		if ( Inventory::victory( lv_select ) )
		{
			win_icon_gfx_.current_frame_x_ = HAVE_X;
		}
		win_icon_gfx_.render( { LEFT_EDGE, ROW_1, 8, 8 } );

		diamond_gfx_.current_frame_x_ = DONT_HAVE_X;
		if ( Inventory::haveDiamond( lv_select ) )
		{
			diamond_gfx_.current_frame_x_ = HAVE_X;
		}
		diamond_gfx_.render( { LEFT_EDGE + 8, ROW_1, 8, 8 } );
	}

	gem_icon_gfx_.render( { LEFT_EDGE, ROW_2, 8, 8 } );
	
	if ( Inventory::totalFundsShown() < 0 )
	{
		Text::renderNumber( Inventory::totalFundsShown(), AREA.x + 24, ROW_2, 9, Text::FontShade::DARK_MID_GRAY );
	}
	else
	{
		Text::renderNumber( Inventory::totalFundsShown(), AREA.x + 24, ROW_2, 9 );
	}
};