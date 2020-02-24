#include "camera.hpp"
#include "clock.hpp"
#include "event_system.hpp"
#include "inventory.hpp"
#include "inventory_level.hpp"
#include "inventory_level_graphics.hpp"
#include "map.hpp"
#include "mezun_helpers.hpp"
#include "render.hpp"
#include "sprite.hpp"
#include "wtext_obj.hpp"

static constexpr int MISC_X = 185;
static constexpr int FLASHING_TIMER_SHADES_NUM = 12;
static constexpr WTextCharacter::Color FLASHING_TIMER_SHADES[ FLASHING_TIMER_SHADES_NUM ] =
{
	WTextCharacter::Color::BLACK,
	WTextCharacter::Color::DARK_GRAY,
	WTextCharacter::Color::DARK_MID_GRAY,
	WTextCharacter::Color::LIGHT_MID_GRAY,
	WTextCharacter::Color::LIGHT_GRAY,
	WTextCharacter::Color::WHITE,
	WTextCharacter::Color::WHITE,
	WTextCharacter::Color::LIGHT_GRAY,
	WTextCharacter::Color::LIGHT_MID_GRAY,
	WTextCharacter::Color::DARK_MID_GRAY,
	WTextCharacter::Color::DARK_GRAY,
	WTextCharacter::Color::BLACK
};
static constexpr int FLASHING_TIMER_SPEED = 8;

InventoryLevelGraphics::InventoryLevelGraphics( int max_hp )
:
	showing_key_ ( false ),
	show_on_off_ ( false ),
	on_off_state_ ( false ),
	flashing_timer_ ( 0 ),
	flashing_time_shade_ ( 0 ),
	kill_count_ ( -1 ),
	mcguffins_to_render_ ( -1 ),
	main_texture_ ( Unit::WINDOW_WIDTH_PIXELS, HEIGHT, 0, Y ),
	health_gfx_ ( Y, max_hp ),
	ticker_ ( BOTTOM_ROW_Y ),
	oxygen_meter_ ( TOP_ROW_Y )
{};

InventoryLevelGraphics::~InventoryLevelGraphics()
{
	main_texture_.destroy();
};

void InventoryLevelGraphics::update( const EventSystem& events, const Health& health, const InventoryLevel& inventory, bool funds_changed, bool time_changed )
{
	oxygen_meter_.update( health );
	if ( funds_changed )
	{
		updatePtsGraphics( inventory );
	}
	if ( time_changed )
	{
		updateTimerGraphics( inventory );
	}
	if ( health_gfx_.update( health ) )
	{
		updateHealthGraphics();
	}
	ticker_.update();
	if ( inventory.clock().lowOnTime() )
	{
		++flashing_timer_;
		if ( flashing_timer_ >= inventory.clock().timeRemaining() )
		{
			flashing_timer_ = 0;
			++flashing_time_shade_;
			if ( flashing_time_shade_ == FLASHING_TIMER_SHADES_NUM )
			{
				flashing_time_shade_ = 0;
			}
			updateTimerGraphics( inventory );
		}
	}
	if ( !showing_key_ && events.hasKey() )
	{
		updateKeyGraphics();
		showing_key_ = true;
	}
	if ( show_on_off_ && on_off_state_ != events.isSwitchOn() )
	{
		on_off_state_ = events.isSwitchOn();
		updateSwitchGraphics();
	}
	if ( mcguffins_to_render_ != -1 && mcguffins_to_render_ != inventory.mcguffins() )
	{
		mcguffins_to_render_ = inventory.mcguffins();
		updateMcGuffinGraphics();
	}
};

void InventoryLevelGraphics::init( const Map& lvmap, const InventoryLevel& inventory )
{
	show_on_off_ = lvmap.show_on_off_;
	main_texture_.init();
	forceRerender( inventory );
};

void InventoryLevelGraphics::render( const EventSystem& events, const Sprite& hero, const Camera& camera, const InventoryLevel& inventory )
{
	main_texture_.render();

	// OXYGEN
	oxygen_meter_.render();

	// TICKER
	if ( events.showBossUI() )
	{
		events.renderBossUI();
	}
	else
	{
		ticker_.render();
	}

	// BOPS
	if ( inventory.bopsMultiplier() )
	{
		WTextObj text( mezun::intToChar32String( inventory.howManyBops() ), camera.relativeX( hero.centerXPixels() - 4 ), camera.relativeY( hero.yPixels() - 12 ), WTextCharacter::Color::DARK_GRAY );
		text.render();
	}
	else if ( inventory.multipleGhostKills() )
	{
		WTextObj text( mezun::intToChar32String( inventory.howManyGhostKills() ), camera.relativeX( hero.centerXPixels() - 4 ), camera.relativeY( hero.yPixels() - 12 ), WTextCharacter::Color::DARK_GRAY );
		text.render();
	}
};

void InventoryLevelGraphics::setShowMcGuffins()
{
	if ( mcguffins_to_render_ == -1 )
	{
		mcguffins_to_render_ = 0;
	}
};

void InventoryLevelGraphics::changeKillCounter( int count )
{
	kill_count_ = count;
	updateKillCountGraphics();
};

void InventoryLevelGraphics::updateHealthGraphics()
{
	main_texture_.startDrawing();
	health_gfx_.render();
	main_texture_.endDrawing();
};

void InventoryLevelGraphics::forceRerender( const InventoryLevel& inventory )
{
	main_texture_.startDrawing();
	Render::clearScreenTransparency();
	Render::renderObject( "bg/level-inventory-frame.png", { 0, 0, Unit::WINDOW_WIDTH_PIXELS, HEIGHT }, { 0, 0, Unit::WINDOW_WIDTH_PIXELS, HEIGHT } );
	if ( Inventory::victory() )
	{
		Render::renderObject( "bg/level-select-characters.png", { 16, 40, 8, 8 }, { 10, TOP_ROW_Y_RELATIVE, 8, 8 } );
	}
	if ( Inventory::haveDiamond() )
	{
		Render::renderObject( "bg/level-select-characters.png", { 16, 32, 8, 8 }, { 18, TOP_ROW_Y_RELATIVE, 8, 8 } );
	}
	health_gfx_.render();
	renderPtsGraphics( inventory );
	renderTimerGraphics( inventory );
	if ( kill_count_ != -1 )
	{
		renderKillCountGraphics();
	}
	if ( showing_key_ )
	{
		renderKeyGraphics();
	}
	if ( show_on_off_ )
	{
		renderSwitchGraphics();
	}
	if ( mcguffins_to_render_ != -1 )
	{
		renderMcGuffinGraphics();
	}
	main_texture_.endDrawing();
	ticker_.forceRedraw();
};

void InventoryLevelGraphics::updatePtsGraphics( const InventoryLevel& inventory )
{
	main_texture_.startDrawing();
	Render::renderRect( { 42, TOP_ROW_Y_RELATIVE, 8 * 5, 8 }, 1 );
	renderPtsGraphics( inventory );
	main_texture_.endDrawing();
};

void InventoryLevelGraphics::renderPtsGraphics( const InventoryLevel& inventory )
{
	WTextObj text{ inventory.fundsString(), 42, TOP_ROW_Y_RELATIVE };
	text.render();
};

void InventoryLevelGraphics::updateTimerGraphics( const InventoryLevel& inventory )
{
	main_texture_.startDrawing();
	Render::renderRect( { CLOCK_X, TOP_ROW_Y_RELATIVE, 8 * 4, 8 }, 1 );
	renderTimerGraphics( inventory );
	main_texture_.endDrawing();
};

void InventoryLevelGraphics::renderTimerGraphics( const InventoryLevel& inventory )
{
	WTextObj text{ inventory.clock().getTimeString(), CLOCK_X, TOP_ROW_Y_RELATIVE, FLASHING_TIMER_SHADES[ flashing_time_shade_ ] };
	text.render();
};

void InventoryLevelGraphics::updateKillCountGraphics()
{
	main_texture_.startDrawing();
	Render::renderRect( { MISC_X, TOP_ROW_Y_RELATIVE, 8 * 4, 8 }, 1 );
	renderKillCountGraphics();
	main_texture_.endDrawing();
};

void InventoryLevelGraphics::renderKillCountGraphics()
{
	Render::renderObject( "bg/level-select-characters.png", { 0, 184, 8, 8 }, { MISC_X, TOP_ROW_Y_RELATIVE, 8, 8 } );
	WTextObj text{ U"x" + mezun::intToChar32StringWithPadding( kill_count_, 2 ), MISC_X + 8, TOP_ROW_Y_RELATIVE };
	text.render();
};

void InventoryLevelGraphics::updateKeyGraphics()
{
	main_texture_.startDrawing();
	Render::renderRect( { MISC_X, TOP_ROW_Y_RELATIVE, 8, 8 }, 1 );
	renderKeyGraphics();
	main_texture_.endDrawing();
};

void InventoryLevelGraphics::renderKeyGraphics()
{
	Render::renderObject( "bg/level-select-characters.png", { 8, 184, 8, 8 }, { MISC_X, TOP_ROW_Y_RELATIVE, 8, 8 } );
};

void InventoryLevelGraphics::updateSwitchGraphics()
{
	main_texture_.startDrawing();
	Render::renderRect( { MISC_X, TOP_ROW_Y_RELATIVE, 8 * 3, 8 }, 1 );
	renderSwitchGraphics();
	main_texture_.endDrawing();
};

void InventoryLevelGraphics::renderSwitchGraphics()
{
	const std::u32string string = ( on_off_state_ ) ? U"ON" : U"OFF";
	WTextObj text( string, MISC_X, TOP_ROW_Y_RELATIVE );
	text.render();
};

void InventoryLevelGraphics::updateMcGuffinGraphics()
{
	main_texture_.startDrawing();
	Render::renderRect( { MISC_X, TOP_ROW_Y_RELATIVE, 8 * 3, 8 }, 1 );
	renderMcGuffinGraphics();
	main_texture_.endDrawing();
};

void InventoryLevelGraphics::renderMcGuffinGraphics()
{
	Render::renderObject( "bg/level-select-characters.png", { 16, 184, 8, 8 }, { MISC_X, TOP_ROW_Y_RELATIVE, 8, 8 } );
	WTextObj text{ U"x" + mezun::intToChar32String( mcguffins_to_render_ ), MISC_X + 8, TOP_ROW_Y_RELATIVE };
	text.render();
};