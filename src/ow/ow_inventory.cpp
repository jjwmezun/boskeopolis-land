#include "render.h"
#include "input.h"
#include "level.h"
#include "ow_inventory.hpp"
#include "text.h"

constexpr sdl2::SDLRect OWInventory::AREA;

OWInventory::OWInventory()
:
	inventory_ ()
{};

OWInventory::OWInventory( const Inventory& inventory )
:
	inventory_ ( inventory )
{};

void OWInventory::load()
{
	inventory_.load();
};

void OWInventory::update( const Input& input, int lv_select )
{
	inventory_.update();
	
	if ( !input.held( Input::Action::CANCEL ) )
	{
		show_challenges_lock_ = false;
	}
	else if ( lv_select <= 0 )
	{
		show_challenges_lock_ = true;
	}
	
	if ( !show_challenges_lock_ && input.held( Input::Action::CANCEL ) )
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
	//Text::renderNumber( lv_select, LEFT_EDGE, AREA.y + 8 );
	
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
			gem_score  = inventory_.gemScore( lv_select );
			time_score = inventory_.timeScore( lv_select );
			
			if ( inventory_.levelComplete( lv_select ) )
			{
				name_shade = ( Text::FontShade )( color_animation_ );
			}
			
			if ( inventory_.gemChallengeBeaten( lv_select ) )
			{
				gem_shade = ( Text::FontShade )( color_animation_ );
			}
			
			if ( inventory_.timeChallengeBeaten( lv_select ) )
			{
				time_shade = ( Text::FontShade )( color_animation_ );
			}
		}
		
		Text::renderText( Level::NameOLevel( ( unsigned int )( lv_select ) ), LEFT_EDGE + 24, ROW_1, nullptr, name_shade );

		gem_icon_gfx_.render( { RIGHT_EDGE - ( 12 * 8 ), ROW_1, 8, 8 } );
		Text::renderText( gem_score, RIGHT_EDGE - ( 11 * 8 ), ROW_1, nullptr, gem_shade );
		
		time_icon_gfx_.render( { RIGHT_EDGE - ( 5 * 8 ), ROW_1, 8, 8 } );
		Text::renderText( time_score, RIGHT_EDGE - ( 4 * 8 ), ROW_1, nullptr, time_shade );
		
		diamond_gfx_.current_frame_x_ = 56;
		if ( inventory_.haveDiamond( lv_select ) )
		{
			diamond_gfx_.current_frame_x_ = 48;
		}
		diamond_gfx_.render( { LEFT_EDGE, ROW_1, 8, 8 } );
		
		win_icon_gfx_.current_frame_x_ = 56;
		if ( inventory_.victory( lv_select ) )
		{
			win_icon_gfx_.current_frame_x_ = 40;
		}
		win_icon_gfx_.render( { LEFT_EDGE + 8, ROW_1, 8, 8 } );
	}

	gem_icon_gfx_.render( { LEFT_EDGE, ROW_2, 8, 8 } );
	
	if ( inventory_.totalFundsShown() < 0 )
	{
		Text::renderNumber( inventory_.totalFundsShown(), AREA.x + 24, ROW_2, 9, Text::FontShade::DARK_MID_GRAY );
	}
	else
	{
		Text::renderNumber( inventory_.totalFundsShown(), AREA.x + 24, ROW_2, 9 );
	}
};

const Inventory& OWInventory::inventory() const
{
	return inventory_;
};