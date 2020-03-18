#pragma once

class Camera;
class EventSystem;
class Health;
class InventoryLevel;
class Map;
class Sprite;

#include "inventory_level_health.hpp"
#include "mezun_sdl2.hpp"
#include "news_ticker.hpp"
#include "oxygen_meter.hpp"
#include "sprite_graphics.hpp"
#include "text_obj.hpp"
#include "texture_box.hpp"

class InventoryLevelGraphics final
{
	public:
		static constexpr int HEIGHT = 40;
		static constexpr int Y = Unit::WINDOW_HEIGHT_PIXELS - HEIGHT;
		static constexpr int TOP_ROW_Y_RELATIVE = 10;
		static constexpr int TOP_ROW_Y = Y + TOP_ROW_Y_RELATIVE;
		static constexpr int BOTTOM_ROW_Y = Y + 23;
		static constexpr int CLOCK_ICON_X = Unit::MiniBlocksToPixels( 17 );
		static constexpr int CLOCK_X = 146;

		InventoryLevelGraphics( int max_hp );
		~InventoryLevelGraphics();
		InventoryLevelGraphics( const InventoryLevelGraphics& ) = delete;
		InventoryLevelGraphics( InventoryLevelGraphics&& ) = delete;
		InventoryLevelGraphics& operator=( const InventoryLevelGraphics& ) = delete;
		InventoryLevelGraphics& operator=( InventoryLevelGraphics&& ) = delete;
		void init( const Map& lvmap, const InventoryLevel& inventory );
		void update( const EventSystem& events, const Health& health, const InventoryLevel& inventory, bool funds_changed, bool time_changed );
		void render( const EventSystem& events, const Sprite& hero, const Camera& camera, const InventoryLevel& inventory );
		void setShowMcGuffins();
		void changeKillCounter( int count );
		void forceRerender( const InventoryLevel& inventory );
		void reRenderDiamond();

	private:
		void updateHealthGraphics();
		void updatePtsGraphics( const InventoryLevel& inventory );
		void renderPtsGraphics( const InventoryLevel& inventory );
		void updateTimerGraphics( const InventoryLevel& inventory );
		void renderTimerGraphics( const InventoryLevel& inventory );
		void updateKillCountGraphics();
		void renderKillCountGraphics();
		void updateKeyGraphics();
		void renderKeyGraphics();
		void updateSwitchGraphics();
		void renderSwitchGraphics();
		void updateMcGuffinGraphics();
		void renderMcGuffinGraphics();
		void renderDiamond();
		
		bool showing_key_;
		bool show_on_off_;
		bool on_off_state_;
		int flashing_timer_;
		int flashing_time_shade_;
		int kill_count_;
		int mcguffins_to_render_;
		TextureBox main_texture_;
		InventoryLevelHealth health_gfx_;
		NewsTicker ticker_;
		OxygenMeter oxygen_meter_;
};
